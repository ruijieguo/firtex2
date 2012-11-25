//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn ruijieguo@gmail.com
// Created	: 2006/6/10
//
#ifndef __ComponentCreatorFactory_H
#define __ComponentCreatorFactory_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/utility/Singleton.h"
#include "FieldComponentCreator.h"
#include "firtex/thread/SynchronizedObject.h"
#include "firtex/utility/String.h"
#include <map>

FX_NS_DEF(index);

class ComponentCreatorFactory : public FX_NS(utility)::Singleton<ComponentCreatorFactory> 
                              , public FX_NS(thread)::SynchronizedObject
{
protected:
    ComponentCreatorFactory(void);

    typedef std::map<tstring,FieldComponentCreator*> CreatorMap;

public:
    /**
     * register a field component Creator
     * @param pCreator field component Creator
     */
    void registerCreator(FieldComponentCreator* pCreator);

    /**
     * get a registered factory
     * @param szIdentifier field component Creator identifier
     */
    FieldComponentCreator* getCreator(const tstring& szIdentifier);

private:
    CreatorMap m_creatorMap;

    DECLARE_LAZY_SINGLETON(ComponentCreatorFactory);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif
