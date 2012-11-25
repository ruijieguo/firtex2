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

#ifndef __FX_LOADBALLANCEPOLICYFACTORY_H
#define __FX_LOADBALLANCEPOLICYFACTORY_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Singleton.h"
#include "firtex/thread/SynchronizedObject.h"
#include "LoadBallancePolicy.h"
#include <map>

FX_NS_DEF(app);

class LoadBallancePolicyFactory : public FX_NS(thread)::SynchronizedObject
                                , public FX_NS(utility)::Singleton<LoadBallancePolicyFactory>
{
public:
    typedef std::map<std::string, LoadBallancePolicy::Creator*> CreatorMap;
    typedef std::vector<std::string>  FilterList;

protected:
    LoadBallancePolicyFactory(void);
    ~LoadBallancePolicyFactory(void);
public:			
    /**
     * create load ballance policy by identifier
     * @param sIdentifier identifier of policy
     * @return Load ballance policy
     */
    LoadBallancePolicy* create(const std::string& sIdentifier);

public:
    /**
     * register a creator of LoadBallancePolicy
     * @param pCreator creator of LoadBallancePolicy
     */
    void registerCreator(LoadBallancePolicy::Creator* pCreator);

protected:
    /**
     * get a registered creator
     * @param _identifier identifier of Tokenfilter
     * @return creator of LoadBallancePolicy
     */
    LoadBallancePolicy::Creator* getCreator(const std::string& _identifier);

    /**
     * get creator map			 
     */
    CreatorMap& getCreatorMap(){return m_creatorsMap;}

protected:
    CreatorMap m_creatorsMap;

    DECLARE_LAZY_SINGLETON(LoadBallancePolicyFactory);

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif
