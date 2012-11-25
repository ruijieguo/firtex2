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
// Created	: 2010-07-07 23:09:33

#ifndef __FX_REQOPTQUERYEXECUTOR_H
#define __FX_REQOPTQUERYEXECUTOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/QueryExecutor.h"

FX_NS_DEF(search);

class ReqOptQueryExecutor : public QueryExecutor
{
public:
    ReqOptQueryExecutor(FeatureProviderPtr& pProvider,
                        FX_NS(utility)::PoolPtr& pPool,
                        const QueryExecutorPtr& pReqExe,
                        const QueryExecutorPtr& pOptExe);
    ~ReqOptQueryExecutor();

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
     * Number of atomic terms in this query
     */
    size_t size() const;

protected:
    void doGetMatchedDoc(MatchedDoc& matchedDoc);

protected:
    QueryExecutorPtr m_pReqExecutor;
    QueryExecutorPtr m_pOptExecutor;
    docid_t m_lastDocId;

private:
    DECLARE_STREAM_LOGGER();
};

//////////////////////////////////////////////////////////////////////////
//

inline void ReqOptQueryExecutor::doGetMatchedDoc(MatchedDoc& matchedDoc)
{
    DocScoreFeature& docScoreFeature = matchedDoc.scoreFeature();

    //Add a dummy term score feature
    TermScoreFeature& termScoreFeature = docScoreFeature.nextTermFeature();
    termScoreFeature.setTermFreq(0.0);

    m_pReqExecutor->currentMatchedDoc(matchedDoc);

    if (m_pOptExecutor.isNotNull())
    {
        m_pOptExecutor->skipTo(matchedDoc.getDocId());
        m_pOptExecutor->currentMatchedDoc(matchedDoc);
    }
}

FX_NS_END

#endif //__FX_REQOPTQUERYEXECUTOR_H
