#ifndef __FX_MUTEX_H
#define __FX_MUTEX_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/NonCopyable.h"
#include "firtex/thread/ScopedLock.h"


#if defined(FX_WINDOWS)
#include "firtex/thread/Mutex_WIN32.h"
#else
#include "firtex/thread/Mutex_POSIX.h"
#endif

FX_NS_DEF(thread);

/**
 * @class Mutex
 *
 * A Mutex is used to provide serialized (one thread at a time) 
 * access to some portion of code. Mutexes are recursive, 
 * that is, the same mutex can be locked multiple times by 
 * the same thread (but, of course, not by other threads).
 *
 * Using the ScopedLock class is the preferred way to automatically
 * lock and unlock a mutex.
 *
 * <b>Error Checking</b>
 *
 * A Mutex will throw a DeadlockException if an attempt to acquire a Mutex more 
 * than once is made from the context of the same thread.
 *
 * A Mutex will throw an InvalidOpException if an attempt to release a Mutex is 
 * made from the context of a thread that does not currently own that Mutex.
 */
class Mutex : private FX_NS(utility)::NonCopyable
{
public:
    typedef ScopedLock<Mutex> Guard;
  
    /** creates the Mutex. */
    Mutex();
    
    /** destroys the Mutex.*/
    ~Mutex();

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
     * Tries to lock the mutex. 
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
    MutexImpl* m_pImpl;
};

///////////////////////////////////////////////////////
// inlines
//

inline void Mutex::lock()
{
    m_pImpl->lock();
}

inline void Mutex::lock(long milliseconds)
{
    if (!m_pImpl->tryLock(milliseconds))
        FIRTEX_THROW(TimeoutException, "Lock timeout");
}

inline bool Mutex::tryLock()
{
    return m_pImpl->tryLock();
}

inline bool Mutex::tryLock(long milliseconds)
{
    return m_pImpl->tryLock(milliseconds);
}

inline void Mutex::unlock()
{
    m_pImpl->unlock();
}

FX_NS_END

#endif 
