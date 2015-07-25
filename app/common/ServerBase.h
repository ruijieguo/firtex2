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
// Created : 2012-01-11 22:53:42

#ifndef __FX_SERVERBASE_H
#define __FX_SERVERBASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "ServiceConf.h"

FX_NS_DEF(app);

class ServerBase
{
public:
    ServerBase();
    virtual ~ServerBase();

public:
    /// Initialize resource
    virtual bool init(const std::string& sConfFile) = 0;

    /// Run the server
    virtual void run() = 0;

    /// Stop the server
    virtual void stop() = 0;

    /// Wait for finish
    virtual void join() = 0;

protected:
    bool loadConf(const std::string& sConfFile);
    void setupLogger();

protected:
    ServiceConf m_conf;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(ServerBase);

FX_NS_END

#endif //__FX_SERVERBASE_H
