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

FX_NS_DEF(search);

class ScorerProfile
{
public:
    ScorerProfile();
    virtual ~ScorerProfile();

public:
    /**
     * Create scorer for specified query
     */
    ScorerPtr createScorer() const;

    /**
     * Get similarity of this profile
     */
    const Similarity* getSimilarity() const;

protected:

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_SCORERPROFILE_H
