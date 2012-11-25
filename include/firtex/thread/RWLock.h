#ifndef __RWLOCK_H
#define __RWLOCK_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/NonCopyable.h"

#if defined(FX_WINDOWS)
#include "RWLock_WIN32.h"
#else
#include "RWLock_POSIX.h"
#endif

FX_NS_DEF(thread);

class ScopedRWLock;

/**
 * @class RWLock
 * 
 * @author Ruijie Guo
 * @date <2009-05-16>
 * @version 1.2.0
 *
 * A RWLock provides a set of coordinated Lockable objects that can be used to
 * guard an object; One for read-only access, and another for read-write access.
 *
 */  

class RWLock : public FX_NS(utility)::NonCopyable 
{
public:
    typedef ScopedRWLock ScopedLock;

    /**
     * Create a Read/Writer lock.
     * @exception InitializationException thrown if resources could not be 
     *            allocated for this object.
     */
    RWLock();

    /** Destroys the Reader/Writer lock.*/
    ~RWLock();

public:
    /*
     * Acquires read lock. If another thread currently holds a write lock,
     * waits until the write lock is released.
     */
    void readLock();

    /**
     * Try to acquire read lock. 
     * @return Immediately returns true if successful, or
     *         false if another thread currently holds a write lock.
     */
    bool tryReadLock();
    
    /** Acquire write lock. If one or more other threads currently hold 
     * locks, waits until all locks are released. The results are undefined
     * if the same thread already holds a read or write lock
     */
    void writeLock();
    
    /** Try to acquire a write lock. 
     * @return Immediately returns true if successful,
     *         or false if one or more other threads currently hold 
     *         locks. The result is undefined if the same thread already
     *         holds a read or write lock.
     */
    bool tryWriteLock();
    
    /** Releases the read or write lock.*/
    void unlock();
private:
    RWLockImpl* m_pImpl;
};

/**
 * @class ScopedRWLock
 * A variant of ScopedLock for reader/writer locks.
 */
class ScopedRWLock
{
public:
    ScopedRWLock(RWLock& rwl, bool write = false);
    ~ScopedRWLock();

private:
    RWLock& m_rwLock;

    ScopedRWLock();
    ScopedRWLock(const ScopedRWLock&);
    ScopedRWLock& operator = (const ScopedRWLock&);
};

///////////////////////////////////////////////////
// inlines

//////////////////////////////////////////////////
// RWLock
inline void RWLock::readLock()
{
    m_pImpl->readLock();
}

inline bool RWLock::tryReadLock()
{
    return m_pImpl->tryReadLock();
}

inline void RWLock::writeLock()
{
    m_pImpl->writeLock();
}

inline bool RWLock::tryWriteLock()
{
    return m_pImpl->tryWriteLock();
}

inline void RWLock::unlock()
{
    m_pImpl->unlock();
}

//////////////////////////////////////////////////
///ScopedRWLock
inline ScopedRWLock::ScopedRWLock(RWLock& rwl, bool write): m_rwLock(rwl)
{
    if (write)
        m_rwLock.writeLock();
    else
        m_rwLock.readLock();
}

inline ScopedRWLock::~ScopedRWLock()
{
    m_rwLock.unlock();
}

FX_NS_END

#endif


