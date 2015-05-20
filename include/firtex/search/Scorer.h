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
// Created	: 2010-07-05 20:42:13

#ifndef __FX_SCORER_H
#define __FX_SCORER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/MatchedDocSet.h"
#include "firtex/search/FeatureProvider.h"
#include "firtex/search/QueryFeature.h"

FX_NS_DEF(search);

class Scorer
{
public:
    Scorer() {}
    virtual ~Scorer() {}

public:
    /**
     * Begin to execute a query
     * @param pProvider providing features for ranking
     * @param queryFeature query features
     */
    virtual void beginQuery(const FeatureProviderPtr& pProvider,
                            const QueryFeature& queryFeature) = 0;
    
    /**
     * Score matched documents
     * @param matchedDocs the matched doc set
     */
    virtual void score(MatchedDocSet& matchedDocs) = 0;

    /**
     * Query executing is ended
     */
    virtual void endQuery() = 0;

public:
    /**
     * Set parameter
     */
    virtual void setParameter(const std::string& sParam) {}
};

DEFINE_TYPED_PTR(Scorer);

FX_NS_END

#endif //__FX_SCORER_H
