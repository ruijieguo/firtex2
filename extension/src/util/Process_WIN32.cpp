#include "firtex/extension/util/Process_WIN32.h"
#include "firtex/utility/File.h"

using namespace std;
FX_NS_USE(utility);
FX_NS_DEF(util);

SETUP_LOGGER(util, Process);
SETUP_LOGGER(util, ProcessHandle);

std::string Process::STDOUT_FILE_NAME = "stdout";
std::string Process::STDERR_FILE_NAME = "stderr";

Process::Process() 
{
}

Process::~Process() 
{
}

Process::PIDType Process::id()
{
    return (Process::PIDType)GetCurrentProcessId(); 
}


void Process::times(long& userTime, long& kernelTime)
{
    FILETIME ftCreation;
    FILETIME ftExit;
    FILETIME ftKernel;
    FILETIME ftUser;

    if (GetProcessTimes(GetCurrentProcess(), &ftCreation, &ftExit, &ftKernel, &ftUser) != 0)
    {
        ULARGE_INTEGER time;
        time.LowPart  = ftKernel.dwLowDateTime;
        time.HighPart = ftKernel.dwHighDateTime;
        kernelTime    = long(time.QuadPart/10000000L);
        time.LowPart  = ftUser.dwLowDateTime;
        time.HighPart = ftUser.dwHighDateTime;
        userTime      = long(time.QuadPart/10000000L);
    }
    else
    {
        userTime = kernelTime = -1;
    }
}

//static 
ProcessHandlePtr Process::launch(const std::string& command, const Args& args,
                                 const EnvMap& envVariables,
                                 const std::string& sWorkDir)
{
    if (!sWorkDir.empty())
    {
        File f(sWorkDir);
        if (!f.createDirectories())
        {
            FIRTEX_THROW(SystemException, "Cannot create work "
                         "directory for: [%s]", command.c_str());
        }
    }

    std::string commandLine = command;
    for (Args::const_iterator it = args.begin(); it != args.end(); ++it)
    {
        commandLine.append(" ");
        commandLine.append(*it);
    }
    
    //TODO: redirect stderr/stdout

    STARTUPINFO startupInfo;
    GetStartupInfo(&startupInfo); // take defaults from current process
    startupInfo.cb          = sizeof(STARTUPINFO);
    startupInfo.lpReserved  = NULL;
    startupInfo.lpDesktop   = NULL;
    startupInfo.lpTitle     = NULL;
    startupInfo.dwFlags     = STARTF_FORCEOFFFEEDBACK | STARTF_USESTDHANDLES;
    startupInfo.cbReserved2 = 0;
    startupInfo.lpReserved2 = NULL;
	
    HANDLE hProc = GetCurrentProcess();
    DuplicateHandle(hProc, GetStdHandle(STD_INPUT_HANDLE), hProc,
                    &startupInfo.hStdInput, 0, TRUE, DUPLICATE_SAME_ACCESS);
    DuplicateHandle(hProc, GetStdHandle(STD_OUTPUT_HANDLE), hProc,
                    &startupInfo.hStdOutput, 0, TRUE, DUPLICATE_SAME_ACCESS);
    DuplicateHandle(hProc, GetStdHandle(STD_ERROR_HANDLE), hProc, 
                    &startupInfo.hStdError, 0, TRUE, DUPLICATE_SAME_ACCESS);

    string sEnvVar;
    for (EnvMap::const_iterator it = envVariables.begin();
         it != envVariables.end(); ++it)
    {
        sEnvVar.append(it->first);
        sEnvVar.append("=");
        sEnvVar.append(it->second);
        sEnvVar.append(1, (char)'\0');
    }
    
    PROCESS_INFORMATION processInfo;
    BOOL rc = CreateProcessA(
            NULL, 
            const_cast<char*>(commandLine.c_str()), 
            NULL, 
            NULL, 
            TRUE, 
            0, 
            sEnvVar.empty() ? NULL : (LPVOID)sEnvVar.data(), 
            sWorkDir.empty() ? NULL : (LPCTSTR)sWorkDir.c_str(), 
            &startupInfo, 
            &processInfo);
    CloseHandle(startupInfo.hStdInput);
    CloseHandle(startupInfo.hStdOutput);
    CloseHandle(startupInfo.hStdError);
    if (rc)
    {
        CloseHandle(processInfo.hThread);
        return ProcessHandlePtr(new ProcessHandle(processInfo.hProcess,
                        processInfo.dwProcessId));
    }
    else
    {
        FIRTEX_THROW(SystemException, "Cannot launch process: [%s]", 
                     command.c_str());
    }
    return ProcessHandlePtr();
}

ProcessHandlePtr Process::launch(const std::string& command, const Args& args)
{
    EnvMap emptyEvn;
    return launch(command, args, emptyEvn, "");
}

void Process::kill(const ProcessHandle& handle)
{
    if (TerminateProcess(handle.process(), 0) == 0)
    {
        CloseHandle(handle.process());
        FIRTEX_THROW(SystemException, "cannot kill process");
    }
    CloseHandle(handle.process());
}

void Process::kill(PIDType pid)
{
    HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProc)
    {
        if (TerminateProcess(hProc, 0) == 0)
        {
            CloseHandle(hProc);
            FIRTEX_THROW(SystemException, "cannot kill process");
        }
        CloseHandle(hProc);
    }
    else
    {
        switch (GetLastError())
        {
        case ERROR_ACCESS_DENIED:
            FIRTEX_THROW(NoPermissionException, "cannot kill process");
        case ERROR_NOT_FOUND: 
            FIRTEX_THROW(NotFoundException, "cannot kill process");
        default:
            FIRTEX_THROW(SystemException, "cannot kill process");
        }
    }
}

//static 
ProcessStatus Process::getProcessStatus(const ProcessHandlePtr& pProcHandle)
{
    DWORD nExitCode = 0;
    BOOL ret = GetExitCodeProcess(pProcHandle->process(), &nExitCode);
    if (!ret)
    {
        FX_LOG(ERROR, "Get exit code for process: [%u] FAILED, error: [%d] ",
               pProcHandle->id(), GetLastError());
        return PS_UNKNOWN;
    }

    if (STILL_ACTIVE == nExitCode)
    {
        return PS_RUNNING;
    }
    else if(0 == nExitCode)
    {
        return PS_EXIT_NORMAL;
    }
    else
    {
        return PS_EXIT_WITH_ERROR;
    }
}

FX_NS_END

