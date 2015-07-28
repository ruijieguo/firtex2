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
// Created	: 2010-07-06 21:38:05

#ifndef __FX_PHRASEQUERYEXECUTOR_H
#define __FX_PHRASEQUERYEXECUTOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/QueryExecutor.h"
#include "firtex/search/PhraseQuery.h"
#include "firtex/index/TermPostingIterator.h"
#include <vector>

FX_NS_DEF(search);

class PhraseQueryExecutor : public QueryExecutor
{
public:
    PhraseQueryExecutor(const PhraseQuery* pQuery,
                        FeatureProviderPtr& pProvider,
                        FX_NS(utility)::PoolPtr& pPool);
    ~PhraseQueryExecutor();

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

public:
    void addPosting(const FX_NS(index)::TermPostingIteratorPtr& pPostingIter);
    void setSlop(size_t nSlop) {m_nSlop = nSlop;}
    
protected:
    void doGetMatchedDoc(MatchedDoc& matchedDoc);
    inline float phraseFreq();

protected:
    typedef std::vector<FX_NS(index)::TermPostingIteratorPtr> PostingVector;

    const PhraseQuery* m_pQuery;
    PostingVector m_postings;
    size_t m_nSlop;
    float m_phraseFreq;
    docid_t m_lastDocId;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(PhraseQueryExecutor);

////////////////////////////////////////////////////
//
inline void PhraseQueryExecutor::addPosting(
    const FX_NS(index)::TermPostingIteratorPtr& pPostingIter)
{
    m_postings.push_back(pPostingIter);
}

inline float PhraseQueryExecutor::phraseFreq()
{
    size_t nTotalPost = m_postings.size();
    loc_t curPos = 0;
    float freq = 0.0;
    float distance = 0;
    loc_t slop = (loc_t)m_nSlop;

    while(true)
    {
        loc_t cursor = 0;
        size_t nMatchedCnt = 0;//match nMatchedCnt
        loc_t nextPos = 0;
        do 
        {
            nextPos = m_postings[cursor]->skipToPosition(curPos + (loc_t)cursor);
            FX_TRACE("curPos: %d, cursor: %u, nextPos: %d", 
                            curPos, (uint32_t)cursor, nextPos);
            if (nextPos == INVALID_POS)
            {
                return freq; //No more matched position
            }

            if (nextPos > curPos + (loc_t)cursor + slop)
            {
                curPos = nextPos - (loc_t)cursor;
                nMatchedCnt = 1;
                distance = 0;
                FX_TRACE("Unmatch: curPos: %d, distance: %f",
                        curPos, distance);
            } 
            else 
            {
                ++nMatchedCnt; // matched position
                distance = (nMatchedCnt == 1) ? 0 : (float)(nextPos - curPos - cursor);
                FX_TRACE("matched: distance: %f", distance);
            }
            nextPos -= (loc_t)cursor;
            ++cursor;
            cursor %= nTotalPost;
        } while (nMatchedCnt < nTotalPost);
        freq += 1 / (1 + distance);
        FX_TRACE("matched result: distance: %f, freq: %f", distance, freq);
        curPos = nextPos + 1; //Move to next position
    }

    return  freq;
}

inline void PhraseQueryExecutor::doGetMatchedDoc(MatchedDoc& matchedDoc)
{
    docid_t curDocId = m_postings[0]->doc();
    bool bMatched = (matchedDoc.getDocId() == curDocId);

    FX_TRACE("Extract term score feature of phrase query: docId %d",
                    matchedDoc.getDocId());

    DocScoreFeature& docScoreFeature = matchedDoc.scoreFeature();
    TermScoreFeature& termScoreFeature = docScoreFeature.nextTermFeature();
    //first term score feature is phrase freq
    termScoreFeature.setTermFreq(bMatched ? m_phraseFreq : 0.0f);

    for (size_t i = 0; i < m_postings.size(); ++i)
    {
        TermScoreFeature& termScoreFeature = docScoreFeature.nextTermFeature();
        termScoreFeature.setTermFreq(bMatched ? (float)m_postings[i]->freq() : 0.0f);
        FX_TRACE("Extract sub term score feature of phrase query: docId %d, tf: %f",
                        matchedDoc.getDocId(), termScoreFeature.getTermFreq());

    }
}

FX_NS_END

#endif //__FX_PHRASEQUERYEXECUTOR_H
