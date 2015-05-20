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
// Created : 2012-12-31 21:03:09

#ifndef __FX_BM25SCORERPROFILE_H
#define __FX_BM25SCORERPROFILE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ScorerProfile.h"

FX_NS_DEF(search);

class BM25ScorerProfile : public ScorerProfile
{
public:
    BM25ScorerProfile();
    ~BM25ScorerProfile();

public:
    /**
     * Return the name of the profile name
     */
    virtual std::string getProfileName() const;

    /**
     * Create scorer for specified query
     */
    virtual ScorerPtr createScorer() const;
    
private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_BM25SCORERPROFILE_H
