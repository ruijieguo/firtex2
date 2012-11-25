//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-09-22 17:17:12

#ifndef __FX_LOGGINGLAYOUTFACTORY_H
#define __FX_LOGGINGLAYOUTFACTORY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/common/LoggingLayout.h"
#include <map>

FX_NS_DEF(common);

class LoggingLayoutFactory : public FX_NS(utility)::Singleton<LoggingLayoutFactory>
{
public:
    LoggingLayoutFactory();
    ~LoggingLayoutFactory();

public:
    /**
     * Register a creator of LoggingLayout
     * @param pCreator creator of LoggingLayout			 
     */
    void registerCreator(LoggingLayout::Creator* pCreator);

    /**
     * Create LoggingLayout by name
     */
    LoggingLayout* createLoggingLayout(const tstring& sIdent);

protected:
    typedef std::map<tstring, LoggingLayout::Creator*> CreatorMap;

    CreatorMap m_creators;
};

FX_NS_END

#endif //__FX_LOGGINGLAYOUTFACTORY_H
