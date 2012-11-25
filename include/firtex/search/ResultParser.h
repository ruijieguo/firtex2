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
// Created : 2011-10-03 23:36:22

#ifndef __FX_RESULTPARSER_H
#define __FX_RESULTPARSER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/QueryResult.h"

FX_NS_DEF(search);

class ResultParser
{
public:
    ResultParser() {}
    virtual ~ResultParser() {}

public:
    /**
     * Parse string result to query result object
     * 
     * @param str result string
     * @param result query result object
     */
    virtual void parse(const std::string& str, 
                       FX_NS(search)::QueryResult& result) = 0;
};

FX_NS_END

#endif //__FX_RESULTPARSER_H
