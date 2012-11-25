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
// Created	: 2010-08-18 22:53:26

#ifndef __FX_INDEXCLEANERFACTORY_H
#define __FX_INDEXCLEANERFACTORY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/IndexCleaner.h"
#include <map>

FX_NS_DEF(index);

class IndexCleanerFactory : public FX_NS(utility)::Singleton<IndexCleanerFactory>
{
public:
    typedef IndexCleaner::CreatorPtr CreatorPtr;

public:
    IndexCleanerFactory();
    ~IndexCleanerFactory();

public:
    /**
     * Register processor
     * 
     * @param pCreator creator of processor
     */
    void registerIndexCleaner(const CreatorPtr& pCreator);

    /**
     * Create file system by identifier
     */
    IndexCleaner* createIndexCleaner(const tstring& sIdentifier);

private:
    typedef std::map<std::string, CreatorPtr> CreatorMap;

    CreatorMap m_fileSysMap;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_INDEXCLEANERFACTORY_H
