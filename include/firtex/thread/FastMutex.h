#ifndef __FASTMUTEX_H
#define __FASTMUTEX_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/NonCopyable.h"
#include "firtex/thread/ScopedLock.h"

#if defined(FX_WINDOWS)
#include "firtex/thread/Mutex_WIN32.h"
#else
#include "firtex/thread/FastMutex_POSIX.h"
#endif

FX_NS_DEF(thread);

/**
 * @class FastMutex
 * 
 * A FastMutex (mutual exclusion) is similar to a Mutex.
 * Unlike a Mutex, however, a FastMutex is not recursive,
 * which means that a deadlock will occur if the same
 * thread tries to lock a mutex it has already locked again.
 * Locking a FastMutex is faster than locking a recursive Mutex.
 * Using the ScopedLock class is the preferred way to automatically
 * lock and unlock a mutex.
 */
class FastMutex: private FX_NS(utility)::NonCopyable
{
public:
    typedef ScopedLock<FastMutex> Guard;

    /**
     * creates the Mutex.
     */
    FastMutex();
    
    /**
     * destroys the Mutex.
     */
    ~FastMutex();

    /**
     * Locks the mutex. Blocks if the mutex
     * is held by another thread.
     */
    void lock();
    
    /**
     * Locks the mutex. 
     * @param milliseconds Number of milliseconds to blocks up if
     *        the mutex is held by another thread. 
     *        
     * @exception Throws a TimeoutException if the mutex can 
     *            not be locked within the given timeout.
     *
     * Performance Note: On most platforms (including Windows), 
     * this member function is implemented using a loop calling 
     * (the equivalent of) tryLock() and Thread::sleep().
     * On POSIX platforms that support pthread_mutex_timedlock(),
     * this is used.
     */
    void lock(long milliseconds);
    
    /**
     * Try to lock the mutex. 
     * @return Return false immediately if the mutex is already 
     *         held by another thread. Returns true if the mutex 
     *         was successfully locked.
     */
    bool tryLock();
    
    /**
     * Locks the mutex. 
     * @param milliseconds Number of milliseconds to blocks up if
     *        the mutex is held by another thread. 
     * @return Return true if the mutex was successfully locked.
     *
     * Performance Note: On most platforms (including Windows),
     * this member function is implemented using a loop calling 
     * (the equivalent of) tryLock() and Thread::sleep().
     * On POSIX platforms that support pthread_mutex_timedlock(),
     * this is used.
     */
    bool tryLock(long milliseconds);
    
    /**
     * Unlocks the mutex so that it can be acquired by
     * other threads.
     */
    void unlock();

private:
    FastMutexImpl* m_pImpl;
};

///////////////////////////////////////////////////////
// inlines
//
inline void FastMutex::lock()
{
    m_pImpl->lock();
}

inline void FastMutex::lock(long milliseconds)
{
    if (!m_pImpl->tryLock(milliseconds))
        FIRTEX_THROW(TimeoutException, "Timeout");
}

inline bool FastMutex::tryLock()
{
    return m_pImpl->tryLock();
}

inline bool FastMutex::tryLock(long milliseconds)
{
    return m_pImpl->tryLock(milliseconds);
}

inline void FastMutex::unlock()
{
    m_pImpl->unlock();
}

FX_NS_END

#endif 
