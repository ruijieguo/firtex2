#ifndef __MUTEX_WIN32_H
#define __MUTEX_WIN32_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include <windows.h>

FX_NS_DEF(thread)

class MutexImpl
{
protected:
	MutexImpl();
	~MutexImpl();
	void lock();
	bool tryLock();
	bool tryLock(long milliseconds);
	void unlock();

    friend class Mutex;	
    friend class FastMutex;	
private:
	CRITICAL_SECTION m_cs;
};

typedef MutexImpl FastMutexImpl;

//////////////////////////////////////////
// inlines
//
inline void MutexImpl::lock()
{
	try
	{
		::EnterCriticalSection(&m_cs);
	}
	catch (...)
	{
		FIRTEX_THROW(SystemException, "cannot lock mutex");
	}
}


inline bool MutexImpl::tryLock()
{
	try
	{
#if(_WIN32_WINNT >= 0x0400)
		return ::TryEnterCriticalSection(&m_cs) == TRUE;
#else
        return true;
#endif
	}
	catch (...)
	{
        FIRTEX_THROW(SystemException, "cannot lock mutex");
	}
}


inline void MutexImpl::unlock()
{
	::LeaveCriticalSection(&m_cs);
}

FX_NS_END

#endif 
