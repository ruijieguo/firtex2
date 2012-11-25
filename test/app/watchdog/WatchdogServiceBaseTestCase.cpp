#include "WatchdogServiceBaseTestCase.h"
#include "WatchdogServiceBase.h"
#include "firtex/utility/File.h"
#include "TestHelper.h"

using namespace std;
FX_NS_USE(util);
FX_NS_USE(utility);

FX_NS_DEF(app);

SETUP_LOGGER(app, WatchdogServiceBaseTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(WatchdogServiceBaseTestCase);

class WatchdogServiceMock : public WatchdogServiceBase
{
public:
    typedef std::map<std::string, ProcessInfoPtr> ProcMap;
    
public:
    WatchdogServiceMock() 
    {
    }

public:
    virtual FX_NS(util)::ProcessHandlePtr launchProcess(
            const std::string& command, const Args& args,
            const EnvMap& envVariables, const std::string& sWorkDir)
    {
#ifdef FX_WINDOWS
        ProcessHandlePtr pProcHandle = new ProcessHandle(NULL,
                (ProcessHandle::PIDType)m_activeProcesses.size());
#else
        ProcessHandlePtr pProcHandle = new ProcessHandle(
                (ProcessHandle::PIDType)m_activeProcesses.size());
#endif
        ProcessInfoPtr pProcInfo = new ProcessInfo(command, args,
                envVariables, 0, sWorkDir, pProcHandle, false);
        pProcInfo->status = PS_RUNNING;
        m_activeProcesses.insert(make_pair(command, pProcInfo));
        return pProcHandle;
    }

    virtual void killProcess(const FX_NS(util)::ProcessHandle& handle)
    {
        for (ProcMap::iterator it = m_activeProcesses.begin();
             it != m_activeProcesses.end(); ++it)
        {
            if (it->second->procHandle->id() == handle.id())
            {
                m_activeProcesses.erase(it);
                break;
            }
        }
    }

    virtual FX_NS(util)::ProcessStatus getProcessStatus(
            const FX_NS(util)::ProcessHandlePtr& pProcHandle)
    {
        for (ProcMap::iterator it = m_activeProcesses.begin();
             it != m_activeProcesses.end(); ++it)
        {
            if (it->second->procHandle->id() == pProcHandle->id())
            {
                return it->second->status;
            }
        }
        return PS_UNKNOWN;
    }

private:
    ProcMap m_activeProcesses;
};

WatchdogServiceBaseTestCase::WatchdogServiceBaseTestCase() 
{
}

WatchdogServiceBaseTestCase::~WatchdogServiceBaseTestCase() 
{
}

void WatchdogServiceBaseTestCase::setUp()
{
    File f(getTestPath());
    if (File::exists(getTestPath()))
    {
        f.remove(true);
    }
    f.createDirectory();
}

void WatchdogServiceBaseTestCase::tearDown()
{
    if (File::exists(getTestPath()))
    {
        File f(getTestPath());
        f.remove(true);
    }
}

void WatchdogServiceBaseTestCase::testStartProcess()
{
    WatchdogServiceMock wd;
    wd.init(getTestPath());
    
    WatchdogServiceBase::ErrorInfo errorInfo;
    WatchdogServiceBase::ExecutionTask task;
    task.command.processName = "test_process";
    task.command.parameters.push_back("param1");
    task.command.parameters.push_back("param2");
    task.envVariables.insert(make_pair("LD_LIBRARY_PATH", "test_path"));
    task.processInfoKeepTime = 10;
    task.workDirectory = "~/work_dir";
    task.restartAfterCrash = false;
    WatchdogServiceBase::PIDType pid = wd.startProcess(errorInfo, task);
    CPPUNIT_ASSERT(errorInfo.errorCode == WatchdogServiceBase::EC_OK);
    CPPUNIT_ASSERT(pid == 0);

#ifdef FX_WINDOWS
    ProcessHandlePtr pProcHandle = new ProcessHandle(NULL, pid);
#else
    ProcessHandlePtr pProcHandle = new ProcessHandle(pid);
#endif
    CPPUNIT_ASSERT(wd.getProcessStatus(pProcHandle) == PS_RUNNING);
    CPPUNIT_ASSERT(File::exists(getTestPath() + "/process.status"));

    WatchdogServiceBase::ErrorInfo errorInfo2;
    WatchdogServiceBase::Command stopCmd;
    stopCmd.processName = "test_process";
    stopCmd.parameters.push_back("param1");
    stopCmd.parameters.push_back("param2");
    wd.stopProcess(errorInfo2, stopCmd, 10);
    CPPUNIT_ASSERT(errorInfo2.errorCode == WatchdogServiceBase::EC_OK);

    stopCmd.processName = "test_process_not_exits";
    wd.stopProcess(errorInfo2, stopCmd, 10);
    CPPUNIT_ASSERT(errorInfo2.errorCode == WatchdogServiceBase::EC_PROCESS_NOT_FOUND);
}

void WatchdogServiceBaseTestCase::testRecover()
{
    WatchdogServiceMock wd;
    wd.init(getTestPath());

    WatchdogServiceBase::ErrorInfo errorInfo;
    WatchdogServiceBase::ExecutionTask task;
    task.command.processName = "test_process";
    task.command.parameters.push_back("param1");
    task.command.parameters.push_back("param2");
    task.envVariables.insert(make_pair("LD_LIBRARY_PATH", "test_path"));
    task.processInfoKeepTime = 10;
    task.workDirectory = "~/work_dir";
    task.restartAfterCrash = true;
    WatchdogServiceBase::PIDType pid = wd.startProcess(errorInfo, task);
    CPPUNIT_ASSERT(errorInfo.errorCode == WatchdogServiceBase::EC_OK);
    CPPUNIT_ASSERT(pid == 0);

    //recover
    WatchdogServiceMock wdRecover;
    wdRecover.init(getTestPath());
#ifdef FX_WINDOWS
    ProcessHandlePtr pProcHandle = new ProcessHandle(NULL, pid);
#else
    ProcessHandlePtr pProcHandle = new ProcessHandle(pid);
#endif
    CPPUNIT_ASSERT(wdRecover.getProcessStatus(pProcHandle) == PS_RUNNING);
    CPPUNIT_ASSERT(File::exists(getTestPath() + "/process.status"));
}

string WatchdogServiceBaseTestCase::getTestPath()
{
    return TestHelper::getTestDataPath("watchdog_testdir", true);
}


FX_NS_END

