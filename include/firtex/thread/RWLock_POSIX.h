#ifndef __RWLOCK_POSIX_H
#define __RWLOCK_POSIX_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include <pthread.h>
#include <errno.h>

FX_NS_DEF(thread);

class RWLockImpl
{
protected:
    RWLockImpl();
    ~RWLockImpl();
    void readLock();
    bool tryReadLock();
    void writeLock();
    bool tryWriteLock();
    void unlock();
	
    friend class RWLock;

private:
    void handleError(int ec);

private:
    pthread_rwlock_t m_rwLock;
};


/////////////////////////////////////////////////
// inlines
//
inline void RWLockImpl::readLock()
{
    int rc = pthread_rwlock_rdlock(&m_rwLock);
    if (rc) 
    {
        handleError(rc);
    }
}

inline bool RWLockImpl::tryReadLock()
{
    int rc = pthread_rwlock_tryrdlock(&m_rwLock);
    if (rc == 0)
        return true;
    else if (rc == EBUSY)
        return false;
    else
    {
        handleError(rc);
    }

}

inline void RWLockImpl::writeLock()
{
    if (pthread_rwlock_wrlock(&m_rwLock)) 
        FIRTEX_THROW(InitializationException, "cannot lock reader/writer lock");
}

inline bool RWLockImpl::tryWriteLock()
{
    int rc = pthread_rwlock_trywrlock(&m_rwLock);
    if (rc == 0)
        return true;
    else if (rc == EBUSY)
        return false;
    else
    {
        handleError(rc);
    }

}

inline void RWLockImpl::unlock()
{
    int ret = pthread_rwlock_unlock(&m_rwLock);
    if (ret)
    {
        handleError(ret);
    }
}

FX_NS_END

#endif
