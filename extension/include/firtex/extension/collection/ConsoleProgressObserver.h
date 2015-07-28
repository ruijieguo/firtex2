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
// Created : 2011-07-03 22:57:02

#ifndef __FX_CONSOLEPROGRESSOBSERVER_H
#define __FX_CONSOLEPROGRESSOBSERVER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Timestamp.h"
#include "firtex/thread/FastMutex.h"
#include "firtex/extension/collection/ProgressObserver.h"
#include <atomic>

FX_NS_DEF(collection);

class ConsoleProgressObserver : public ProgressObserver
{
public:
    ConsoleProgressObserver(uint64_t nReportInterval);
    ~ConsoleProgressObserver();

public:
    /// Move ahead
    virtual void step();

    ///  Process complete
    virtual void complete();

private:
    uint64_t m_nProcessedCount;
    uint64_t m_nReportInterval;
    uint64_t m_nLastProcessed;

    FX_NS(thread)::FastMutex m_mutex;
    FX_NS(utility)::Timestamp m_lastProccessTime;
    std::atomic_long m_lastProccessed;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_CONSOLEPROGRESSOBSERVER_H
