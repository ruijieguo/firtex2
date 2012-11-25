#include "firtex/thread/Thread_WIN32.h"
#include "firtex/utility/Exception.h"
#include "firtex/common/Logger.h"
#include <process.h>

FX_NS_USE(utility);
FX_NS_DEF(thread);

ThreadImpl::TSS ThreadImpl::m_threadMap;
const ThreadImpl::threadid_t ThreadImpl::INVALID_THREADID = 0;

ThreadImpl::ThreadImpl()
    : m_pRunnableTarget(0)
    , m_hThread(0)
    , m_nPrio(PRIO_NORMAL_IMPL)
    , m_nStackSize(0)//FXM_HTHREAD_STACK_SIZE)
{

}

ThreadImpl::~ThreadImpl()
{
    if (m_hThread) 
    {
        CloseHandle(m_hThread);
    }
}

void ThreadImpl::setPriorityImpl(int prio)
{
    if (prio != m_nPrio)
    {
        m_nPrio = prio;
        if (m_hThread)
        {
            if (SetThreadPriority(m_hThread, m_nPrio) == 0)
                FIRTEX_THROW(SystemException, "cannot set thread priority");
        }
    }
}

void ThreadImpl::startImpl(Runnable& target)
{
    if (isRunningImpl())
        FIRTEX_THROW(SystemException, "thread already running");

    m_pRunnableTarget = &target;

    createImpl(runnableEntry, this);
}

void ThreadImpl::createImpl(Entry ent, void* pData)
{
#if defined(_DLL)
    DWORD threadId;
    m_hThread = CreateThread(NULL, m_nStackSize, ent, pData, 0, &threadId);
#else
    unsigned threadId;
    m_hThread = (HANDLE) _beginthreadex(NULL, m_nStackSize, runnableEntry, this, 0, &threadId);
#endif
    if (!m_hThread)
        FIRTEX_THROW(SystemException, "cannot create thread");
    if (m_nPrio != PRIO_NORMAL_IMPL && !SetThreadPriority(m_hThread, m_nPrio))
        FIRTEX_THROW(SystemException, "cannot set thread priority");
}

void ThreadImpl::joinImpl()
{
    if (!m_hThread) return;

    switch (WaitForSingleObject(m_hThread, INFINITE))
    {
    case WAIT_OBJECT_0:
        threadCleanup();
        return;
    default:
        throw SystemException("cannot join thread");
    }
}

bool ThreadImpl::joinImpl(long milliseconds)
{
    if (!m_hThread) return true;

    switch (WaitForSingleObject(m_hThread, milliseconds + 1))
    {
    case WAIT_TIMEOUT:
        return false;
    case WAIT_OBJECT_0:
        threadCleanup();
        return true;
    default:
        FIRTEX_THROW(SystemException, "cannot join thread");
    }
}

bool ThreadImpl::isRunningImpl() const
{
    if (m_hThread)
    {
        DWORD ec = 0;
        return GetExitCodeThread(m_hThread, &ec) && ec == STILL_ACTIVE;
    }
    return false;
}

void ThreadImpl::threadCleanup()
{
    if (!m_hThread) return;
    if (CloseHandle(m_hThread)) m_hThread = NULL;
}

#if defined(_DLL)
DWORD WINAPI ThreadImpl::runnableEntry(LPVOID pThread)
#else
    unsigned __stdcall ThreadImpl::runnableEntry(void* pThread)
#endif
{
    ThreadImpl* pThreadImpl = reinterpret_cast<ThreadImpl*>(pThread);
    m_threadMap.set(pThreadImpl);
    try
    {
        pThreadImpl->m_pRunnableTarget->run();
    }
    catch (const FirteXException& )
    {
        throw;
    }
    return 0;
}

FX_NS_END
