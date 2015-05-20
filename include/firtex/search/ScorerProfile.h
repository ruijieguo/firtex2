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
// Created	: 2010-07-11 23:47:41

#ifndef __FX_SCORERPROFILE_H
#define __FX_SCORERPROFILE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/Scorer.h"
#include <map>

FX_NS_DEF(search);

class ScorerProfile
{
public:
    typedef std::map<std::string, std::string> KeyValueMap;

public:
    ScorerProfile();
    virtual ~ScorerProfile();

public:
    /**
     * Return the name of the profile name
     */
    virtual std::string getProfileName() const = 0;

    /**
     * Create scorer for specified query
     */
    virtual ScorerPtr createScorer() const = 0;

    /**
     * Set parameters
     * @param params parameters
     */
    virtual void setParameters(const KeyValueMap& params) {}
};

FX_NS_END

#endif //__FX_SCORERPROFILE_H
