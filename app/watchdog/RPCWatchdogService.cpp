#include "RPCWatchdogService.h"

using namespace std;
FX_NS_USE(thread);
FX_NS_USE(util);
FX_NS_USE(utility);


FX_NS_DEF(app);

SETUP_LOGGER(app, RPCWatchdogService);

RPCWatchdogService::RPCWatchdogService()
{
}

RPCWatchdogService::~RPCWatchdogService()
{
}

void RPCWatchdogService::startProcess(WatchdogExecutionResult& _return, 
                                      const WatchdogExecutionTask& task)
{
    ExecutionTask task2;
    thriftTaskToBaseTask(task2, task);
    
    ErrorInfo errorInfo;
    WatchdogServiceBase::PIDType pid = WatchdogServiceBase::startProcess(
            errorInfo, task2);
    if (errorInfo.errorCode == WatchdogServiceBase::EC_OK)
    {
        _return.errorInfo.errorCode = WatchdogErrorCode::WATCHDOG_EC_OK;
        _return.pid = pid;
    }
    else
    {
        baseErrorCodeToThriftErrorCode(_return.errorInfo, errorInfo);
    }
}

void RPCWatchdogService::stopProcess(WatchdogErrorInfo& _return, 
                                     const WatchdogCommand& cmd,
                                     const int32_t stopSignal)
{
    WatchdogServiceBase::Command cmd2;
    cmd2.processName = cmd.processName;
    cmd2.parameters = cmd.parameters;

    ErrorInfo errorInfo;
    WatchdogServiceBase::stopProcess(errorInfo, cmd2, stopSignal);
    baseErrorCodeToThriftErrorCode(_return, errorInfo);
}

void RPCWatchdogService::getProcessStatus(WatchdogProcessStatus& _return,
        const WatchdogCommand& cmd)
{
    WatchdogServiceBase::Command cmd2;
    cmd2.processName = cmd.processName;
    cmd2.parameters = cmd.parameters;

    ErrorInfo errorInfo;
    ProcessInfoPtr pProcInfo = WatchdogServiceBase::getProcessInfo(
            errorInfo, cmd2);
    if (errorInfo.errorCode == WatchdogServiceBase::EC_OK)
    {
        _return.pid = pProcInfo->procHandle->id();
        _return.status = (int32_t)pProcInfo->status;
        _return.restartCount = pProcInfo->restartCount;
        _return.lastCrashTime = pProcInfo->lastCrashTime;
        _return.command = cmd;
        _return.stopSignal = pProcInfo->stopSignal;
        _return.errorInfo.errorCode = WatchdogErrorCode::WATCHDOG_EC_OK;
    }
    else
    {
        baseErrorCodeToThriftErrorCode(_return.errorInfo, errorInfo);
    }
}

void RPCWatchdogService::clear()
{
    WatchdogServiceBase::clear();
}

void RPCWatchdogService::thriftTaskToBaseTask(ExecutionTask& task, 
        const WatchdogExecutionTask& thriftTask)
{
    task.command.processName = thriftTask.command.processName;
    task.envVariables = thriftTask.envVariables;
    task.processInfoKeepTime = thriftTask.processInfoKeepTime;
    task.workDirectory = thriftTask.workDirectory;
    task.restartAfterCrash = thriftTask.restartAfterCrash;
}

void RPCWatchdogService::baseErrorCodeToThriftErrorCode(
        WatchdogErrorInfo& errorInfo, const ErrorInfo& baseErrorInfo)
{
    errorInfo.errorMsg = baseErrorInfo.errorMsg;
    errorInfo.errorCode = (WatchdogErrorCode::type)baseErrorInfo .errorCode;
}

bool RPCWatchdogService::ping()
{
    return true;
}

FX_NS_END
