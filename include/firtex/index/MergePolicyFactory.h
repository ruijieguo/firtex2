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
// Created	: 2011-02-22 23:35:12

#ifndef __FX_MERGEPOLICYFACTORY_H
#define __FX_MERGEPOLICYFACTORY_H

#include <map>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/SynchronizedObject.h"
#include "firtex/utility/Singleton.h"
#include "firtex/index/MergePolicy.h"

FX_NS_DEF(index);

class MergePolicyFactory : public FX_NS(utility)::Singleton<MergePolicyFactory> 
                          , public FX_NS(thread)::SynchronizedObject
{
protected:
    MergePolicyFactory(void);

    typedef std::map<tstring, MergePolicy::Creator*> CreatorMap;

public:
    /**
     * Register forward index creator
     * @param pCretor creator of forward index
     */
    void registerMergePolicy(MergePolicy::Creator* pCreator);

    /**
     * Create a registered merge poilcy
     * @param sIdentifier identifier of merge policy
     */
    MergePolicy* createMergePolicy(const tstring& sIdentifier);

private:
    CreatorMap	m_creatorMap;

    DECLARE_LAZY_SINGLETON(MergePolicyFactory);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_MERGEPOLICYFACTORY_H
