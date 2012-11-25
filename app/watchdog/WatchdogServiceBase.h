//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2012-03-21 23:56:11

#ifndef __FX_WATCHDOGSERVICEBASE_H
#define __FX_WATCHDOGSERVICEBASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/FastMutex.h"
#include "firtex/thread/Thread.h"
#include "firtex/extension/util/Process.h"
#include "firtex/utility/XMLDocumentWrapper.h"

FX_NS_DEF(app);

class WatchdogServiceBase : public FX_NS(thread)::Runnable
{
public:
    const static size_t DEFAULT_LOOP_INTERVAL = 200; //in msec

public:
    typedef FX_NS(util)::Process::PIDType PIDType;

    typedef std::vector<std::string> Args;
    typedef std::map<std::string, std::string> EnvMap;

    enum ErrorCode 
    {
        EC_OK = 0,
        EC_PROCESS_EXIST = 1,
        EC_PROCESS_NOT_FOUND = 2,
        EC_NO_PERMISSION = 3,
        EC_SERIALIZE_FAIL = 4,
        EC_SYSTEM_ERROR = 5,
    };

    struct ErrorInfo
    {
        ErrorCode errorCode;
        std::string errorMsg;
    };

    struct Command 
    {
        std::string processName;
        std::vector<std::string> parameters;
    };

    struct ExecutionTask 
    {
        ExecutionTask() : processInfoKeepTime(0), restartAfterCrash(false) {}
        Command command;
        EnvMap envVariables;
        int64_t processInfoKeepTime;
        std::string workDirectory;
        bool restartAfterCrash;
    };

    struct ProcessInfo
    {
        ProcessInfo()
            : processInfoKeepTime(0)
            , status(FX_NS(util)::PS_UNKNOWN)
            , restartCount(0)
            , lastCrashTime(0)
            , stopSignal(0)
            , restartAfterCrash(false)
        {
        }

        ProcessInfo(const std::string& sProcName, const Args& args,
                    const EnvMap& evnV, int64_t nKeepTime, 
                    const std::string& workDir, 
                    const FX_NS(util)::ProcessHandlePtr& p,
                    bool r) 
            : processName(sProcName)
            , parameters(args)
            , envVariables(evnV)
            , processInfoKeepTime(nKeepTime)
            , workDirectory(workDir)
            , procHandle(p)
            , status(FX_NS(util)::PS_UNKNOWN)
            , restartCount(0)
            , launchTime(time(NULL))
            , exitTime(0)
            , lastCrashTime(0)
            , stopSignal(0)
            , restartAfterCrash(r)
        {
        }

        std::string processName;
        Args parameters;
        EnvMap envVariables;
        int64_t processInfoKeepTime;
        std::string workDirectory;
        FX_NS(util)::ProcessHandlePtr procHandle;
        FX_NS(util)::ProcessStatus status;
        int32_t restartCount;
        time_t launchTime;
        time_t exitTime;
        time_t lastCrashTime;
        int32_t stopSignal;
        bool restartAfterCrash;
    };

    DEFINE_TYPED_PTR(ProcessInfo);

    typedef std::map<std::string, ProcessInfoPtr> ProcessMap;

public:
    WatchdogServiceBase();
    virtual ~WatchdogServiceBase();

public:
    ///Initialize 
    void init(const std::string& sWorkDir);

    /**
     * Execute a command
     *
     * @param errorInfo error information of execute task
     * @param task the execution task 
     * @return process id
     */
    PIDType startProcess(ErrorInfo& errorInfo, const ExecutionTask& task);

    /**
     * Implementation of rpc service method. 
     * Stop a process 
     * @param errorInfo error information of execute task
     * @param cmd the command of process
     * @param stopSignal signal to send
     */
    void stopProcess(ErrorInfo& errorInfo, const Command& cmd, 
                     int32_t stopSignal);

    /**
     * Get specified process status 
     * 
     * @param errorInfo error information of execute task
     * @param cmd the command of process
     * @return process info
     */    
    ProcessInfoPtr getProcessInfo(ErrorInfo& errorInfo, const Command& cmd);

    /**
     * Clear all active processes
     */
    void clear();

public:
    virtual FX_NS(util)::ProcessHandlePtr launchProcess(
            const std::string& command, const Args& args,
            const EnvMap& envVariables, const std::string& sWorkDir);
    virtual void killProcess(const FX_NS(util)::ProcessHandle& handle);
    virtual FX_NS(util)::ProcessStatus getProcessStatus(
            const FX_NS(util)::ProcessHandlePtr& pProcHandle);

protected:
    std::string encodeProcessName(
            const std::string& sProcName, 
            const std::vector<std::string>& params);
    void restartProcess(ProcessInfoPtr& pProcInfo);
    void procInfoToXML(FX_NS(utility)::XMLNodeWrapperPtr& pRootNode, 
                       const ProcessInfoPtr& pProcInfo);

    bool serializeStatusToFile();
    bool deserializeStatusFromFile();
    void run();
    void runOnce();

    inline std::string encodeParameters(
            const std::vector<std::string>& params);

private:
    FX_NS(thread)::ThreadPtr m_pMonitorThread;

    FX_NS(thread)::FastMutex m_mutex;
    ProcessMap m_processes;
    std::string m_sWorkDir;
    std::string m_sDefaultProcessWorkDir;
    std::string m_sStatusFile;
    
    volatile bool m_bStop;

private:
    DECLARE_LOGGER();
};

/////////////////////////////////////////////
inline std::string WatchdogServiceBase::encodeParameters(
        const std::vector<std::string>& params)
{
    std::ostringstream oss;
    for (size_t i = 0; i < params.size(); ++i)
    {
        oss << " " << params[i];
    }
    return oss.str();
}

FX_NS_END

#endif //__FX_WATCHDOGSERVICEBASE_H
