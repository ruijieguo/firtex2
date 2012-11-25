//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2006/8/3
//
#include "firtex/extension/collection/CollectionFactory.h"
#include "firtex/extension/collection/DirCollection.h"
#include "firtex/extension/collection/TrecCollection.h"
//#include "firtex/extension/collection/FileListCollection.h"
//#include "firtex/extension/collection/TrecCollection.h"
//#include "firtex/extension/collection/ZIPTrecCollection.h"
//#include "firtex/extension/collection/GZTrecCollection.h"
//#include "firtex/extension/collection/FirteXCollection.h"
//#include "firtex/extension/collection/TWCollection.h"
//#include "firtex/extension/collection/GZTWCollection.h"
#include "firtex/extension/collection/StandardCollection.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/config/GlobalConfig.h"

FX_NS_DEF(collection)

CollectionFactory::CollectionFactory(void)
{
    // registerCreator(FirteXCollection::Creator::instance());
    registerCreator(DirCollection::Creator::instance());
    // registerCreator(FileListCollection::Creator::instance());
    registerCreator(TrecCollection::Creator::instance());
    // registerCreator(GZTrecCollection::Creator::instance());
    // registerCreator(ZIPTrecCollection::Creator::instance());
    // registerCreator(TWCollection::Creator::instance());
    // registerCreator(GZTWCollection::Creator::instance());
    registerCreator(StandardCollection::Creator::instance());
}

CollectionFactory::~CollectionFactory(void)
{
}

Collection* CollectionFactory::createCollection(const std::string& sIdent)
{	
    SCOPED_LOCK();
    std::string str;
    if(!FX_NS(utility)::startsWithNoCase(sIdent, std::string(_T("collection."))))
        str = "collection.";
    str += sIdent;

    Collection::Creator* pCreator = CollectionFactory::getCreator(str);
    Collection* pCol = NULL;
    if(pCreator)
    {
        pCol = pCreator->create();
    }
    return pCol;		
}

void CollectionFactory::registerCreator(Collection::Creator* pCreator)
{
    SCOPED_LOCK();
    std::string str = pCreator->getIdentifier();
    getCreatorMap().insert(make_pair(str, pCreator));
}

Collection::Creator* CollectionFactory::getCreator(const std::string& sIdent)
{
    CreatorMap& cm = getCreatorMap();	
    map<std::string, Collection::Creator*>::const_iterator iter = cm.find(sIdent);
    if(iter != cm.end())
        return iter->second;
    return NULL;
}

CollList CollectionFactory::getRegisteredCollections()
{			
    SCOPED_LOCK();
    CollList cl;
    CreatorMap& am = getCreatorMap();
    CreatorMap::iterator iter = am.begin();
    while(iter != am.end())
    {
        cl.push_back(iter->first);
        iter++;
    }
    return cl;
}

FX_NS_END
