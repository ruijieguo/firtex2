#include "firtex/extension/util/ServerApplication.h"
#include "firtex/extension/util/SignalHandler.h"

#ifndef FX_WINDOWS
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

FX_NS_DEF(util);

SETUP_STREAM_LOGGER(util, ServerApplication);

#ifdef FX_WINDOWS
    SERVICE_STATUS ServerApplication::m_svcStatus;
	SERVICE_STATUS_HANDLE ServerApplication::m_svcStatusHandle = (SERVICE_STATUS_HANDLE)0;
	std::string ServerApplication::m_sProgNameCopy = "";
	ServerApplication* ServerApplication::m_pAppCopy = NULL;
#endif

ServerApplication::ServerApplication(const std::string& sProgName, 
                                     const std::string& sDesc)
    : Application(sProgName, sDesc)
    , m_bRunAsDaemon(false)
{
    m_pDaemonRunner = new DaemonAppRunner(*this);

    m_pDaemonRunner->addOption();
}

ServerApplication::~ServerApplication() 
{
    delete m_pDaemonRunner;
}

Application::Status ServerApplication::run()
{
    if (m_bRunAsDaemon)
    {
        daemonize();
#ifdef FX_WINDOWS
        return AppRunner::S_RUNNING;
#endif
    }
    setupSignalHandler();

    Application::Status stat = Application::run();

    if (stat == AppRunner::S_RUNNING)
    {
        SignalHandler::instance()->wait();
        Application::stop();
        Application::join();
    }
    return stat;
}

#ifdef FX_WINDOWS

/// serivce control handler - we only support Stop request
void WINAPI serviceControlHandler(DWORD dwCtrl)
{
    // Handle the requested control code. 
    switch(dwCtrl) 
    {  
    case SERVICE_CONTROL_STOP: 
    case SERVICE_CONTROL_SHUTDOWN:
		ServerApplication::reportServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
        // Signal the Pion to shutdown.
        SignalHandler::instance()->shutdown();
        return;

    case SERVICE_CONTROL_INTERROGATE: 
        // Fall through to send current status.
        break; 
        
    default: 
        break;
    } 

	ServerApplication::reportServiceStatus(ServerApplication::m_svcStatus.dwCurrentState, NO_ERROR, 0);
}

void WINAPI SvcMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
    // Register the handler function for the service
	ServerApplication::m_svcStatusHandle = RegisterServiceCtrlHandler(ServerApplication::m_sProgNameCopy.c_str(), serviceControlHandler);
    if(!ServerApplication::m_svcStatusHandle )
    { 
		FIRTEX_THROW(SystemException, "RegisterServiceCtrlHandler() FAILED: [%s]", ServerApplication::WinErrorInfo());        
        return;
    } 

    // These SERVICE_STATUS members remain as set here
    ServerApplication::m_svcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
    ServerApplication::m_svcStatus.dwServiceSpecificExitCode = 0;    

    // Report initial status to the SCM
    ServerApplication::reportServiceStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

	ServerApplication::m_pAppCopy->run();

    ServerApplication::reportServiceStatus(SERVICE_STOPPED, NO_ERROR, 0);
}

#endif

void ServerApplication::daemonize()
{
#ifdef FX_WINDOWS
    if (!isRegistered())
    {
        registerService();
    }

	ServerApplication::m_sProgNameCopy  = getProgName();
	ServerApplication::m_pAppCopy = this;
    SERVICE_TABLE_ENTRY dispatchTable[] = 
        { 
			{ (LPSTR)getProgName().c_str(), (LPSERVICE_MAIN_FUNCTION)SvcMain }, 
            { NULL, NULL } 
        }; 
    if (!StartServiceCtrlDispatcher(dispatchTable))
    { 
		FIRTEX_THROW(SystemException, "StartServiceCtrlDispatcher() FAILED: [%s]", WinErrorInfo());
    }
#else
    /// Exit if already running as a daemon
    if (getppid() == 1)
    {
        return;
    }

    int i = fork();
    if (i < 0)
    {
        FIRTEX_THROW_AND_LOG(SystemException, "Cannot fork daemon process.");
    }
    else if (i > 0) 
    {
        /// exit if parent
        exit(0);
    }

    /// obtain a new process group
    if (setsid() == -1)
    {
        FIRTEX_THROW_AND_LOG(SystemException, "setsid() FAILED.");
    }

    umask(0);
	
    /// Bind stdio to /dev/null
    int fd = open("/dev/null", O_RDWR);
    if (fd == -1)
    {
        FIRTEX_THROW_AND_LOG(SystemException, "open(\"/dev/null\") FAILED.");
    }
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        FIRTEX_THROW_AND_LOG(SystemException, "dup2(STDIN) FAILED.");
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        FIRTEX_THROW_AND_LOG(SystemException, "dup2(STDOUT) FAILED.");
    }

    if (dup2(fd, STDERR_FILENO) == -1) 
    {
        FIRTEX_THROW_AND_LOG(SystemException, "dup2(STDERR) FAILED");
    }

    if (fd > STDERR_FILENO) 
    {
        if (close(fd) == -1) 
        {
            FIRTEX_THROW_AND_LOG(SystemException, "close() FAILED");
        }
    }
