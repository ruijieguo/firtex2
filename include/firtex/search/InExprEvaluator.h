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
// Created	: 2011-01-09 12:00:21

#ifndef __FX_INEXPREVALUATOR_H
#define __FX_INEXPREVALUATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ExprEvaluator.h"
#include "firtex/queryparser/ExprNode.h"

FX_NS_DEF(search);

template<typename T>
class InExprEvaluator : public ExprEvaluator
{
public:
    typedef T ValueType;
    typedef std::vector<ValueType> ValueVector;

public:
    InExprEvaluator(ExprEvaluator* pEval,
                    const FX_NS(queryparser)::ConstList* pConstList);
    ~InExprEvaluator();

public:
    /**
     * Evaluate the float value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    float evaluateFloat(const ScoredDoc& scoredDoc);

    /**
     * Evaluate the double value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    double evaluateDouble(const ScoredDoc& scoredDoc);

    /**
     * Evaluate the int32 value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    int32_t evaluateInt32(const ScoredDoc& scoredDoc);

    /**
     * Evaluate the int64 value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    int64_t evaluateInt64(const ScoredDoc& scoredDoc);

protected:
    ExprEvaluator* m_pLeftEval;
    ValueVector m_values;

private:
    DECLARE_STREAM_LOGGER();
};

SETUP_STREAM_LOGGER_TEMP(search, InExprEvaluator);

///////////////////////////////////////////////
//
template<typename T>
InExprEvaluator<T>::InExprEvaluator(ExprEvaluator* pEval,
                                    const FX_NS(queryparser)::ConstList* pConstList)
    : ExprEvaluator(ExprTypeTraits<T>::VALUE_TYPE)
    , m_pLeftEval(pEval)
{
    if ((pConstList->getRetType() == FX_NS(queryparser)::VALUE_INT32)
        || (pConstList->getRetType() == FX_NS(queryparser)::VALUE_INT64))
    {
        const FX_NS(queryparser)::ConstList::IntVector& intList =
            pConstList->getIntList();
        m_values.reserve(intList.size());
        for (size_t i = 0; i < intList.size(); ++i)
        {
            m_values.push_back((ValueType)intList[i]);
        }
    }
    else if ((pConstList->getRetType() == FX_NS(queryparser)::VALUE_FLOAT)
        || (pConstList->getRetType() == FX_NS(queryparser)::VALUE_DOUBLE))
    {
        const FX_NS(queryparser)::ConstList::DoubleVector& doubleList =
            pConstList->getDoubleList();
        m_values.reserve(doubleList.size());
        for (size_t i = 0; i < doubleList.size(); ++i)
        {
            m_values.push_back((ValueType)doubleList[i]);
        }
    }
    std::sort(m_values.begin(), m_values.end());
}

template<typename T>
InExprEvaluator<T>::~InExprEvaluator()
{
    delete m_pLeftEval;
    m_pLeftEval = NULL;
}

template<typename T>
float InExprEvaluator<T>::evaluateFloat(const ScoredDoc& scoredDoc)
{
    float val = m_pLeftEval->evaluateFloat(scoredDoc);
    bool ret = std::binary_search(m_values.begin(), m_values.end(),
                                  (ValueType)val);
    return ret ? 1.0f : 0.0f;
}

template<typename T>
double InExprEvaluator<T>::evaluateDouble(const ScoredDoc& scoredDoc)
{
    double val = m_pLeftEval->evaluateDouble(scoredDoc);
    bool ret = std::binary_search(m_values.begin(), m_values.end(),
                                  (ValueType)val);
    return ret ? 1.0f : 0.0f;
}

template<typename T>
int32_t InExprEvaluator<T>::evaluateInt32(const ScoredDoc& scoredDoc)
{
    int32_t val = m_pLeftEval->evaluateInt32(scoredDoc);
    bool ret = std::binary_search(m_values.begin(), m_values.end(),
                                  (ValueType)val);
    return ret ? 1 : 0;
}

template<typename T>
int64_t InExprEvaluator<T>::evaluateInt64(const ScoredDoc& scoredDoc)
{
    int64_t val = m_pLeftEval->evaluateInt64(scoredDoc);
    bool ret = std::binary_search(m_values.begin(), m_values.end(),
                                  (ValueType)val);
    return ret ? 1 : 0;
}

typedef InExprEvaluator<int32_t> Int32InExprEvaluator;
typedef InExprEvaluator<int64_t> Int64InExprEvaluator;
typedef InExprEvaluator<float> FloatInExprEvaluator;
typedef InExprEvaluator<double> DoubleInExprEvaluator;

FX_NS_END

#endif //__FX_INEXPREVALUATOR_H
