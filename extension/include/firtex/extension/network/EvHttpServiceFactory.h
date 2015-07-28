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
#include "firtex/extension/network/EvHttpRequestContext.h"
#include "firtex/extension/util/Regex.h"
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
    EvHttpService* findService(const EvHttpRequestContext* pCtx) const;

private:
    class Router
    {
    public:
        bool init(const std::string& sRouterDirective)
        {
            std::size_t n = sRouterDirective.find(" ");
            if (n == std::string::npos)
            {
                return false;
            }
            std::string cmd = sRouterDirective.substr(0, n);
            if (cmd == "GET")
            {
                m_cmdType = EVHTTP_REQ_GET;
            }
            else if (cmd == "POST")
            {
                m_cmdType = EVHTTP_REQ_POST;
            }
            else if (cmd == "HEAD")
            {
                m_cmdType = EVHTTP_REQ_HEAD;
            }
            else if (cmd == "PUT")
            {
                m_cmdType = EVHTTP_REQ_POST;
            }
            else if (cmd == "DELETE")
            {
                m_cmdType = EVHTTP_REQ_POST;
            }
            else 
            {
                return false;
            }
            if ((int32_t)sRouterDirective.size() - (int32_t)n - 1 <= 0)
            {
                return false;
            }
            std::string path = sRouterDirective.substr(n + 1, sRouterDirective.size() - n - 1);
            m_pRE.reset(new FX_NS(util)::Regex());
            if (!m_pRE->compile(path))
            {
                return false;
            }
            return true;
        }


        bool match(evhttp_cmd_type cmd, const std::string& sResource) const
        {
            if (cmd != m_cmdType)
            {
                return false;
            }
            if (m_pRE->match(sResource) == FX_NS(util)::Regex::MATCH_OK)
            {
                return true;
            }
            return false;
        }
            
    private:
        evhttp_cmd_type m_cmdType;
        FX_NS(util)::RegexPtr m_pRE;
    };
    DEFINE_TYPED_PTR(Router);

private:
    typedef std::vector<std::pair<RouterPtr, FX_NS(network)::EvHttpServicePtr> > ServiceVector;

    ServiceVector m_services;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(EvHttpServiceFactory);

FX_NS_END

#endif //__FX_EVHTTPSERVICEFACTORY_H
