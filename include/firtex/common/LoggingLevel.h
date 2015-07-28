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
// Created : 2011-07-02 22:51:21

#ifndef __FX_LOGGINGLEVEL_H
#define __FX_LOGGINGLEVEL_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/StringUtils.h"

FX_NS_DEF(common);

struct LoggingLevel
{
public:
    typedef enum
    {
        LEVEL_FATAL    = 1,
        LEVEL_CRITICAL,
        LEVEL_ERROR,
        LEVEL_WARN,
        LEVEL_NOTICE,
        LEVEL_INFO,
        LEVEL_DEBUG,
        LEVEL_TRACE,
        LEVEL_MAX,
    } LevelType;

public:
    static std::string levelToStr(LevelType lvl)
    {
        FIRTEX_ASSERT2(lvl >= LEVEL_FATAL && lvl < LEVEL_MAX);
        return LEVEL_PREFIX[lvl];
    }

    static LevelType strToLevel(const std::string& sLvl)
    {
        for (uint32_t lvl = (uint32_t)LEVEL_FATAL; lvl < (uint32_t)LEVEL_MAX; ++lvl)
        {
            if (!FX_NS(utility)::strCompareNoCase(sLvl.c_str(), LEVEL_PREFIX[lvl]))
            {
                return (LevelType)lvl;
            }            
        }

        FIRTEX_THROW(BadParameterException, "Not a valid log level : [%s]", sLvl.c_str());
    }

private:
    static char LEVEL_PREFIX[LEVEL_MAX][20];
};

FX_NS_END

#endif //__FX_LOGGINGLEVEL_H
