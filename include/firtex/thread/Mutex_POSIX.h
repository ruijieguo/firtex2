#ifndef __MUTEX_POSIX_H
#define __MUTEX_POSIX_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include <pthread.h>
#include <errno.h>

FX_NS_DEF(thread);

class MutexImpl
{
public:
	MutexImpl();
	MutexImpl(bool fast);
	~MutexImpl();
	void lock();
	bool tryLock();
	bool tryLock(long milliseconds);
	void unlock();

    friend class Mutex;	
private:
	pthread_mutex_t _mutex;
};

////////////////////////////////////////////////////
// inlines
//
inline void MutexImpl::lock()
{
	if (pthread_mutex_lock(&_mutex)) 
    {
		FIRTEX_THROW(SystemException, "Cannot lock mutex");
    }
}


inline bool MutexImpl::tryLock()
{
	int rc = pthread_mutex_trylock(&_mutex);
	if (rc == 0)
		return true;
	else if (rc == EBUSY)
		return false;
	else
    {
		FIRTEX_THROW(SystemException, "cannot lock mutex");
    }

}


inline void MutexImpl::unlock()
{
	if (pthread_mutex_unlock(&_mutex))
		FIRTEX_THROW(SystemException, "cannot unlock mutex");
}

FX_NS_END

#endif 
