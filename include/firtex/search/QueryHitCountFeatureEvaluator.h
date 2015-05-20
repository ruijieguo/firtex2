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
// Created : 2013-01-26 11:07:15

#ifndef __FX_QUERYHITCOUNTFEATUREEVALUATOR_H
#define __FX_QUERYHITCOUNTFEATUREEVALUATOR_H

#include <vector>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ExprEvaluator.h"
#include "firtex/search/FeatureContext.h"
#include "firtex/search/MatchedDoc.h"

FX_NS_DEF(search);

class QueryHitCountFeatureEvaluator : public ExprEvaluator
{
public:
    QueryHitCountFeatureEvaluator(const FeatureContext* pFeatureCtx);
    ~QueryHitCountFeatureEvaluator();

public:
    /**
     * Evaluate the float value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    virtual float evaluateFloat(const ScoreDoc& scoredDoc);

    /**
     * Evaluate the double value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    virtual double evaluateDouble(const ScoreDoc& scoredDoc);

    /**
     * Evaluate the int32 value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    virtual int32_t evaluateInt32(const ScoreDoc& scoredDoc);

    /**
     * Evaluate the int64 value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    virtual int64_t evaluateInt64(const ScoreDoc& scoredDoc);

private:
    inline int32_t doEvaluate(const MatchedDoc& matchedDoc);

private:
    typedef std::vector<size_t> IdxVect;

    IdxVect m_termIdx;

private:
    DECLARE_LOGGER();
};

////////////////////////////////////////////
inline int32_t QueryHitCountFeatureEvaluator::doEvaluate(const MatchedDoc& matchedDoc)
{
    int32_t nHitCount = 0;
    const DocScoreFeature& docScoreFeature = matchedDoc.scoreFeature();
    for (IdxVect::const_iterator it = m_termIdx.begin();
         it != m_termIdx.end(); ++it)
    {
       FIRTEX_ASSERT2(*it < docScoreFeature.getNumTerms());
       const TermScoreFeature& tsFeature = docScoreFeature.getTermScoreFeature(*it);
       nHitCount += (int32_t)tsFeature.getTermFreq();
    }
    return nHitCount;
}

FX_NS_END

#endif //__FX_QUERYHITCOUNTFEATUREEVALUATOR_H
