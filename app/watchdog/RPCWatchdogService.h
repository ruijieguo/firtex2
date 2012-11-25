#ifndef __FX_PRCDAEMONSERVICE_H
#define __FX_PRCDAEMONSERVICE_H

#include <list>
#include <map>
#include "WatchdogServiceBase.h"
#include "../thrift/gen-cpp/ThriftWatchdogService.h"

FX_NS_USE(util);
FX_NS_DEF(app);

class RPCWatchdogService : public ThriftWatchdogServiceIf, 
                           public WatchdogServiceBase
{
public:
    typedef WatchdogServiceBase::PIDType PIDType;
    typedef WatchdogServiceBase::Args Args;
    typedef WatchdogServiceBase::EnvMap EnvMap;
    typedef WatchdogServiceBase::ProcessInfo ProcessInfo;
    typedef WatchdogServiceBase::ProcessInfoPtr ProcessInfoPtr;
    typedef WatchdogServiceBase::ProcessMap ProcessMap;

public:
    RPCWatchdogService();
    ~RPCWatchdogService();

public:
    /**
     * Implementation of  rpc service method. 
     * Execute a command
     *
     * @param _return result of execute task
     * @param task the execution task 
     */
    void startProcess(WatchdogExecutionResult& _return, 
                      const WatchdogExecutionTask& task);

    /**
     * Implementation of rpc service method. 
     * Stop a process 
     * @param _return result of stop operation
     * @param cmd the command of process
     * @param stopSignal signal to send
     */
    void stopProcess(WatchdogErrorInfo& _return, const WatchdogCommand& cmd,
                     const int32_t stopSignal);

    /**
     * Implementation of  rpc service method. 
     * Get specified process status 
     * 
     * @param _return process result
     * @param cmd the command of process
     */    
    void getProcessStatus(WatchdogProcessStatus& _return,
                          const WatchdogCommand& cmd);

    /**
     * Clear all active processes
     */
    void clear();

    /// Implement ping service
    bool ping();

private:
    void thriftTaskToBaseTask(ExecutionTask& task, 
                              const WatchdogExecutionTask& thriftTask);
    void baseErrorCodeToThriftErrorCode(WatchdogErrorInfo& errorInfo,
            const ErrorInfo& baseErrorInfo);

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(RPCWatchdogService);

//////////////////////////////////////////
//

FX_NS_END

#endif //__FX_PRCDAEMONSERVICE_H
