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
// Created : 2013-01-02 12:34:36

#ifndef __FX_FIELDLENGTHFEATUREEVALUATOR_H
#define __FX_FIELDLENGTHFEATUREEVALUATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ExprEvaluator.h"
#include "firtex/index/LengthNormIterator.h"

FX_NS_DEF(search);

class FieldLengthFeatureEvaluator : public ExprEvaluator
{
public:
    FieldLengthFeatureEvaluator(const FX_NS(index)::LengthNormIteratorPtr& pLengthIter);
    ~FieldLengthFeatureEvaluator();

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
    inline int32_t doEvaluate(const ScoreDoc& scoredDoc);

private:
    FX_NS(index)::LengthNormIteratorPtr m_pLengthNormIt;

private:
    DECLARE_LOGGER();
};

////////////////
inline int32_t FieldLengthFeatureEvaluator::doEvaluate(const ScoreDoc& scoredDoc)
{
    int32_t nLen = 0;
    if (!m_pLengthNormIt->seek(scoredDoc.getDocId(), nLen))
    {
        FIRTEX_THROW_AND_LOG(RuntimeException, "Seek length norm "
                             "FAILED: docId: [%d]", scoredDoc.getDocId());
    }
    return nLen;
}

FX_NS_END

#endif //__FX_FIELDLENGTHFEATUREEVALUATOR_H
