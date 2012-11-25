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
// Created	: 2006/6/10
//
#include "firtex/index/ComponentCreatorFactory.h"
#include "firtex/index/datetime/DateTimeComponentCreator.h"
#include "firtex/index/keyword/KeywordComponentCreator.h"
#include "firtex/index/primarykey/PrimaryKeyCreator.h"
#include "firtex/index/text/TextComponentCreator.h"
#include "firtex/index/numeric/SimpleNumericComponentCreator.h"

FX_NS_DEF(index)

SETUP_STREAM_LOGGER(index, ComponentCreatorFactory);

ComponentCreatorFactory::ComponentCreatorFactory()
{
    registerCreator(DateTimeComponentCreator::instance());
    registerCreator(KeywordComponentCreator::instance());
    registerCreator(TextComponentCreator::instance());
    registerCreator(PrimaryKeyCreator::instance());

    registerCreator(SimpleInt8ComponentCreator::instance());
    registerCreator(SimpleUInt8ComponentCreator::instance());

    registerCreator(SimpleInt16ComponentCreator::instance());
    registerCreator(SimpleUInt16ComponentCreator::instance());

    registerCreator(SimpleInt32ComponentCreator::instance());
    registerCreator(SimpleUInt32ComponentCreator::instance());

    registerCreator(SimpleInt64ComponentCreator::instance());
    registerCreator(SimpleUInt64ComponentCreator::instance());

    registerCreator(SimpleUInt128ComponentCreator::instance());
}

void ComponentCreatorFactory::registerCreator(FieldComponentCreator* pCreator)
{
    SCOPED_LOCK();
    tstring str = pCreator->getIdentifier();
    if(m_creatorMap.find(str) != m_creatorMap.end())
    {
        FX_STREAM_LOG(WARN) << _T("Field component Creator: ") 
                            << str << _T(" is already registered.") 
                            << FIRTEX_ENDL;
        return ;
    }
    m_creatorMap.insert(make_pair(str,pCreator));
}

FieldComponentCreator* ComponentCreatorFactory::getCreator(const tstring& szIdentifier)
{
    SCOPED_LOCK();
    tstring str = szIdentifier;
    CreatorMap::const_iterator iter = m_creatorMap.find(szIdentifier);
    if(iter != m_creatorMap.end())
        return iter->second;
    return NULL;
}

FX_NS_END

