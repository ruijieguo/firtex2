#ifndef __FXTHREAD_H
#define __FXTHREAD_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/FastMutex.h"
#include "firtex/thread/ThreadLocal.h"
#include <string>

#if defined(FX_WINDOWS)
#include "firtex/thread/Thread_WIN32.h"
#else
#include "firtex/thread/Thread_POSIX.h"
#endif

FX_NS_DEF(thread);

class Runnable;

/**
 * @class Thread
 *
 * This class implements a platform-independent
 * wrapper to an operating system thread.
 *
 */
class Thread : private ThreadImpl
{
public:
    /// Thread priorities.
    enum Priority
    {
        PRIO_LOWEST  = PRIO_LOWEST_IMPL, /// The lowest thread priority.
        PRIO_LOW     = PRIO_LOW_IMPL,    /// A lower than normal thread priority.
        PRIO_NORMAL  = PRIO_NORMAL_IMPL, /// The normal thread priority.
        PRIO_HIGH    = PRIO_HIGH_IMPL,   /// A higher than normal thread priority.
        PRIO_HIGHEST = PRIO_HIGHEST_IMPL /// The highest thread priority.
    };

    typedef ThreadImpl::threadid_t threadid_t;

    using ThreadImpl::INVALID_THREADID;

public:
    /** Creates a thread. Call start() to start it.*/
    Thread();
    
    /** Creates a named thread. Call start() to start it.*/
    Thread(const std::string& name);
    
    /** Destroys the thread.*/
    ~Thread();

    /** Return the unique thread ID of the thread.*/
    int getId() const;

    /** Return teh name of the thread.*/
    std::string getName() const;

    /** Sets the name of the thread.*/
    void setName(const std::string& name);

    /**
     * Sets the thread's priority.

     * @param prio priority of the thread
     */
    void setPriority(Priority prio);

    /** Return the thread's priority.*/
    Priority getPriority() const;

    /**
     * Sets the thread's stack size in bytes.
     * @param size Stack size to set, set stack size to 0 will 
     *        use the default stack size.
     */
    void setStackSize(size_t size);
    
    /**
     * Return the thread's stack size in bytes.
     * @return stack size of thread. If the default 
     *         stack size is used, 0 is returned.
     */
    size_t getStackSize() const;

    /**
     * Starts the thread with the given target.
     */
    void start(Runnable& target);
    
    /**
     * Waits until the thread completes execution.
     * If multiple threads try to join the same
     * thread, the result is undefined.
     */
    void join();

    /**
     * Waits for at most the given interval for the thread
     * to complete. 
     * @param milliseconds milliseconds to wait
     * @exception Throws a TimeoutException if the thread
     *            does not complete within the specified time interval.
     */
    void join(long milliseconds);

    /**
     * Waits for at most the given interval for the thread
     * to complete. 
     * @param milliseconds milliseconds to wait
     * @return true if the thread has finished, false otherwise.
     */
    bool tryJoin(long milliseconds);

    /** Return true if the thread is running. */
    bool isRunning() const;

public:
    /**
     * Suspends the current thread for the specified
     * amount of time.
     * @param milliseconds milliseconds to sleep
     */
    static void sleep(long milliseconds);

    /**
     * Yields cpu to other threads.
     */
    static void yield();
    
    /**
     * Return the Thread object for the currently active thread.
     * If the current thread is the main thread, 0 is returned.
     */
    static Thread* current();

    /// Return current thread id (system)
    static threadid_t getSysThreadId();

    /// Test whether the thread is alive
    static bool isThreadAlive(threadid_t id);

public:
    /// Return a reference to the thread's local storage.
    ThreadLocalStorage& tls();

    /// Clears the thread's local storage.
    void clearTLS();

protected:
    /// Creates a unique name for a thread.
    std::string makeName();

    /// Creates and returns a unique id for a thread.
    static int uniqueId();
    
private:
    Thread(const Thread&);
    Thread& operator = (const Thread&);

private:
    int32_t m_nId;
    std::string m_sName;
    ThreadLocalStorage* m_pTLS;
    mutable FastMutex m_mutex;
};

DEFINE_TYPED_PTR(Thread);

/////////////////////////////////////////////////
// inlines
//
inline int Thread::getId() const
{
    return m_nId;
}

inline std::string Thread::getName() const
{
    FastMutex::Guard lock(m_mutex);
    
    return m_sName;
}

inline bool Thread::isRunning() const
{
    return isRunningImpl();
}

inline void Thread::sleep(long milliseconds)
{
    sleepImpl(milliseconds);
}

inline void Thread::yield()
{
    yieldImpl();
}

inline Thread::threadid_t Thread::getSysThreadId()
{
    return getSysThreadIdImpl();
}

inline bool Thread::isThreadAlive(threadid_t id)
{
    return isThreadAliveImpl(id);
}

inline void Thread::setStackSize(size_t size)
{
    setStackSizeImpl(size);
}

inline size_t Thread::getStackSize() const
{
    return getStackSizeImpl();
}

FX_NS_END

#endif 
