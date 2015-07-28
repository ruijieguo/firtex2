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
// Created : 2013-01-24 21:25:06

#ifndef __FX_BM25TERMSCORER_H
#define __FX_BM25TERMSCORER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/BM25Weight.h"
#include "firtex/search/MatchedDocSet.h"
#include "firtex/index/LengthNormIterator.h"

FX_NS_DEF(search);

class BM25TermScorer
{
public:
    static score_t K1;
    static score_t B;

public:
    BM25TermScorer(const IndexFeature& indexFeature,
                   const BM25Weight& weight,
                   const TermQueryFeature* pTqFeature,
                   uint32_t termScoreIdx)
        : m_indexFeature(indexFeature)
        , m_weight(weight)
        , m_uAvgFieldLen(1)
        , m_uTermScoreIdx(termScoreIdx)
    {
        if (pTqFeature->getTerm())
        {
            const std::string& fieldName = pTqFeature->getTerm()->getField();
            m_uAvgFieldLen = indexFeature.getAverageFieldLength(fieldName);
            if (m_uAvgFieldLen == 0)
            {
                m_uAvgFieldLen = 1;
            }
            m_pLengthNormIt = indexFeature.lengthNorm(fieldName);
        }
    }

public:
    /**
     * Score a batch documents
     */
    void score(MatchedDocSet& matchedDocs);

    /**
     * Score one document
     */
    score_t score(docid_t docId, const TermScoreFeature& termScoreFeature);
    
    /** 
     * return the index of term in the query
     */
    uint32_t getTermIdx() const;

protected:
    const IndexFeature& m_indexFeature;
    BM25Weight m_weight;
    FX_NS(index)::LengthNormIteratorPtr m_pLengthNormIt;
    uint32_t m_uAvgFieldLen;
    uint32_t m_uTermScoreIdx;

private:
    DECLARE_LOGGER();
};

/////////////////////////////////////////////
inline void BM25TermScorer::score(MatchedDocSet& matchedDocs)
{
    FX_TRACE("Begin score:");
    size_t size = matchedDocs.size();
    for (size_t i = 0; i < size; ++i)
    {
        MatchedDoc& md = matchedDocs[i];
        const DocScoreFeature& docScoreFeature = md.scoreFeature();
        const TermScoreFeature& termScoreFeature =
            docScoreFeature.getTermScoreFeature(m_uTermScoreIdx);
        score_t s = score(md.getDocId(), termScoreFeature);
        if (s > 0.0f)
        {
            md.addScore(s);
        }
    }
}

inline score_t BM25TermScorer::score(docid_t docId, 
        const TermScoreFeature& termScoreFeature)
{
    uint32_t uAvgFieldLen = m_uAvgFieldLen;
    float tf = termScoreFeature.getTermFreq();
    FX_TRACE("docId: %d, tf: %f", docId, tf);
    if (tf > 0.0)
    {
        uint32_t fieldLen = uAvgFieldLen;
        if (m_pLengthNormIt)
        {
            m_pLengthNormIt->seek(docId, (int32_t&)fieldLen);
        }
        FX_TRACE("docId: %d, tf: %f, idf: %f, term boost: %f, "
                 "fieldLen: %f, avg fieldLen: %f", 
                 docId, tf, m_weight.getIdf(), 
                 m_weight.getBoost(), (float)fieldLen, (float)uAvgFieldLen);
        score_t bmScore = (score_t)((tf * (K1 + 1)) / (tf + K1 * (1 - B + B * 
                                (fieldLen/uAvgFieldLen))));
        bmScore = bmScore * m_weight.getIdf() * m_weight.getBoost();
        FX_TRACE("docId: %d, BM25 score: %f", docId, bmScore);
        return bmScore;
    }
    return 0.0f;
}

inline uint32_t BM25TermScorer::getTermIdx() const
{
    return m_uTermScoreIdx;
}

FX_NS_END

#endif //__FX_BM25TERMSCORER_H
