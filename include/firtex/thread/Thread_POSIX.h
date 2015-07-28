#ifndef __THREAD_POSIX_H
#define __THREAD_POSIX_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/Runnable.h"
#include "firtex/common/SharedPtr.h"
//#include "firtex/utility/SignalHandler.h"
#include "firtex/thread/Event.h"
#include "firtex/thread/RWLock.h"
#include <pthread.h>
#include <signal.h>
#include <memory>
#include <errno.h>
#include <set>

FX_NS_DEF(thread);

class ThreadImpl
{
public:	
    enum Priority
    {
        PRIO_LOWEST_IMPL,
        PRIO_LOW_IMPL,
        PRIO_NORMAL_IMPL,
        PRIO_HIGH_IMPL,
        PRIO_HIGHEST_IMPL
    };

    typedef pthread_t threadid_t;
    const static threadid_t INVALID_THREADID;

    ThreadImpl();				
    ~ThreadImpl();

    void setPriorityImpl(int prio);
    int getPriorityImpl() const;
    static int getMinOSPriorityImpl();
    static int getMaxOSPriorityImpl();
    void setStackSizeImpl(size_t size);
    size_t getStackSizeImpl() const;
    void startImpl(Runnable& target);

    void joinImpl();
    bool joinImpl(long milliseconds);
    bool isRunningImpl() const;
    static void sleepImpl(long milliseconds);
    static void yieldImpl();
    static ThreadImpl* currentImpl();
    static threadid_t getSysThreadIdImpl();
    inline static bool isThreadAliveImpl(threadid_t id);

protected:
    static void* runnableEntry(void* pThread);
    static int mapPrio(int prio);
    static int reverseMapPrio(int osPrio);

    static void activeThread();
    static void deactiveThread();

private:
    class TSS
    {
    public:
        TSS()
        {
            if (pthread_key_create(&_key, NULL))
                FIRTEX_THROW(SystemException, "Cannot allocate thread context key");
        }

        ~TSS()
        {
            pthread_key_delete(_key);
        }

        ThreadImpl* get() const
        {
            return reinterpret_cast<ThreadImpl*>(pthread_getspecific(_key));
        }

        void set(ThreadImpl* pThread)
        {
            pthread_setspecific(_key, pThread);
        }
	
    private:
        pthread_key_t _key;
    };

    struct ThreadData
    {
        ThreadData()
            : pRunnableTarget(0)
            , thread(0)
            , prio(PRIO_NORMAL_IMPL)
            , done(false)
            , stackSize(0)
        {
        }

        Runnable*   pRunnableTarget;
        pthread_t   thread;
        int         prio;
        int         osPrio;
        Event       done;
        std::size_t stackSize;
    };
    DEFINE_TYPED_PTR(ThreadData);

    ThreadDataPtr m_pData;

#if defined(FX_UNIX)
    SignalHandler::JumpBufferVec _jumpBufferVec;
    friend class SignalHandler;
#endif

public:    
    static TSS m_threadMap;

private:
    typedef std::set<threadid_t> ThreadIdSet;

    static ThreadIdSet s_activeThreads;
    static RWLock s_mapLock;
};

/////////////////////////////////////////////
// inlines
//
inline int ThreadImpl::getPriorityImpl() const
{
    return m_pData->prio;
}

inline void ThreadImpl::sleepImpl(long milliseconds)
{
    // This is specific to DECThreads
    timespec interval;
    interval.tv_sec  = milliseconds / 1000;
    interval.tv_nsec = (milliseconds % 1000)*1000000;
    while (nanosleep(&interval, &interval) == -1)
    {
        if (errno == EINTR)
        {
            continue;
        }
        else
        {
            FIRTEX_THROW(SystemException, "nanosleep failed");
        }
    }
}

inline bool ThreadImpl::isRunningImpl() const
{
    return (m_pData->pRunnableTarget != 0);  
}

inline void ThreadImpl::yieldImpl()
{
    sched_yield();
}

inline size_t ThreadImpl::getStackSizeImpl() const
{
    return m_pData->stackSize;
}

inline ThreadImpl::threadid_t ThreadImpl::getSysThreadIdImpl()
{
    return pthread_self();
}

inline ThreadImpl* ThreadImpl::currentImpl()
{
    return m_threadMap.get();
}

inline void ThreadImpl::activeThread()
{
    ScopedRWLock wLock(s_mapLock, true);
    s_activeThreads.insert(getSysThreadIdImpl());
}

inline void ThreadImpl::deactiveThread()
{
    ScopedRWLock wLock(s_mapLock, true);
    s_activeThreads.erase(getSysThreadIdImpl());
}

inline bool ThreadImpl::isThreadAliveImpl(threadid_t id)
{
    // if sig is zero, error checking is performed but no signal is actually sent. 
    // ESRCH - No thread could be found corresponding to that specified by the given thread ID
    // Unresolved problem: Linux may crash on dead thread_id.  Workaround unknown (except signal handler...)
    // return pthread_kill(id, 0) != ESRCH;

    ScopedRWLock wLock(s_mapLock, false);
    return (s_activeThreads.find(id) != s_activeThreads.end());
}

FX_NS_END

#endif //
