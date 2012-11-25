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
// Created : 2011-07-02 19:28:02

#ifndef __FX_BASICLOGGINGLAYOUT_H
#define __FX_BASICLOGGINGLAYOUT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/LoggingLevel.h"
#include "firtex/common/LoggingLayout.h"

FX_NS_DEF(common);

class BasicLoggingLayout : public LoggingLayout
{
public:
    DECLARE_LAYOUT_CREATOR(BasicLoggingLayout, "basic");

public:
    BasicLoggingLayout() {}
    ~BasicLoggingLayout() {}

public:
    /**
     * Format the logging data to a string
     * Implement this method to create your own layout format.
     *
     * @param message appendable string 
     * @param event The LoggingEvent.
     */
    virtual void format(std::ostringstream& message, const LoggingEvent& event);

    virtual void configure(FX_NS(config)::Configurator& conf) {}

private:
    static tchar LEVEL_PRINT_PREFIX[LoggingLevel::LEVEL_MAX][20];
};

FX_NS_END

#endif //__FX_BASICLOGGINGLAYOUT_H
