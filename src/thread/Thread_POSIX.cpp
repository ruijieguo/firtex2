#include "firtex/thread/Thread_POSIX.h"
#include "firtex/utility/Exception.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/thread/FastMutex.h"
#include <set>
#include <assert.h>

#include <signal.h>
#include <memory>
#if defined(__sun) && defined(__SVR4)
#	if !defined(__EXTENSIONS__)
#		define __EXTENSIONS__
#	endif
#	include <limits.h>
#endif


//
// Block SIGPIPE in main thread.
//
#if defined(FX_UNIX)
namespace
{
class SignalBlocker
{
public:
    SignalBlocker()
    {
        sigset_t sset;
        sigemptyset(&sset);
        sigaddset(&sset, SIGPIPE); 
        pthread_sigmask(SIG_BLOCK, &sset, 0);
    }
    ~SignalBlocker()
    {
    }
};
	
static SignalBlocker signalBlocker;
}
#endif


FX_NS_USE(utility);
FX_NS_USE(common);

FX_NS_DEF(thread);


ThreadImpl::TSS ThreadImpl::m_threadMap;
const ThreadImpl::threadid_t ThreadImpl::INVALID_THREADID = 0;

ThreadImpl::ThreadIdSet ThreadImpl::s_activeThreads;
RWLock ThreadImpl::s_mapLock;

ThreadImpl::ThreadImpl() :
    m_pData(new ThreadData)
{
}

ThreadImpl::~ThreadImpl()
{
    if (isRunningImpl())
    {
        pthread_detach(m_pData->thread);
    }
}

void ThreadImpl::setPriorityImpl(int prio)
{
    if (prio != m_pData->prio)
    {
        m_pData->prio = prio;
        if (isRunningImpl())
        {
            struct sched_param par;
            par.sched_priority = mapPrio(m_pData->prio);
            if (pthread_setschedparam(m_pData->thread, SCHED_OTHER, &par))
            {
                FIRTEX_THROW(SystemException, "cannot set thread priority");
            }
        }
    }
}

int ThreadImpl::getMinOSPriorityImpl()
{
#if defined(__VMS) || defined(__digital__)
    return PRI_OTHER_MIN;
#else
    return sched_get_priority_min(SCHED_OTHER);
#endif
}

int ThreadImpl::getMaxOSPriorityImpl()
{
#if defined(__VMS) || defined(__digital__)
    return PRI_OTHER_MAX;
#else
    return sched_get_priority_max(SCHED_OTHER);
#endif
}

void ThreadImpl::setStackSizeImpl(size_t size)
{
#ifndef PTHREAD_STACK_MIN
    m_pData->stackSize = 0;
#else
    if (size != 0)
    {
#if defined(__APPLE__)
        // we must round up to a multiple of the memory page size
        const int PAGE_SIZE = 4096;
        size = ((size + PAGE_SIZE - 1)/PAGE_SIZE)*PAGE_SIZE;
#endif
        if (size < PTHREAD_STACK_MIN)
            size = PTHREAD_STACK_MIN;
    }
    m_pData->stackSize = size;
#endif
}

void ThreadImpl::startImpl(Runnable& target)
{
    if (m_pData->pRunnableTarget)
        FIRTEX_THROW(SystemException, "thread already running");

    pthread_attr_t attributes;
    pthread_attr_init(&attributes);

    if (m_pData->stackSize != 0)
    {
        if (0 != pthread_attr_setstacksize(&attributes, m_pData->stackSize))
            FIRTEX_THROW(SystemException, "cannot set thread stack size");
    }

    m_pData->pRunnableTarget = &target;
    if (pthread_create(&m_pData->thread, &attributes, runnableEntry, this))
    {
        m_pData->pRunnableTarget = 0;
        FIRTEX_THROW(SystemException, "cannot start thread");
    }

    if (m_pData->prio != PRIO_NORMAL_IMPL)
    {
        struct sched_param par;
        par.sched_priority = mapPrio(m_pData->prio);
        if (pthread_setschedparam(m_pData->thread, SCHED_OTHER, &par))
            FIRTEX_THROW(SystemException, "cannot set thread priority");
    }
}

void ThreadImpl::joinImpl()
{
    m_pData->done.wait();
    void* result;
    if (pthread_join(m_pData->thread, &result))
    {
        FIRTEX_THROW(SystemException, "cannot join thread"); 
    }
}

bool ThreadImpl::joinImpl(long milliseconds)
{
    if (m_pData->done.tryWait(milliseconds))
    {
        void* result;
        if (pthread_join(m_pData->thread, &result))
            FIRTEX_THROW(SystemException, "cannot join thread");
        return true;
    }
    else return false;
}

void* ThreadImpl::runnableEntry(void* pThread)
{
#if defined(FX_UNIX)
    sigset_t sset;
    sigemptyset(&sset);
    sigaddset(&sset, SIGQUIT);
    sigaddset(&sset, SIGTERM);
    sigaddset(&sset, SIGPIPE); 
    pthread_sigmask(SIG_BLOCK, &sset, 0);
#endif

    ThreadImpl* pThreadImpl = reinterpret_cast<ThreadImpl*>(pThread);
    m_threadMap.set(pThreadImpl);

    ThreadDataPtr pData = pThreadImpl->m_pData;

    activeThread();

    try
    {
        pData->pRunnableTarget->run();
    }
    catch (const FirteXException& exc)
    {
        deactiveThread();
        throw;
    }
    catch (const std::exception& e)
    {
        deactiveThread();
        throw;
    }
    catch (...)
    {
        deactiveThread();
        FIRTEX_THROW(RuntimeException, "Run thread failed");
    }
    pData->pRunnableTarget = 0;
    pData->done.set();
    deactiveThread();

    return 0;
}

int ThreadImpl::mapPrio(int prio)
{
    int pmin = getMinOSPriorityImpl();
    int pmax = getMaxOSPriorityImpl();

    switch (prio)
    {
    case PRIO_LOWEST_IMPL:
        return pmin;
    case PRIO_LOW_IMPL:
        return pmin + (pmax - pmin)/4;
    case PRIO_NORMAL_IMPL:
        return pmin + (pmax - pmin)/2;
    case PRIO_HIGH_IMPL:
        return pmin + 3*(pmax - pmin)/4;
    case PRIO_HIGHEST_IMPL:
        return pmax;
    default:
        throw BadParameterException("invalid thread priority");
    }
    return -1; // just to satisfy compiler - we'll never get here anyway
}


int ThreadImpl::reverseMapPrio(int prio)
{
    int pmin = getMinOSPriorityImpl();
    int pmax = getMaxOSPriorityImpl();
    int normal = pmin + (pmax - pmin)/2;
    if (prio == pmax)
        return PRIO_HIGHEST_IMPL;
    if (prio > normal)
        return PRIO_HIGH_IMPL;
    else if (prio == normal)
        return PRIO_NORMAL_IMPL;
    else if (prio > pmin)
        return PRIO_LOW_IMPL;
    else
        return PRIO_LOWEST_IMPL;
}

FX_NS_END
