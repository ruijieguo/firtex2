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
#ifndef __FORWARDINDEXFACTORY_H
#define __FORWARDINDEXFACTORY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/utility/Singleton.h"
#include "firtex/index/ForwardIndexWriter.h"
#include "firtex/index/ForwardIndexReader.h"
#include "firtex/index/ForwardIndexMerger.h"
#include "firtex/thread/SynchronizedObject.h"
#include <map>

FX_NS_DEF(index);

class ForwardIndexFactory : public FX_NS(utility)::Singleton<ForwardIndexFactory> 
                          , public FX_NS(thread)::SynchronizedObject
{
protected:
    ForwardIndexFactory(void);

    typedef std::map<tstring, ForwardIndexCreator*> CreatorMap;

public:
    /**
     * Register forward index creator
     * @param pCretor creator of forward index
     */
    void registerForwardIndex(ForwardIndexCreator* pCreator);

    /**
     * Create a registered forward index writer
     * @param sIdentifier identifier of forward index writer
     */
    ForwardIndexWriter* createForwardIndexWriter(const tstring& sIdentifier);

    /**
     * Create a registered forward index reader
     * @param sIdentifier identifier of forward index reader
     */
    ForwardIndexReader* createForwardIndexReader(const tstring& sIdentifier);

    /**
     * Create a registered forward index Merger
     * @param sIdentifier identifier of forward index Merger
     */
    ForwardIndexMerger* createForwardIndexMerger(const tstring& sIdentifier);

private:
    CreatorMap	m_creatorMap;

    DECLARE_LAZY_SINGLETON(ForwardIndexFactory);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif
