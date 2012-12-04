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
// Created	: 2010-07-04 23:59:14

#ifndef __FX_QUERYEXECUTOR_H
#define __FX_QUERYEXECUTOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/HitCollector.h"
#include "firtex/search/Scorer.h"
#include "firtex/search/MatchedDocSet.h"
#include "firtex/search/FeatureProvider.h"
#include "firtex/search/TermQueryFeature.h"
#include "firtex/search/QueryTracer.h"
#include "firtex/utility/Pool.h"

FX_NS_DEF(search);

class QueryExecutor
{
public:
    const static size_t DEFAULT_MATCHED_DOC_BUFFER_SIZE = 32;

public:
    QueryExecutor();
    QueryExecutor(FeatureProviderPtr& pProvider,
                  FX_NS(utility)::PoolPtr& pPool);


    virtual ~QueryExecutor();

public:
    /**
     * Execute the query, scoring matched documents and put them 
     * into the hit collector
     * @param pCollector hit collector to store and sort scored doc
     * @param pScorer document scorer
     * @param pTracer The query tracer
     */
    virtual void execute(HitCollectorPtr& pCollector,
                         ScorerPtr& pScorer,
                         const QueryTracerPtr& pTracer);

    /**
     * Move to next matched documents
     * @param scoreDocs matched documents stored here
     * @return number of matched documents
     */   
    virtual size_t advance(MatchedDocSet& scoreDocs) = 0;
    
    /** 
     * Skip to document whose id is equal to or greater than <i>docId</i>.
     * @param docId docid to skip to
     * @return an docid whoese value is not less than <i>docId</i>
     */ 
    virtual docid_t skipTo(docid_t docId) = 0;

    /**
     * Get the latest matched document meta
     */
    virtual void currentMatchedDoc(MatchedDoc& matchedDoc) = 0;

    /**
     * Number of atomic terms in this query
     */
    virtual size_t size() const = 0;

    /**
     * Extract query feature
     */
    virtual void extractFeature(QueryFeature& queryFeature) const = 0;

public:
    FeatureProviderPtr getFeatureProvider() const;
    
protected:
    FeatureProviderPtr m_pFeatureProvider;
    FX_NS(utility)::PoolPtr m_pPool;

private:
    DECLARE_STREAM_LOGGER();
};

typedef FX_NS(common)::SharedPtr<QueryExecutor> QueryExecutorPtr;

///////////////////////////////////////////////////
//inline function
inline FeatureProviderPtr QueryExecutor::getFeatureProvider() const 
{
    return m_pFeatureProvider;
}

FX_NS_END

#endif //__FX_QUERYEXECUTOR_H
