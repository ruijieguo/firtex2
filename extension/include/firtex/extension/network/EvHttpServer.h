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
// Created : 2012-01-09 20:01:33

#ifndef __FX_EVHTTPSERVER_H
#define __FX_EVHTTPSERVER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/network/EvHttpRequestContext.h"
#include "firtex/extension/network/EvHttpServiceFactory.h"
#include "firtex/extension/network/EvHttpRequestHandler.h"
#include "firtex/thread/Thread.h"
#include <event2/event.h>

FX_NS_DEF(network);

class EvHttpServer : public FX_NS(thread)::Runnable
{
public:
    EvHttpServer(const std::string& sAddr, int32_t nPort,
                 const EvHttpServiceFactoryPtr& pServiceFactory,
                 size_t nWorkThreadCount);
    ~EvHttpServer();

public:
    /**
     * Start the server
     *
     * @param bBlocking true if blocking here
     * @throw NetworkException if failed
     */
    void start(bool bBlocking);

    /// Stop the server
    void stop();

    /// Wait everything finish
    void join();

private:
    void process(struct evhttp_request* pReq);
    void complete(EvHttpRequestContext* pCtx);

    void run();

    static void handleRequest(struct evhttp_request* pReq, void* self);

private:
    /// event base of libevent
    struct event_base* m_pEventBase;
    
    /// http object of libevent
    struct evhttp* m_pEventHttp;

    FX_NS(thread)::ThreadPtr m_pEventThread;

    /// Handler for request
    EvHttpRequestHandlerPtr m_pRequestHandler;

    /// The host name
    std::string m_sHost;
    
    /// Listening port
    int32_t m_nPort;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(EvHttpServer);

FX_NS_END

#endif //__FX_EVHTTPSERVER_H
