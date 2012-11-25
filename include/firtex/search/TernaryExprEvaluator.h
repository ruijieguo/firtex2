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
// Created	: 2011-01-03 23:32:09

#ifndef __FX_TERNARYEXPREVALUATOR_H
#define __FX_TERNARYEXPREVALUATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ExprEvaluator.h"

FX_NS_DEF(search);

template<typename EvalOp>
class TernaryExprEvaluator : public ExprEvaluator
{
public:
    typedef EvalOp OpType;
    
public:
    TernaryExprEvaluator(ValueType type, 
                         ExprEvaluator* pEval1,
                         ExprEvaluator* pEval2,
                         ExprEvaluator* pEval3);
    ~TernaryExprEvaluator();

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
    OpType m_operator;
    ExprEvaluator* m_pExprEval1;
    ExprEvaluator* m_pExprEval2;
    ExprEvaluator* m_pExprEval3;
};

////////////////////////////////////////////////////////
//
template<typename EvalOp>
TernaryExprEvaluator<EvalOp>::TernaryExprEvaluator(
        ValueType type, 
        ExprEvaluator* pEval1,
        ExprEvaluator* pEval2,
        ExprEvaluator* pEval3)
    : ExprEvaluator(type)
    , m_pExprEval1(pEval1)
    , m_pExprEval2(pEval2)
    , m_pExprEval3(pEval3)
{
}

template<typename EvalOp>
TernaryExprEvaluator<EvalOp>::~TernaryExprEvaluator()
{
    delete m_pExprEval1;
    delete m_pExprEval2;
    delete m_pExprEval3;
}

template<typename EvalOp>
float TernaryExprEvaluator<EvalOp>::evaluateFloat(const ScoredDoc& scoredDoc)
{
    return m_operator(m_pExprEval1->evaluateFloat(scoredDoc), 
                      m_pExprEval2->evaluateFloat(scoredDoc), 
                      m_pExprEval3->evaluateFloat(scoredDoc));
}

template<typename EvalOp>
double TernaryExprEvaluator<EvalOp>::evaluateDouble(const ScoredDoc& scoredDoc)
{
    return m_operator(m_pExprEval1->evaluateDouble(scoredDoc), 
                      m_pExprEval2->evaluateDouble(scoredDoc), 
                      m_pExprEval3->evaluateDouble(scoredDoc));
}

template<typename EvalOp>
int32_t TernaryExprEvaluator<EvalOp>::evaluateInt32(const ScoredDoc& scoredDoc)
{
    return m_operator(m_pExprEval1->evaluateInt32(scoredDoc), 
                      m_pExprEval2->evaluateInt32(scoredDoc), 
                      m_pExprEval3->evaluateInt32(scoredDoc));
}

template<typename EvalOp>
int64_t TernaryExprEvaluator<EvalOp>::evaluateInt64(const ScoredDoc& scoredDoc)
{
    return m_operator(m_pExprEval1->evaluateInt64(scoredDoc), 
                      m_pExprEval2->evaluateInt64(scoredDoc), 
                      m_pExprEval3->evaluateInt64(scoredDoc));
}

////////////////////////////////////////////////
//define operators
#define DEFINE_TERNARY_OPERATOR(name, exprFloat, exprDouble, exprInt32, exprInt64) \
    struct name##Op                                                     \
    {                                                                   \
        float operator() (float firstVal, float secondVal, float thirdVal) \
        {                                                               \
            return exprFloat;                                           \
        }                                                               \
        double operator() (double firstVal, double secondVal, double thirdVal) \
        {                                                               \
            return exprDouble;                                          \
        }                                                               \
        int32_t operator() (int32_t firstVal, int32_t secondVal, int32_t thirdVal) \
        {                                                               \
            return exprInt32;                                           \
        }                                                               \
        int64_t operator() (int64_t firstVal, int64_t secondVal, int64_t thirdVal) \
        {                                                               \
            return exprInt64;                                           \
        }                                                               \
    }

DEFINE_TERNARY_OPERATOR(If, 
                        (firstVal != 0.0f) ? secondVal : thirdVal,
                        (firstVal != 0.0f) ? secondVal : thirdVal,
                        firstVal ? secondVal : thirdVal,
                        firstVal ? secondVal : thirdVal);

DEFINE_TERNARY_OPERATOR(MAdd, 
                        firstVal * secondVal + thirdVal,
                        firstVal * secondVal + thirdVal,
                        firstVal * secondVal + thirdVal,
                        firstVal * secondVal + thirdVal);

DEFINE_TERNARY_OPERATOR(Mul3, 
                        firstVal * secondVal * thirdVal,
                        firstVal * secondVal * thirdVal,
                        firstVal * secondVal * thirdVal,
                        firstVal * secondVal * thirdVal);

typedef TernaryExprEvaluator<IfOp> IfExprEvaluator;
typedef TernaryExprEvaluator<MAddOp> MAddExprEvaluator;
typedef TernaryExprEvaluator<Mul3Op> Mul3ExprEvaluator;

FX_NS_END

#endif //__FX_TERNARYEXPREVALUATOR_H
