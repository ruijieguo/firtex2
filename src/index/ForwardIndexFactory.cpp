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
// Email	: ruijieguo@software.ict.ac.cn
// Created	: 2006/6/10
//
#include "firtex/index/ForwardIndexFactory.h"
#include "firtex/index/forwardindex/PrimitiveTypeForwardIndexCreator.h"

FX_NS_DEF(index)

SETUP_STREAM_LOGGER(index, ForwardIndexFactory);

ForwardIndexFactory::ForwardIndexFactory()
{
    registerForwardIndex(PrimitiveTypeForwardIndexCreator<int32_t>::instance());
    registerForwardIndex(PrimitiveTypeForwardIndexCreator<int64_t>::instance());

    registerForwardIndex(PrimitiveTypeForwardIndexCreator<uint32_t>::instance());
    registerForwardIndex(PrimitiveTypeForwardIndexCreator<uint64_t>::instance());

    registerForwardIndex(PrimitiveTypeForwardIndexCreator<float>::instance());
    registerForwardIndex(PrimitiveTypeForwardIndexCreator<double>::instance());
}

void ForwardIndexFactory::registerForwardIndex(ForwardIndexCreator* pCreator)
{
    SCOPED_LOCK();
    tstring str = pCreator->getIdentifier();
    if(m_creatorMap.find(str) != m_creatorMap.end())
    {
        FX_STREAM_LOG(WARN) << _T("Field component producer: ") 
                            << str << _T(" is already registered.") 
                            << FIRTEX_ENDL;
        return ;
    }
    m_creatorMap.insert(make_pair(str, pCreator));
}

ForwardIndexWriter* ForwardIndexFactory::createForwardIndexWriter(const tstring& sIdentifier)
{
    SCOPED_LOCK();

    CreatorMap::const_iterator iter = m_creatorMap.find(sIdentifier);
    if(iter != m_creatorMap.end())
        return iter->second->createWriter();
    return NULL;
}

ForwardIndexReader* ForwardIndexFactory::createForwardIndexReader(const tstring& sIdentifier)
{
    SCOPED_LOCK();

    CreatorMap::const_iterator iter = m_creatorMap.find(sIdentifier);
    if(iter != m_creatorMap.end())
        return iter->second->createReader();
    return NULL;
}

ForwardIndexMerger* ForwardIndexFactory::createForwardIndexMerger(const tstring& sIdentifier)
{
    SCOPED_LOCK();

    CreatorMap::const_iterator iter = m_creatorMap.find(sIdentifier);
    if(iter != m_creatorMap.end())
        return iter->second->createMerger();
    return NULL;
}

FX_NS_END

