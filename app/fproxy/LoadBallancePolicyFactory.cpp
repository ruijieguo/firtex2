//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2012-01-24 21:42:04

#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/KeyValueParser.h"
#include "LoadBallancePolicyFactory.h"
#include "RoundRobinPolicy.h"
#include <memory>

FX_NS_USE(utility);
FX_NS_DEF(app);

SETUP_LOGGER(app, LoadBallancePolicyFactory);

LoadBallancePolicyFactory::LoadBallancePolicyFactory()
{
    registerCreator(RoundRobinPolicy::Creator::instance());
}

LoadBallancePolicyFactory::~LoadBallancePolicyFactory(void)
{
}

LoadBallancePolicy* LoadBallancePolicyFactory::create(const std::string& sIdentifier)
{
    SCOPED_LOCK();
    std::string sId = sIdentifier;
    if (!startsWithNoCase(sIdentifier, std::string("load_ballance.")))
    {
        sId = "load_ballance." + sId;
    }
    LoadBallancePolicy* pLoadBallancePolicy = NULL;
    LoadBallancePolicy::Creator* pCreator = getCreator(sId);
    if (pCreator)
    {
        pLoadBallancePolicy = pCreator->create();
    }
    if (!pLoadBallancePolicy)
    {
        FX_LOG(ERROR, "Create filter FAILED: [%s]", sIdentifier.c_str());
        return NULL;
    }
    return pLoadBallancePolicy;
}

void LoadBallancePolicyFactory::registerCreator(LoadBallancePolicy::Creator* pCreator)
{
    SCOPED_LOCK();
    std::string str = pCreator->getIdentifier();
    getCreatorMap().insert(make_pair(str, pCreator));
}

LoadBallancePolicy::Creator* LoadBallancePolicyFactory::getCreator(const std::string& _identifier)
{
    std::string str = _identifier;
    CreatorMap& am = getCreatorMap();
    CreatorMap::iterator iter = am.find(str);
    if(iter != am.end())
        return iter->second;
    return NULL;
}

FX_NS_END
