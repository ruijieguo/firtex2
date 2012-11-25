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
// Created : 2012-01-11 23:05:46

#ifndef __FX_SEARCHSERVERBASE_H
#define __FX_SEARCHSERVERBASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "SearchServerConf.h"
#include "SearchResource.h"
#include "../common/ServerBase.h"

FX_NS_DEF(app);

class SearchServerBase : public ServerBase
{
public:
    SearchServerBase();
    ~SearchServerBase();

public:
    /// Initialize resource
    virtual bool init(const std::string& sConfFile);

    /// Stop the server
    virtual void stop();

    /// Wait for finish
    virtual void join();

protected:
    /// Create server configure
    virtual ServerConfBase* createConf();

private:
    void initRefreshThread();

private:
    class RefreshRunner : public FX_NS(thread)::Runnable
    {
    public:
        RefreshRunner(SearchResource& res, int32_t nInterval)
            : m_resource(res)
            , m_nInterval(nInterval)
            , m_bStopRequested(false)
        {
        }

        void run()
        {
            while (!m_bStopRequested)
            {
                m_resource.reopenIndex();
                FX_NS(thread)::Thread::sleep(m_nInterval * 1000);
            }
        }

        void stop()
        {
            m_bStopRequested = true;
        }

    private:
        SearchResource& m_resource;
        int32_t m_nInterval;
        bool m_bStopRequested;
    };
    DEFINE_TYPED_PTR(RefreshRunner);

protected:
    SearchResource m_resource;

    FX_NS(thread)::ThreadPtr m_pRefreshThread;
    RefreshRunnerPtr m_pRefreshRunner;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(SearchServerBase);

FX_NS_END

#endif //__FX_SEARCHSERVERBASE_H
