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
// Created : 2013-01-26 23:25:01

#ifndef __FX_QUERYWORDCOUNTFEATUREEVALUATOR_H
#define __FX_QUERYWORDCOUNTFEATUREEVALUATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ExprEvaluator.h"
#include "firtex/search/QueryFeature.h"

FX_NS_DEF(search);

class QueryWordCountFeatureEvaluator : public ExprEvaluator
{
public:
    QueryWordCountFeatureEvaluator(const QueryFeature& queryFeature);
    ~QueryWordCountFeatureEvaluator();

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
    int32_t m_nWordCount;

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_QUERYWORDCOUNTFEATUREEVALUATOR_H
