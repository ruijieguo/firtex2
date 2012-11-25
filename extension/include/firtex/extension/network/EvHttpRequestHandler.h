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
// Created : 2012-01-09 22:56:20

#ifndef __FX_EVHTTPREQUESTHANDLER_H
#define __FX_EVHTTPREQUESTHANDLER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/network/EvHttpRequestContext.h"
#include "firtex/utility/BoundedConcurrentQueue.h"
#include "firtex/thread/Runnable.h"
#include "firtex/thread/ThreadPool.h"
#include "firtex/extension/network/EvHttpServiceFactory.h"

FX_NS_DEF(network);

class EvHttpRequestHandler : public FX_NS(thread)::Runnable
{
public:
    const static size_t MAX_REQUEST_QUEUE_SIZE = 500;

public:
    EvHttpRequestHandler(const EvHttpServiceFactoryPtr& pServiceFactory,
                         size_t nWorkThreadCount);
    ~EvHttpRequestHandler();

public:
    /**
     * Process http request asynchronously
     *
     * @param pCtx http request contex
     */ 
    void process(EvHttpRequestContext* pCtx);

public:
    /// Break process loop after finish current processing request
    void stop();

    /// Wait process thread exit
    void join();

private:
    /// Work thread entry
    void run();

private:
    typedef FX_NS(utility)::BoundedConcurrentQueue<EvHttpRequestContext*> RequestQueue;

    EvHttpServiceFactoryPtr m_pServiceFactory;
    RequestQueue m_requests;

    FX_NS(thread)::ThreadPoolPtr m_pLoopThreads;
    
    int64_t m_nRequests;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(EvHttpRequestHandler);

FX_NS_END

#endif //__FX_EVHTTPREQUESTHANDLER_H
