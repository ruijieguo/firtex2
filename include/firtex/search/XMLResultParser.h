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
// Created : 2011-10-03 23:36:37

#ifndef __FX_XMLRESULTPARSER_H
#define __FX_XMLRESULTPARSER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ResultParser.h"
#include "firtex/utility/XMLDocumentWrapper.h"

FX_NS_DEF(search);

class XMLResultParser : public ResultParser
{
public:
    XMLResultParser();
    ~XMLResultParser();

public:
    /**
     * Parse string result to query result object
     * 
     * @param str result string
     * @param result query result object
     */
    void parse(const std::string& str, 
               QueryResult& result);

private:
    QueryTracer* parseTraceNode(const FX_NS(utility)::XMLNodeWrapperPtr& pTraceRootNode);

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_XMLRESULTPARSER_H
