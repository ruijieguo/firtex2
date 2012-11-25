#ifndef __THREAD_WIN32_H
#define __THREAD_WIN32_H

#include "firtex/common/StdHeader.h"
#include "firtex/thread/Runnable.h"
#include "firtex/utility/Exception.h"
#include <windows.h>

FX_NS_DEF(thread);

class ThreadImpl
{
public:
    typedef DWORD threadid_t;
    const static threadid_t INVALID_THREADID;

    class TSS
    {
    public:
        TSS() : m_slot(TlsAlloc())
        {
            if (m_slot == TLS_OUT_OF_INDEXES)
            {
                FIRTEX_THROW(SystemException, "cannot allocate thread context key");
            }
        }
        ~TSS()
        {
            TlsFree(m_slot);
        }

        ThreadImpl* get() const
        {
            return reinterpret_cast<ThreadImpl*>(TlsGetValue(m_slot));
        }
        void set(ThreadImpl* pThread)
        {
            TlsSetValue(m_slot, pThread);
        }
	
    private:
        DWORD m_slot;
    };

public:	
#if defined(_DLL)
    typedef DWORD (WINAPI *Entry)(LPVOID);
#else
    typedef unsigned (__stdcall *Entry)(void*);
#endif

    enum Priority
    {
        PRIO_LOWEST_IMPL  = THREAD_PRIORITY_LOWEST,
        PRIO_LOW_IMPL     = THREAD_PRIORITY_BELOW_NORMAL,
        PRIO_NORMAL_IMPL  = THREAD_PRIORITY_NORMAL,
        PRIO_HIGH_IMPL    = THREAD_PRIORITY_ABOVE_NORMAL,
        PRIO_HIGHEST_IMPL = THREAD_PRIORITY_HIGHEST
    };

    ThreadImpl();				
    ~ThreadImpl();

public:
    void setPriorityImpl(int prio);
    int getPriorityImpl() const;
    void setStackSizeImpl(size_t size);
    size_t getStackSizeImpl() const;
    void startImpl(Runnable& target);

    void joinImpl();
    bool joinImpl(long milliseconds);
    bool isRunningImpl() const;

public:
    static void sleepImpl(long milliseconds);
    static void yieldImpl();
    static ThreadImpl* currentImpl();
    static threadid_t getSysThreadIdImpl();
    /// Tests whether the thread is alive
    inline static bool isThreadAliveImpl(threadid_t id);


protected:
#if defined(_DLL)
    static DWORD WINAPI runnableEntry(LPVOID pThread);
#else
    static unsigned __stdcall runnableEntry(void* pThread);
#endif

    void createImpl(Entry ent, void* pData);
    void threadCleanup();

private:
    Runnable* m_pRunnableTarget;
    HANDLE m_hThread;
    int m_nPrio;
    size_t m_nStackSize;

protected:
    static TSS m_threadMap;
};

//////////////////////////////////////////////
// inlines
//
inline int ThreadImpl::getPriorityImpl() const
{
    return m_nPrio;
}

inline void ThreadImpl::sleepImpl(long milliseconds)
{
    Sleep(DWORD(milliseconds));
}

inline void ThreadImpl::yieldImpl()
{
    Sleep(0);
}

inline void ThreadImpl::setStackSizeImpl(size_t size)
{
    m_nStackSize = size;
}

inline size_t ThreadImpl::getStackSizeImpl() const
{
    return m_nStackSize;
}

inline ThreadImpl::threadid_t ThreadImpl::getSysThreadIdImpl()
{
    return GetCurrentThreadId();
}

inline ThreadImpl* ThreadImpl::currentImpl()
{
    return m_threadMap.get();
}

inline bool ThreadImpl::isThreadAliveImpl(threadid_t id)
{
    HANDLE h = ::OpenThread(SYNCHRONIZE, FALSE, id);
    if ( h == NULL )
        return false;
    ::CloseHandle(h);
    return true;
}

FX_NS_END

#endif
