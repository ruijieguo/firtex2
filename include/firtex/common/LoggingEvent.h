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
// Created : 2011-07-02 19:02:34

#ifndef __FX_LOGGINGEVENT_H
#define __FX_LOGGINGEVENT_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Timestamp.h"
#include "firtex/thread/Thread.h"
#include "firtex/common/LoggingLevel.h"
#include <string>

FX_NS_DEF(common);

struct LoggingEvent
{
public:
    LoggingEvent(const std::string& logger, const std::string& msg,
                 LoggingLevel::LevelType lvl)
        : loggerName(logger), message(msg), level(lvl), line(-1), threadId(0)
    {
    }

    LoggingEvent(const std::string& logger, const std::string& msg,
                 LoggingLevel::LevelType lvl, const std::string& f, 
                 int32_t l, const std::string& func)
        : loggerName(logger), message(msg), level(lvl)
        , file(f), line(l), function(func)
        , threadId(FX_NS(thread)::Thread::getSysThreadId())
    {
    }

    ~LoggingEvent() {}

public:
    /// The logger name.
    const std::string loggerName;

    /// The application supplied message of logging event.
    const std::string message;

    /// Level of logging event.
    const LoggingLevel::LevelType level;

    /// File name.
    const std::string file;

    /// Line count.
    const int32_t line;

    /// Function name.
    const std::string function;

    /**
     * The name of thread, e.g. the process id or thread id.
     */
    const FX_NS(thread)::Thread::threadid_t threadId;

    /// The time stamp
    FX_NS(utility)::Timestamp timestamp;
};

FX_NS_END

#endif //__FX_LOGGINGEVENT_H
