#include "firtex/extension/util/SignalHandler.h"

#ifdef FX_WINDOWS
#include <windows.h>
#else
#include <signal.h>
#endif

FX_NS_USE(thread);

FX_NS_DEF(util);

SETUP_STREAM_LOGGER(util, SignalHandler);

#ifdef FX_WINDOWS
BOOL WINAPI consoleCtrlHandler(DWORD ctrlType)
{
    switch(ctrlType) 
    {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_SHUTDOWN_EVENT:
//        FX_LOG(INFO, "Receive signal: [%d], exiting...", ctrlType);
        SignalHandler::instance()->shutdown();
        return TRUE;
    default:
        return FALSE;
    }
}
#else
void signalHandler(int sig)
{
    //FX_LOG(INFO, "Receive signal: [%d], exiting...", sig);
    SignalHandler::instance()->shutdown();
}
#endif

SignalHandler::SignalHandler() 
    : m_bShutdownRequested(false)
{
}

SignalHandler::~SignalHandler() 
{
}

void SignalHandler::shutdown()
{
    FastMutex::Guard g(m_mutex);
    m_bShutdownRequested = true;
    m_cond.broadcast();
}

void SignalHandler::wait()
{
    FastMutex::Guard g(m_mutex);
    while (!m_bShutdownRequested)
    {
        m_cond.wait(m_mutex);
    }
}

FX_NS_END

