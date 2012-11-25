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
// Created : 2011-05-02 20:10:15

#ifndef __FX_EVHTTPSERVICEFACTORY_H
#define __FX_EVHTTPSERVICEFACTORY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/network/EvHttpService.h"
#include <string>
#include <map>

FX_NS_DEF(network);

class EvHttpServiceFactory
{
public:
    static std::string NOT_FOUND_SERVICE_NAME;
    static std::string BAD_REQUEST_SERVICE_NAME;

public:
    EvHttpServiceFactory();
    virtual ~EvHttpServiceFactory();

public:
    /**
     * Register a service for specified request
     * @param pService service for handling requests
     */
    void registerService(EvHttpService* pService);

    /**
     * Find service by request string
     * @param sRequest the http request
     * @return service for handling the request
     */
    EvHttpService* findService(const std::string& sRequest) const;

private:
    typedef std::map<std::string, EvHttpService*> ServiceMap;

    ServiceMap m_services;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(EvHttpServiceFactory);

FX_NS_END

#endif //__FX_EVHTTPSERVICEFACTORY_H
