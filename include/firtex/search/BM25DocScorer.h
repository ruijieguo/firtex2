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
// Created : 2013-01-24 23:16:14

#ifndef __FX_BM25DOCSCORER_H
#define __FX_BM25DOCSCORER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/FeatureProvider.h"
#include "firtex/search/QueryFeature.h"
#include "firtex/search/BM25TermScorer.h"
#include "firtex/search/MatchedDocSet.h"

FX_NS_DEF(search);

class BM25DocScorer
{
public:
    BM25DocScorer();
    ~BM25DocScorer();

public:
    /**
     * Initialize scorer
     *
     * @param pProvider feature provider
     * @param queryFeature query feature
     */
    void init(const FeatureProvider* pProvider,
              const QueryFeature& queryFeature,
              const std::string& sField = "");

    /**
     * Score a batch of matched doc
     */
    void score(MatchedDocSet& matchedDocs);

    /**
     * Score one document
     * @param docId doc id
     * @param docScoreFeature document feature
     */
    score_t score(docid_t docId, const DocScoreFeature& docScoreFeature);

    /**
     * Clear scored information
     */
    void clear();

private:
    typedef std::vector<BM25TermScorer*> TermScorerVect;

    TermScorerVect m_termScorers;

private:
    DECLARE_LOGGER();
};

//////////////////////////////////////////////////
inline void BM25DocScorer::score(MatchedDocSet& matchedDocs)
{
    for (TermScorerVect::const_iterator it = m_termScorers.begin();
         it != m_termScorers.end(); ++it)
    {
        (*it)->score(matchedDocs);
    }
}

inline score_t BM25DocScorer::score(docid_t docId, 
                                    const DocScoreFeature& docScoreFeature)
{
    score_t s = 0;
    for (TermScorerVect::const_iterator it = m_termScorers.begin();
         it != m_termScorers.end(); ++it)
    {
        BM25TermScorer* pTermScorer = *it;
        uint32_t uTermIdx = pTermScorer->getTermIdx();
        FIRTEX_ASSERT2(uTermIdx < docScoreFeature.getNumTerms());
        
        s += (*it)->score(docId, docScoreFeature.getTermScoreFeature(uTermIdx));
    }
    return s;
}

FX_NS_END

#endif //__FX_BM25DOCSCORER_H
