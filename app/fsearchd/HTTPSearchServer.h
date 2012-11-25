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
// Created : 2012-01-11 23:31:22

#ifndef __FX_HTTPSEARCHSERVER_H
#define __FX_HTTPSEARCHSERVER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/network/EvHttpServer.h"
#include "SearchServerBase.h"

FX_NS_DEF(app);

class HTTPSearchServer : public SearchServerBase
{
public:
    HTTPSearchServer();
    ~HTTPSearchServer();

public:
    /// Run the server
    virtual void run();

    /// Stop the server
    virtual void stop();

    /// Wait for finish
    virtual void join();

private:
    FX_NS(network)::EvHttpServerPtr m_pServer;
    FX_NS(network)::EvHttpServiceFactoryPtr m_pServiceFactory;

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_HTTPSEARCHSERVER_H
