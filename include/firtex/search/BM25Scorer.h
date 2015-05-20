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
// Created	: 2010-11-12 22:38:14

#ifndef __FX_BM25SCORER_H
#define __FX_BM25SCORER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/Scorer.h"
#include "firtex/search/BM25DocScorer.h"
#include <math.h>

FX_NS_DEF(search);

class BM25Scorer : public Scorer
{
public:
    BM25Scorer();
    ~BM25Scorer();

public:
    /**
     * Begin to execute a query
     * @param pProvider providing features for ranking
     * @param queryFeature query features
     */
    void beginQuery(const FeatureProviderPtr& pProvider,
                    const QueryFeature& queryFeature);
    
    /**
     * Score matched documents
     * @param matchedDocs the matched doc set
     */
    void score(MatchedDocSet& matchedDocs);

    /**
     * Query executing is ended
     */
    void endQuery();

protected:
    BM25DocScorer m_docScorer;

private:
    DECLARE_STREAM_LOGGER();
};

//////////////////////////////////////////////////////
//

FX_NS_END

#endif //__FX_BM25SCORER_H
