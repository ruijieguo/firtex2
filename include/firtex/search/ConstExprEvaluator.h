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
// Created	: 2011-01-03 19:34:45

#ifndef __FX_CONSTEXPREVALUATOR_H
#define __FX_CONSTEXPREVALUATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ExprEvaluator.h"

FX_NS_DEF(search);

template<typename T>
class ConstExprEvaluator : public ExprEvaluator
{
public :
    typedef T ValueType;

public:
    ConstExprEvaluator(ValueType value);
    ~ConstExprEvaluator();

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

private:
    ValueType m_value;
};

/////////////////////////////////////////////
//
template<typename T>
ConstExprEvaluator<T>::ConstExprEvaluator(ValueType value)
    : ExprEvaluator(ExprTypeTraits<T>::VALUE_TYPE)
    , m_value(value)
{
}

template<typename T>
ConstExprEvaluator<T>::~ConstExprEvaluator()
{
}

template<typename T>
float ConstExprEvaluator<T>::evaluateFloat(const ScoredDoc& scoredDoc)
{
    return (float)m_value;
}

template<typename T>
double ConstExprEvaluator<T>::evaluateDouble(const ScoredDoc& scoredDoc)
{
    return (double)m_value;
}

template<typename T>
int32_t ConstExprEvaluator<T>::evaluateInt32(const ScoredDoc& scoredDoc)
{
    return (int32_t)m_value;
}

template<typename T>
int64_t ConstExprEvaluator<T>::evaluateInt64(const ScoredDoc& scoredDoc)
{
    return (int64_t)m_value;
}

typedef ConstExprEvaluator<float> ConstFloatExprEvaluator;
typedef ConstExprEvaluator<double> ConstDoubleExprEvaluator;
typedef ConstExprEvaluator<int32_t> ConstInt32ExprEvaluator;
typedef ConstExprEvaluator<int64_t> ConstInt64ExprEvaluator;

FX_NS_END

#endif //__FX_CONSTEXPREVALUATOR_H
