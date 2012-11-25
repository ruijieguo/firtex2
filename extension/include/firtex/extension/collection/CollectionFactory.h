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
#ifndef _COLLECTIONFACTORY_H
#define _COLLECTIONFACTORY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/common/StdHeader.h"
#include "Collection.h"
#include "firtex/utility/Singleton.h"
#include <map>

FX_NS_DEF(collection);

typedef std::map<std::string,Collection::Creator*> CreatorMap;
typedef std::vector<std::string> CollList;

class CollectionFactory : public FX_NS(thread)::SynchronizedObject,
                          public FX_NS(utility)::Singleton<CollectionFactory>
{		
protected:
    CollectionFactory(void);
    virtual ~CollectionFactory(void);

public:
    /**
     * creator a collection by identifier
     * @param sIdent sIdent of collection
     * @return a new created collection
     */
    Collection*	createCollection(const std::string& sIdent);

    /**
     * register a creator of collection			 
     * @param pCreator creator of collection			 
     */
    void registerCreator(Collection::Creator* pCreator);

    /** 
     * get registered collections
     * @return registered collections 
     */
    CollList getRegisteredCollections();

protected:
    /**
     * get a registered creator
     * @param sIdent identifier of collection
     * @return creator of collection
     */
    Collection::Creator* getCreator(const std::string& sIdent);

    /**
     * get creator map			 
     */
    CreatorMap& getCreatorMap(){return m_creatorsMap;}

private:
    CreatorMap	m_creatorsMap; 

    DECLARE_LAZY_SINGLETON(CollectionFactory);
};

FX_NS_END

#endif
