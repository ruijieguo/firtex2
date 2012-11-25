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
#include "firtex/search/TermQueryFeature.h"
#include <math.h>

FX_NS_DEF(search);

class BM25Scorer : public Scorer
{
public:
    static score_t K1;
    static score_t B;

public:
    class Weight
    {
    public:
        Weight() : m_fIdf(0.0), m_fBoost(0.0) {}

    public:
        void init(const IndexFeature& indexFeature,
                  const TermQueryFeature* pTqFeature)
        {
            if (pTqFeature->getQueryType() == TermQueryFeature::QT_PHRASE)
            {
                size_t uNumSuc = pTqFeature->getNumSuccessors();
                FX_TRACE("number successors: %u", (uint32_t)uNumSuc);
                for (size_t i = 0; i < uNumSuc; ++i)
                {
                    Weight w;
                    w.init(indexFeature, (pTqFeature + i + 1));
                    m_fIdf += w.getIdf();
                }
                FX_TRACE("result idf: %f", m_fIdf);
            }
            else 
            {
                df_t df = pTqFeature->getDocFreq();
                if (df > 0)
                {
                    float fNumDocs = (float)indexFeature.getDocCount();
                    m_fIdf = (score_t)(log(fNumDocs / (float)(df + 1)) + 1.0);
                    FX_TRACE("idf: %f, df: %d, total docs: %f",
                            m_fIdf, df, fNumDocs);
                }
            }
            m_fBoost = pTqFeature->getBoost();
        }

        float getIdf() const {return m_fIdf;}
        float getBoost() const {return m_fBoost;}

    protected:
        float m_fIdf;
        float m_fBoost;

    private:
        DECLARE_STREAM_LOGGER();
    };

    class DocScorer
    {
    public:
        DocScorer(const IndexFeature& indexFeature,
                  const Weight& weight,
                  const TermQueryFeature* pTqFeature,
                  uint32_t termScoreIdx)
            : m_indexFeature(indexFeature)
            , m_weight(weight)
            , m_uAvgFieldLen(1)
            , m_uTermScoreIdx(termScoreIdx)
        {
            if (pTqFeature->getTerm())
            {
                const tstring& fieldName = pTqFeature->getTerm()->getField();
                m_uAvgFieldLen = indexFeature.getAverageFieldLength(fieldName);
                if (m_uAvgFieldLen == 0)
                {
                    m_uAvgFieldLen = 1;
                }
                m_pLengthNormIt = indexFeature.lengthNorm(fieldName);
            }
        }

    public:
        void score(MatchedDocSet& matchedDocs);

    protected:
        const IndexFeature& m_indexFeature;
        Weight m_weight;
        FX_NS(index)::LengthNormIteratorPtr m_pLengthNormIt;
        uint32_t m_uAvgFieldLen;
        uint32_t m_uTermScoreIdx;

    private:
        DECLARE_STREAM_LOGGER();
    };

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
    void setupDocScorers(const QueryFeature& queryFeature);
    void clear();

protected:
    typedef std::vector<DocScorer*> DocScorerVect;

    FeatureProviderPtr m_pFeatureProvider;
    DocScorerVect m_docScorers;

private:
    DECLARE_STREAM_LOGGER();
};

//////////////////////////////////////////////////////
//
inline void BM25Scorer::DocScorer::score(MatchedDocSet& matchedDocs)
{
    score_t K1 = BM25Scorer::K1;
    score_t B = BM25Scorer::B;
    uint32_t uAvgFieldLen = m_uAvgFieldLen;

    FX_TRACE("DocScorer::score");
    size_t size = matchedDocs.size();
    for (size_t i = 0; i < size; ++i)
    {
        MatchedDoc& md = matchedDocs[i];
        const DocScoreFeature& docScoreFeature = md.scoreFeature();
        const TermScoreFeature& termScoreFeature =
            docScoreFeature.getTermScoreFeature(m_uTermScoreIdx);
        float tf = termScoreFeature.getTermFreq();
        FX_TRACE("docId: %d, tf: %f", md.getDocId(), tf);
        if (tf > 0.0)
        {
            uint32_t fieldLen = uAvgFieldLen;
            if (m_pLengthNormIt.isNotNull())
            {
                m_pLengthNormIt->seek(md.getDocId(), (int32_t&)fieldLen);
            }
            FX_TRACE("docId: %d, tf: %f, idf: %f, term boost: %f, "
                            "fieldLen: %f, avg fieldLen: %f", 
                            md.getDocId(), tf, m_weight.getIdf(), 
                            m_weight.getBoost(), (float)fieldLen, (float)uAvgFieldLen);
            score_t bmScore = (score_t)((tf * (K1 + 1)) / (tf + K1 * (1 - B + B * 
                                    (fieldLen/uAvgFieldLen))));
            bmScore = bmScore * m_weight.getIdf() * m_weight.getBoost();
            FX_TRACE("docId: %d, BM25 score: %f", md.getDocId(), bmScore);
            md.addScore(bmScore);
        }
    }
}

FX_NS_END

#endif //__FX_BM25SCORER_H
