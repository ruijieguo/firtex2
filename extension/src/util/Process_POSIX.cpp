#include "firtex/extension/util/Process_POSIX.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/Environment.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>

using namespace std;
FX_NS_USE(utility);

FX_NS_DEF(util);

SETUP_LOGGER(util, Process);

std::string Process::STDOUT_FILE_NAME = "stdout";
std::string Process::STDERR_FILE_NAME = "stderr";

Process::Process() 
{
}

Process::~Process() 
{
}

//static 
Process::PIDType Process::id()
{
    return getpid();
}

//static 
void Process::times(long& userTime, long& kernelTime)
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    userTime = usage.ru_utime.tv_sec;
    kernelTime = usage.ru_stime.tv_sec;
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

    int pid = fork();
    if (pid < 0)
    {
        FIRTEX_THROW(SystemException, "Cannot fork process for: [%s]", 
                     command.c_str());
    }

    //fill arguments
    char** argv = new char*[args.size() + 2];
    int i = 0;
    argv[i++] = const_cast<char*>(command.c_str());
    for (Args::const_iterator it = args.begin(); it != args.end(); ++it)
    {
        argv[i++] = const_cast<char*>(it->c_str());
    }
    argv[i] = NULL;

    if (pid == 0) //the child process
    {
        for (EnvMap::const_iterator it = envVariables.begin();
             it != envVariables.end(); ++it)
        {
            Environment::set(it->first, it->second);
        }
        if (!sWorkDir.empty())
        {
            if (chdir(sWorkDir.c_str()) == -1)
            {
                FIRTEX_THROW(RuntimeException, "chdir(%s) FAILED",
                        sWorkDir.c_str());
            }
        }

        // close all open file descriptors other than stdin, stdout, stderr
        for (int i = 3; i < getdtablesize(); ++i)
        {
            close(i);
        }
        
        stringstream ss;
        ss << "." << getpid();
        Path path(sWorkDir);
        path.makeDirectory();

        path.setFileName(STDOUT_FILE_NAME + ss.str());
        if (freopen(path.toString().c_str(), "w+", stdout) == NULL)
        {
            FX_LOG(ERROR, "Reopen [%s] FAILED", path.toString().c_str());
        }

        path.setFileName(STDERR_FILE_NAME + ss.str());
        if (freopen(path.toString().c_str(), "w+", stderr) == NULL)
        {
            FX_LOG(ERROR, "Reopen [%s] FAILED", path.toString().c_str());
        }
        execvp(command.c_str(), argv);

        fprintf(stderr, "command: [%s] execvp FAILED, error code: %d(%s)",
                command.c_str(), errno, strerror(errno));
        fflush(stderr);
        _exit(72);
    }
    else 
    {
        delete[] argv;
        //parent process
        return ProcessHandlePtr(new ProcessHandle(pid));
    }

    //never get here
    return ProcessHandlePtr();
}

//static 
ProcessHandlePtr Process::launch(const std::string& command, const Args& args)
{
    EnvMap emptyEnv;
    return launch(command, args, emptyEnv, "");
}

//static 
int Process::wait(const ProcessHandle& handle)
{
    return handle.wait();
}

//static 
void Process::kill(const ProcessHandle& handle)
{
    kill(handle.id());
}

//static 
void Process::kill(PIDType pid)
{
    if (::kill(pid, SIGKILL) != 0)
    {
        switch (errno)
        {
        case ESRCH:
            FIRTEX_THROW(NotFoundException, "cannot kill process");
        case EPERM:
            FIRTEX_THROW(NoPermissionException, "cannot kill process");
        default:
            FIRTEX_THROW(SystemException, "cannot kill process");
        }
    }
}

ProcessStatus Process::getProcessStatus(
        const ProcessHandlePtr& pProcHandle)
{
    int status;
    int option = WNOHANG;
    errno = 0;
    pid_t retPid = waitpid(pProcHandle->id(), &status, option);
    int waitError = errno;
    FX_DEBUG("waitpid: [%d], return: [%d]", pProcHandle->id(), retPid);
    if (retPid > 0) 
    {
        FX_LOG(INFO, "Process exit status: [%d]", status);
        //has waited a child process
        if (WIFEXITED(status) && (WEXITSTATUS(status) == 0))
        {
            return PS_EXIT_NORMAL;
        } 
        else 
        {
            if (WIFEXITED(status)) 
            {
                FX_LOG(INFO, "Process [%d] exit(%d)", retPid, 
                       WEXITSTATUS(status));
            }
            else if (WIFSIGNALED(status)) 
            {
                FX_LOG(INFO, "Process [%d] killed by signal [%d]", 
                       retPid, WTERMSIG(status));
            }
            else if (WIFSTOPPED(status)) 
            {
                FX_LOG(INFO, "Process [%d] stopped by signal [%d]",
                       retPid, WSTOPSIG(status));
            }
            else if (WTERMSIG(status)) 
            {
                FX_LOG(INFO, "Process [%d] continued", retPid);
            }

            return PS_EXIT_WITH_ERROR;
        }
    } 
    else if (retPid < 0) 
    {
        FX_LOG(INFO, "waitpid return -1, errno: [%d], errmsg: [%s]",
               waitError, strerror(waitError));
        return PS_EXIT_WITH_ERROR;
        // if (waitError == ECHILD) 
        // {
        //     // not my child, but exit with no error
        //     return PS_EXIT_NORMAL;
        // } 
        // else 
        // {
        //     return PS_EXIT_WITH_ERROR;
        // }
    }

    FX_DEBUG("waitpid return 0, errno: [%d], message: [%s]",
             waitError, strerror(waitError));
    
    return PS_RUNNING;
}

FX_NS_END

