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
// Created : 2011-09-10 12:23:04

#ifndef __FX_SIGNALHANDLER_H
#define __FX_SIGNALHANDLER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/FastMutex.h"
#include "firtex/thread/Condition.h"

FX_NS_DEF(util);

class SignalHandler : public FX_NS(utility)::Singleton<SignalHandler>
{
public:
    SignalHandler();
    ~SignalHandler();

public:
    /// Signal the shutdown condition
    void shutdown();

    /// Block until the shutdown condition has been signaled
    void wait();
    
private:
    /// true if we should shutdown now
    bool m_bShutdownRequested;
    
    /// Protect the shutdown condition
    FX_NS(thread)::FastMutex m_mutex;

    /// Triggered when it is time to shutdown
    FX_NS(thread)::Condition m_cond;

private:
    DECLARE_STREAM_LOGGER();
};

#ifdef FX_WINDOWS
extern BOOL WINAPI consoleCtrlHandler(DWORD ctrlType);
#else
extern void signalHandler(int sig);
#endif

FX_NS_END

#endif //__FX_SIGNALHANDLER_H
