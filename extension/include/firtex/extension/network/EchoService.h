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
// Created : 2011-05-02 20:12:52

#ifndef __FX_ECHOSERVICE_H
#define __FX_ECHOSERVICE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/network/EvHttpService.h"

FX_NS_DEF(network);

class EchoService : public EvHttpService
{
public:
    EchoService();
    virtual ~EchoService();

public:
    /**
     * Return requsets this service can handle
     * format: request1, request2....
     * @return the requests string
     */
    std::string requestCanHandle() const;

    /**
     * Handle a http request. This method must be thread-safe
     * @param pCtx request context
     */
    virtual void handleRequest(EvHttpRequestContext* pCtx);
};

DEFINE_TYPED_PTR(EchoService);

FX_NS_END

#endif //__FX_ECHOSERVICE_H
