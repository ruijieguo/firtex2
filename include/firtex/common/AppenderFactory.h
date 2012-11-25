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

#ifndef __FX_APPENDERFACTORY_H
#define __FX_APPENDERFACTORY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/common/Appender.h"
#include <map>

FX_NS_DEF(common);

class AppenderFactory : public FX_NS(utility)::Singleton<AppenderFactory>
{
public:
    AppenderFactory();
    ~AppenderFactory();

public:
    /**
     * Register a creator of appender
     * @param pCreator creator of appender			 
     */
    void registerCreator(Appender::Creator* pCreator);

    /**
     * Create appender by name
     */
    Appender* createAppender(const tstring& sIdent);

protected:
    typedef std::map<tstring, Appender::Creator*> CreatorMap;

    CreatorMap m_creators;
};

FX_NS_END

#endif //__FX_APPENDERFACTORY_H
