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
// Created : 2012-01-11 23:36:14

#ifndef __FX_RPCSEARCHSERVER_H
#define __FX_RPCSEARCHSERVER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "SearchServerBase.h"
#include "../common/RPCServer.h"

FX_NS_DEF(app);

class RPCSearchServer : public SearchServerBase
{
public:
    typedef boost::shared_ptr<apache::thrift::server::TServer> TServerPtr;

public:
    RPCSearchServer();
    ~RPCSearchServer();

public:
    /// Run the server
    virtual void run();

    /// Stop the server
    virtual void stop();

    /// Wait for finish
    virtual void join();

private:
    RPCServerPtr m_pServer;

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_RPCSEARCHSERVER_H
