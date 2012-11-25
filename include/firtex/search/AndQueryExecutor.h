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
// Created	: 2010-07-07 20:33:31

#ifndef __FX_ANDQUERYEXECUTOR_H
#define __FX_ANDQUERYEXECUTOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/QueryExecutor.h"
#include <vector>

FX_NS_DEF(search);

class AndQueryExecutor : public QueryExecutor
{
public:
    AndQueryExecutor(FeatureProviderPtr& pProvider,
                     FX_NS(utility)::PoolPtr& pPool);
    ~AndQueryExecutor();

public:
    /**
     * Move to next matched documents
     * @param scoreDocs matched documents stored here
     * @return number of matched documents
     */   
    size_t advance(MatchedDocSet& docs);

    /** 
     * Skip to document whose id is equal to or greater than <i>docId</i>.
     * @param docId docid to skip to
     * @return an docid whoese value is not less than <i>docId</i>
     */ 
    docid_t skipTo(docid_t docId);

    /**
     * Get the latest matched document meta
     */
    void currentMatchedDoc(MatchedDoc& matchedDoc);

    /**
     * Extract query feature
     */
    void extractFeature(QueryFeature& queryFeature) const;

    /**
     * Add a sub query executor
     */
    void addExecutor(const QueryExecutorPtr& pExe);

    /**
     * Number of atomic terms in this query
     */
    size_t size() const;

protected:
    void doGetMatchedDoc(MatchedDoc& matchedDoc);

protected:
    typedef std::vector<QueryExecutorPtr> ExecutorVector;

    ExecutorVector m_executors;
    docid_t m_lastDocId;

private:
    DECLARE_STREAM_LOGGER();
};

typedef FX_NS(common)::SharedPtr<AndQueryExecutor> AndQueryExecutorPtr;

///////////////////////////////////////////////////////
//
inline void AndQueryExecutor::addExecutor(const QueryExecutorPtr& pExe)
{
    m_executors.push_back(pExe);
}

inline void AndQueryExecutor::doGetMatchedDoc(MatchedDoc& matchedDoc)
{
    DocScoreFeature& docScoreFeature = matchedDoc.scoreFeature();

    //Add a dummy term score feature
    TermScoreFeature& termScoreFeature = docScoreFeature.nextTermFeature();
    termScoreFeature.setTermFreq(0.0);

    for (size_t i = 0; i < m_executors.size(); ++i)
    {
        m_executors[i]->currentMatchedDoc(matchedDoc);
    }
}

FX_NS_END

#endif //__FX_ANDQUERYEXECUTOR_H
