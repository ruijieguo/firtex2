#include "firtex/extension/network/EvThreadInitializer.h"
#include <event2/thread.h>

#ifdef FX_WINDOWS
#include <winsock2.h>
#endif

FX_NS_DEF(network);

SETUP_LOGGER(network, EvThreadInitializer);

EvThreadInitializer::EvThreadInitializer() 
{
    initThreads();
}

EvThreadInitializer::~EvThreadInitializer() 
{
#ifdef FX_WINDOWS
	WSACleanup();
#endif
}

void EvThreadInitializer::initThreads()
{
#ifdef FX_WINDOWS
    if (evthread_use_windows_threads() < 0)
    {
        FX_LOG(WARN, "evthread_use_pthreads FAILED");
    }

/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;

    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) 
	{
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        FIRTEX_THROW(SystemException, "WSAStartup FAILED");        
    }
#else
    if (evthread_use_pthreads() < 0)
    {
        FX_LOG(WARN, "evthread_use_pthreads FAILED");
    }
#endif
}

FX_NS_END

