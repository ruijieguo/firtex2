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
// Created : 2012-04-03 19:53:41

#ifndef __FX_HTTPWATCHDOGSERVICE_H
#define __FX_HTTPWATCHDOGSERVICE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/network/EvHttpService.h"
#include "WatchdogServiceBase.h"

FX_NS_DEF(app);

class HTTPWatchdogService : public FX_NS(network)::EvHttpService, 
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
    HTTPWatchdogService();
    ~HTTPWatchdogService();

public:
    /**
     * Return requsets this service can handle
     * format: request1, request2....
     * @return the requests string
     */
    virtual std::string requestCanHandle() const;

    /**
     * Handle a http request. This method must be thread-safe
     * @param pCtx request context
     */
    virtual void handleRequest(FX_NS(network)::EvHttpRequestContext* pCtx);

protected:
    bool commandToTask(FX_NS(network)::EvHttpRequestContext* pCtx,
                       ExecutionTask& task);
    bool commandToTask(FX_NS(network)::EvHttpRequestContext* pCtx,
                       ExecutionTask& task, int32_t& stopSignal);
    void sendErrorMsg(FX_NS(network)::EvHttpRequestContext* pCtx,
                      const std::string& sErrorMsg, 
                      FX_NS(network)::HTTPTypes::ResponseCode rc);
    void sendErrorInfo(FX_NS(network)::EvHttpRequestContext* pCtx,
                       const ErrorInfo& errorInfo);
    void sendProcessInfo(FX_NS(network)::EvHttpRequestContext* pCtx,
                         const ProcessInfoPtr& pProcInfo);
    void sendPID(FX_NS(network)::EvHttpRequestContext* pCtx, PIDType pid);

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_HTTPWATCHDOGSERVICE_H
