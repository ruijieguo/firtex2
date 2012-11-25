#include "firtex/extension/com/Com.h"
#include <assert.h>
#include "firtex/extension/com/DllServer.h"

#ifdef FX_WINDOWS
#include <windows.h>
#else
#include <pthread.h>
#endif

FX_NS_DEF(com);

class Mutex
{
public:  
    Mutex() 
    {
#ifdef FX_WINDOWS
		::InitializeCriticalSection(&m_lock);
#else // for pthread
      	pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
#if defined(PTHREAD_MUTEX_RECURSIVE_NP)
        pthread_mutexattr_settype_np(&attr, PTHREAD_MUTEX_NORMAL_NP);
#else
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
#endif
        if (pthread_mutex_init(&m_lock, &attr))
        {
            pthread_mutexattr_destroy(&attr);
            assert(false);
        }
        pthread_mutexattr_destroy(&attr);  
#endif
    }

    ~Mutex()
    {
#ifdef FX_WINDOWS
		::DeleteCriticalSection(&m_lock);
#else
        pthread_mutex_destroy(&m_lock);
#endif   
    }

    void lock()
    {
#ifdef FX_WINDOWS
		::EnterCriticalSection(&m_lock);
#else
        pthread_mutex_lock(&m_lock);
#endif
    }

    void unlock()
    {
#ifdef FX_WINDOWS
        ::LeaveCriticalSection(&m_lock);
#else
     	pthread_mutex_unlock(&m_lock);
#endif
    }

private:
#ifdef FX_WINDOWS
    CRITICAL_SECTION m_lock;
#else
    pthread_mutex_t m_lock;
#endif
};

int volatile DllServer::ms_lockCount = 0;

static Mutex COUNT_MUTEX;

FX_HRESULT DllServer::DllCanUnloadNowImpl()
{
	return ( ms_lockCount > 0 ) ? FX_S_FALSE : FX_S_OK;
}

FX_HRESULT DllServer::DllLockServerImpl( int32_t fLock )
{
    COUNT_MUTEX.lock();
	if ( fLock ) 
    {
		++ms_lockCount;
		assert( ms_lockCount != 0 );
	}
	else 
    {
		assert( ms_lockCount != 0 );
		--ms_lockCount;
	}

    COUNT_MUTEX.unlock();

	return FX_S_OK;
}

FX_HRESULT DllServer::DllRegisterServerImpl(FX_REFCLSID clsid, wchar_t const* name, wchar_t const* VersionIndependentProgId, wchar_t const* ProgId)
{
    //TODO:
    return FX_S_OK;
}

FX_HRESULT DllServer::DllUnregisterServerImpl(FX_REFCLSID clsid)
{
    //TODO:
    return FX_S_OK;
}

FX_NS_END