#endif
}

void ServerApplication::setupSignalHandler()
{
#ifdef FX_WINDOWS
    SetConsoleCtrlHandler(consoleCtrlHandler, TRUE);
#else
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
#endif
}

#ifdef FX_WINDOWS
void ServerApplication::registerService()
{
    SC_HANDLE scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!scmHandle)
    {
        FIRTEX_THROW(SystemException, "Cannot open Service Control Manager");
    }

    char szBinary[MAX_PATH];
    if ( !GetModuleFileName( NULL, szBinary, MAX_PATH))
    {
        FIRTEX_THROW(SystemException, "GetModuleFileName() FAILED: [%s]", WinErrorInfo());
    }

    SC_HANDLE svcHandle = CreateService(
            scmHandle,                    /// SCM database
            getProgName().c_str(),        /// name of service
            getProgName().c_str(),        /// service name to display
            SERVICE_ALL_ACCESS,           /// desired access
            SERVICE_WIN32_OWN_PROCESS,    /// service type
            SERVICE_DEMAND_START,         /// start type
            SERVICE_ERROR_NORMAL,         /// error control type
            szBinary,                     /// path to service's binary
            NULL,                         /// no load ordering group
            NULL,			  /// no tag identifier
            NULL,                         /// no dependencies
            NULL,			  /// LocalSystem account
            NULL);			  /// no password
    if (!svcHandle)
    {
        CloseServiceHandle(scmHandle);
        FIRTEX_THROW(SystemException, "Cannot register service: [%s]", getProgName().c_str());
    }
    CloseServiceHandle(svcHandle);
    CloseServiceHandle(scmHandle);
}

void ServerApplication::unregisterService()
{
    SC_HANDLE scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!scmHandle)
    {
        FIRTEX_THROW(SystemException, "Cannot open Service Control Manager");
    }
    SC_HANDLE svcHandle = OpenService(scmHandle, getProgName().c_str(), SERVICE_ALL_ACCESS);
    if (svcHandle != 0)
    {
        DeleteService(svcHandle);
        CloseServiceHandle(svcHandle);
    }
    CloseServiceHandle(scmHandle);
}

bool ServerApplication::isRegistered() const
{
    SC_HANDLE scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!scmHandle)
    {
        FIRTEX_THROW(SystemException, "Cannot open Service Control Manager");
    }
    SC_HANDLE svcHandle = OpenService(scmHandle, getProgName().c_str(), SERVICE_ALL_ACCESS);
    bool ret = false;
    if (svcHandle != 0)
    {
        CloseServiceHandle(svcHandle);
        ret = true;
    }

    CloseServiceHandle(scmHandle);
    return ret;
}

void ServerApplication::reportServiceStatus(DWORD dwCurrentState,
        DWORD dwWin32ExitCode, DWORD dwWaitHint)
{
    static DWORD dwCheckPoint = 1;

    // Fill in the SERVICE_STATUS structure.
    m_svcStatus.dwCurrentState = dwCurrentState;
    m_svcStatus.dwWin32ExitCode = dwWin32ExitCode;
    m_svcStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
    {
        m_svcStatus.dwControlsAccepted = 0;
    }
    else 
    {
        m_svcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    }

    if ((dwCurrentState == SERVICE_RUNNING) || (dwCurrentState == SERVICE_STOPPED))
    {
        m_svcStatus.dwCheckPoint = 0;
    }
    else 
    {
        m_svcStatus.dwCheckPoint = dwCheckPoint++;
    }

    // Report the status of the service to the SCM.
    SetServiceStatus(m_svcStatusHandle, &m_svcStatus);
}

const char* ServerApplication::WinErrorInfo()
{
    static char sBuf[1024];
    
    DWORD uErr = ::GetLastError ();
    SNPRINTF(sBuf, sizeof(sBuf), "code=%d, error=", uErr);

    size_t nLen = strlen(sBuf);
    if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
                       uErr, 0, sBuf + nLen, (DWORD)(sizeof(sBuf) - nLen), NULL))
    {
        SNPRINTF(sBuf + nLen, sizeof(sBuf) - nLen, "(no message)");
    }
    return sBuf;
}

#endif

FX_NS_END

