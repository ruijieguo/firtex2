#include "firtex/thread/RWLock_POSIX.h"
#include "firtex/utility/Exception.h"

FX_NS_USE(utility);
FX_NS_DEF(thread);


RWLockImpl::RWLockImpl()
{
    int ret = pthread_rwlock_init(&m_rwLock, NULL);
    if (ret)
    {
        handleError(ret);
    }
}


RWLockImpl::~RWLockImpl()
{
    pthread_rwlock_destroy(&m_rwLock);
}

void RWLockImpl::handleError(int ec)
{
    if (ec == EINVAL)
    {
        FIRTEX_THROW(InitializationException, "rwlock object not initialized");
    }
    else if (ec == EPERM)
    {
        FIRTEX_THROW(InitializationException,
                     "The current thread does not hold a lock on the rwlock.");
    }
    else
    {
        FIRTEX_THROW(InitializationException, "Unknown error.");
    }
}

FX_NS_END
