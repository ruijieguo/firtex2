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
// Created : 2011-09-10 17:33:23

#ifndef __FX_SERVERAPPLICATION_H
#define __FX_SERVERAPPLICATION_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/util/Application.h"
#include "firtex/extension/util/SharedOptionCallback.h"
#include <string>

FX_NS_DEF(util);

class DaemonAppRunner;
class ServerApplication : public Application
{
public:
    ServerApplication(const std::string& sProgName, const std::string& sDesc);
    ~ServerApplication();

public:
    virtual Status run();

    void setRunAsDaemon(bool bDaemon);
    bool isRunAsDaemon() const;

private:
    void daemonize();
    void setupSignalHandler();

#ifdef FX_WINDOWS
    void registerService();
    void unregisterService();
    bool isRegistered() const;

public:
	static const char* WinErrorInfo();

    static void reportServiceStatus(DWORD dwCurrentState,
		DWORD dwWin32ExitCode, DWORD dwWaitHint);

#endif

private:
    DaemonAppRunner* m_pDaemonRunner;
    /// Run the process as an daemon or not
    bool m_bRunAsDaemon;

#ifdef FX_WINDOWS
public:
    static SERVICE_STATUS m_svcStatus;
    static SERVICE_STATUS_HANDLE m_svcStatusHandle;
	static std::string m_sProgNameCopy;
	static ServerApplication* m_pAppCopy;
#endif

private:
    DECLARE_STREAM_LOGGER();
};

class DaemonAppRunner : public AppRunner
{
public:
    DaemonAppRunner(ServerApplication& app)
        : m_app(app)
    {
    }    

public:
    virtual Status run() {return S_EXIT;};
    virtual void registerOption(Application* pApp) {};

    void addOption()
    {
        m_app.addOption(Option("daemon", "d", "run as daemon")
                        .required(false)
                        .repeatable(false)
                        .callback(SharedOptionCallback(this,
                                        &AppRunner::optionCallback))); 
    }

private:
    virtual void optionCallback(const FX_NS(util)::Option& option, 
                                const std::string& sValue) 
    {
        if (option.matchFull("daemon") || option.matchShort("d"))
        {
            m_app.setRunAsDaemon(true);
        }
    };
private:
    ServerApplication& m_app;
};

/////////////////////////////////////////////////
///
inline void ServerApplication::setRunAsDaemon(bool bDaemon)
{
    m_bRunAsDaemon = bDaemon;
}

inline bool ServerApplication::isRunAsDaemon() const
{
    return m_bRunAsDaemon;
}

FX_NS_END

#endif //__FX_SERVERAPPLICATION_H
