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
// Created	: 2010-07-12 00:00:32

#ifndef __FX_SCORERPROFILEFACTORY_H
#define __FX_SCORERPROFILEFACTORY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ScorerProfile.h"
#include "firtex/utility/Singleton.h"

FX_NS_DEF(search);

class ScorerProfileFactory : public FX_NS(utility)::Singleton<ScorerProfileFactory>
{
protected:
    ScorerProfileFactory();
    ~ScorerProfileFactory();

public:
    /**
     * Register scorer profile
     * 
     * @param pProfile the scorer profile instance
     */
    void registerProfile(ScorerProfile* pProfile);

    /**
     * Return profile by profile name
     */
    const ScorerProfile* getProfile(const std::string& sProfileName = "default");

private:
    void doRegisterProfile(std::string sProfile, ScorerProfile* pProfile);

private:
    typedef std::map<std::string, ScorerProfile*> ProfileMap;

    ProfileMap m_profiles;

private:
    DECLARE_LAZY_SINGLETON(ScorerProfileFactory);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_SCORERPROFILEFACTORY_H
