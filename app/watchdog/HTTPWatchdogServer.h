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
// Created : 2012-04-03 19:25:43

#ifndef __FX_HTTPWATCHDOGSERVER_H
#define __FX_HTTPWATCHDOGSERVER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "../common/ServerBase.h"
#include "firtex/extension/network/EvHttpServer.h"

FX_NS_DEF(app);

class HTTPWatchdogServer : public ServerBase
{
public:
    const static size_t DEFAULT_THREAD_POOL_SIZE = 8;

public:
    HTTPWatchdogServer(const std::string& sWorkDir, int32_t nPort);
    ~HTTPWatchdogServer();

public:
    /// Initialize resource
    virtual bool init(const std::string& sConfFile);

    /// Run the server
    virtual void run();

    /// Stop the server
    virtual void stop();

    /// Wait for finish
    virtual void join();

private:
    FX_NS(network)::EvHttpServerPtr m_pServer;
    FX_NS(network)::EvHttpServiceFactoryPtr m_pServiceFactory;
    std::string m_sWorkDir;
    int32_t m_nListenPort;    

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_HTTPWATCHDOGSERVER_H
