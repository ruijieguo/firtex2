#ifndef __FXTHREADPOOL_H
#define __FXTHREADPOOL_H

#include "firtex/common/SharedPtr.h"
#include "firtex/thread/Thread.h"
#include "firtex/thread/Mutex.h"
#include "firtex/thread/Runnable.h"
#include "firtex/thread/Condition.h"
#include <vector>

FX_NS_DEF(thread);

class PooledThread;
DEFINE_TYPED_PTR(PooledThread);

/**
 * @class ThreadPool
 *
 * @brief A thread pool always keeps a number of threads running, ready
 *  to accept work.
 * The thread pool always keeps a minimum number of threads
 * running. If the demands for threads increases, additional
 * threads are created. Once the demand for threads sinks
 * again, no-longer used threads are stopped and removed
 * from the pool.
 */
class ThreadPool
{
public:
    ThreadPool(size_t nMinCapacity = 2, size_t nMaxCapacity = 16,
               size_t nIdleTime = 60, size_t nStackSize = 0);
    ThreadPool(const std::string& sName, size_t nMinCapacity = 2,
               size_t nMaxCapacity = 16, size_t nIdleTime = 60,
               size_t nStackSize = 0);

    ~ThreadPool();

public:	
    /**
     * Increase (or decrease, if n is negative) 
     * maximum number of threads.
     *
     * @param nCapacity maximum number of threads.
     */
    void addCapacity(int32_t nCapacity);

    /// Return the maximum capacity of threads.
    size_t capacity() const;

    /**
     * Sets the stack size for threads.
     * New stack size applies only for newly created threads.
     */
    void setStackSize(size_t nStackSize);

    /** Return the stack size used to create new threads.*/
    size_t getStackSize() const;

    /** Return the number of currently used threads.*/
    size_t numUsedThreads();

    /** Return the number of currently allocated threads.*/
    size_t numAllocatedThreads() const;

    /** Return the number available threads.*/
    size_t numAvailableThreads();

    /*
     * Obtains a thread and starts the target.
     * @param target runnable object
     * @param bBlock whether block when no available threads
     * @throw throw NoThreadAvailableException if no more
     * threads are available.
     */
    void start(FX_NS(thread)::Runnable& target, bool bBlock = false);

    /**
     * Obtains a thread and starts the target.
     * @param target runnable object
     * @param sName assign the given name to thread
     * @throw throw NoThreadAvailableException if no more
     */
    void start(FX_NS(thread)::Runnable& target, const std::string& sName);

    /**
     * Obtains a thread, adjusts the thread's priority, and starts the target.
     *
     * @param priority priority of thread
     * @param target runnable object
     * @throw throw NoThreadAvailableException if no more
     */
    void startWithPriority(FX_NS(thread)::Thread::Priority priority, FX_NS(thread)::Runnable& target);

    /**
     * Obtains a thread, adjusts the thread's priority, and starts the target.
     *
     * @param priority priority of thread
     * @param target runnable object
     * @param sName assign the given name to thread
     * @throw throw NoThreadAvailableException if no more
     */
    void startWithPriority(FX_NS(thread)::Thread::Priority priority,
                           FX_NS(thread)::Runnable& target, const std::string& sName);

    /** Stops all running threads.*/
    void stopAll();

    /// Waits for all threads to complete.
    void joinAll();

    /**
     * Stops and removes no longer used threads from the thread pool. 
     */
    void collect();

public:
    /// Return a reference to the default thread pool.
    static ThreadPool& defaultPool();

protected:
    PooledThreadPtr getThread(bool bBlock);
    PooledThread* createThread();
    void housekeep();
    void broadcast();

    friend class PooledThread;
private:
    ThreadPool(const ThreadPool& pool);
    ThreadPool& operator = (const ThreadPool& pool);

    typedef std::vector<PooledThreadPtr> ThreadVec;

    std::string m_sName;
    size_t m_nMinCapacity;
    size_t m_nMaxCapacity;
    size_t m_nIdleTime;
    size_t m_nSerial;
    size_t m_nAge;
    size_t m_nStackSize;
    ThreadVec m_threads;
    mutable FX_NS(thread)::FastMutex m_mutex;

    mutable FX_NS(thread)::FastMutex m_condMutex;
    mutable FX_NS(thread)::Condition m_cond;
    bool m_bStopped;
};

DEFINE_TYPED_PTR(ThreadPool);

////////////////////////////////////////////////
// inlines
//
inline void ThreadPool::setStackSize(size_t nStackSize)
{
    m_nStackSize = nStackSize;
}

inline size_t ThreadPool::getStackSize() const
{
    return m_nStackSize;
}

FX_NS_END

#endif 
