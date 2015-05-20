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
// Created	: 2011-01-03 19:57:40

#ifndef __FX_BINARYEXPREVALUATOR_H
#define __FX_BINARYEXPREVALUATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ExprEvaluator.h"
#include <math.h>

FX_NS_DEF(search);

template<typename EvalOp>
class BinaryExprEvaluator : public ExprEvaluator
{
public:
    typedef EvalOp OpType;
    
public:
    BinaryExprEvaluator(ValueType type, 
                        ExprEvaluator* pLeft,
                        ExprEvaluator* pRight);
    ~BinaryExprEvaluator();

public:
    /**
     * Evaluate the float value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    float evaluateFloat(const ScoreDoc& scoredDoc);

    /**
     * Evaluate the double value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    double evaluateDouble(const ScoreDoc& scoredDoc);

    /**
     * Evaluate the int32 value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    int32_t evaluateInt32(const ScoreDoc& scoredDoc);

    /**
     * Evaluate the int64 value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    int64_t evaluateInt64(const ScoreDoc& scoredDoc);

private:
    OpType m_operator;
    ExprEvaluator* m_pLeftEval;
    ExprEvaluator* m_pRightEval;
};

///////////////////////////////////////////
///
template<typename EvalOp>
BinaryExprEvaluator<EvalOp>::BinaryExprEvaluator(ValueType type, 
        ExprEvaluator* pLeft,
        ExprEvaluator* pRight)
    : ExprEvaluator(type)
    , m_pLeftEval(pLeft)
    , m_pRightEval(pRight)
{
}

template<typename EvalOp>
BinaryExprEvaluator<EvalOp>::~BinaryExprEvaluator()
{
    delete m_pLeftEval;
    delete m_pRightEval;
}

template<typename EvalOp>
float BinaryExprEvaluator<EvalOp>::evaluateFloat(const ScoreDoc& scoredDoc)
{
    return m_operator(m_pLeftEval->evaluateFloat(scoredDoc),
                      m_pRightEval->evaluateFloat(scoredDoc));
}

template<typename EvalOp>
double BinaryExprEvaluator<EvalOp>::evaluateDouble(const ScoreDoc& scoredDoc)
{
    return m_operator(m_pLeftEval->evaluateDouble(scoredDoc),
                      m_pRightEval->evaluateDouble(scoredDoc));
}

template<typename EvalOp>
int32_t BinaryExprEvaluator<EvalOp>::evaluateInt32(const ScoreDoc& scoredDoc)
{
    return m_operator(m_pLeftEval->evaluateInt32(scoredDoc),
                      m_pRightEval->evaluateInt32(scoredDoc));
}

template<typename EvalOp>
int64_t BinaryExprEvaluator<EvalOp>::evaluateInt64(const ScoreDoc& scoredDoc)
{
    return m_operator(m_pLeftEval->evaluateInt64(scoredDoc),
                      m_pRightEval->evaluateInt64(scoredDoc));
}


/////////////////////////////////////////////
// Add operation
#define DEFINE_BINARY_OPERATOR(name, exprFloat, exprDouble, exprInt32, exprInt64) \
    struct name##Op                                                     \
    {                                                                   \
        float operator() (float firstVal, float secondVal)              \
        {                                                               \
            return exprFloat;                                           \
        }                                                               \
        double operator() (double firstVal, double secondVal)           \
        {                                                               \
            return exprDouble;                                          \
        }                                                               \
        int32_t operator() (int32_t firstVal, int32_t secondVal)        \
        {                                                               \
            return exprInt32;                                           \
        }                                                               \
        int64_t operator() (int64_t firstVal, int64_t secondVal)        \
        {                                                               \
            return exprInt64;                                           \
        }                                                               \
    }

#define FLOAT_IF_ELSE(expr) ((expr) ? 1.0f : 0.0f)
#define INT_IF_ELSE(expr) ((expr) ? 1 : 0)

DEFINE_BINARY_OPERATOR(Add,
                       firstVal + secondVal,
                       firstVal + secondVal,
                       firstVal + secondVal,
                       firstVal + secondVal);

DEFINE_BINARY_OPERATOR(Sub,
                       firstVal - secondVal,
                       firstVal - secondVal,
                       firstVal - secondVal,
                       firstVal - secondVal);

DEFINE_BINARY_OPERATOR(Mul,
                       firstVal * secondVal,
                       firstVal * secondVal,
                       firstVal + secondVal,
                       firstVal * secondVal);
DEFINE_BINARY_OPERATOR(Div,
                       firstVal / secondVal,
                       firstVal / secondVal,
                       firstVal / secondVal,
                       firstVal / secondVal);

DEFINE_BINARY_OPERATOR(IDiv,
                       (float)((int32_t)(firstVal) / (int32_t)(secondVal)),
                       (double)((int32_t)(firstVal) / (int32_t)(secondVal)),
                       firstVal / secondVal,
                       firstVal / secondVal);

DEFINE_BINARY_OPERATOR(BitAnd,
                       (float)((int32_t)(firstVal) & (int32_t)(secondVal)),
                       (double)((int32_t)(firstVal) & (int32_t)(secondVal)),
                       firstVal & secondVal,
                       firstVal & secondVal);

DEFINE_BINARY_OPERATOR(BitOr,
                       (float)((int32_t)(firstVal) | (int32_t)(secondVal)),
                       (double)((int32_t)(firstVal) | (int32_t)(secondVal)),
                       firstVal | secondVal,
                       firstVal | secondVal);

DEFINE_BINARY_OPERATOR(LT,
                       FLOAT_IF_ELSE(firstVal < secondVal),
                       FLOAT_IF_ELSE(firstVal < secondVal),
                       INT_IF_ELSE(firstVal < secondVal),
                       INT_IF_ELSE(firstVal < secondVal));

DEFINE_BINARY_OPERATOR(GT,
                       FLOAT_IF_ELSE(firstVal > secondVal),
                       FLOAT_IF_ELSE(firstVal > secondVal),
                       INT_IF_ELSE(firstVal > secondVal),
                       INT_IF_ELSE(firstVal > secondVal));

DEFINE_BINARY_OPERATOR(LTE,
                       FLOAT_IF_ELSE(firstVal <= secondVal),
                       FLOAT_IF_ELSE(firstVal <= secondVal),
                       INT_IF_ELSE(firstVal <= secondVal),
                       INT_IF_ELSE(firstVal <= secondVal));

DEFINE_BINARY_OPERATOR(GTE,
                       FLOAT_IF_ELSE(firstVal >= secondVal),
                       FLOAT_IF_ELSE(firstVal >= secondVal),
                       INT_IF_ELSE(firstVal >= secondVal),
                       INT_IF_ELSE(firstVal >= secondVal));

DEFINE_BINARY_OPERATOR(EQ,
                       FLOAT_IF_ELSE(firstVal == secondVal),
                       FLOAT_IF_ELSE(firstVal == secondVal),
                       INT_IF_ELSE(firstVal == secondVal),
                       INT_IF_ELSE(firstVal == secondVal));

DEFINE_BINARY_OPERATOR(NE,
                       FLOAT_IF_ELSE(firstVal != secondVal),
                       FLOAT_IF_ELSE(firstVal != secondVal),
                       INT_IF_ELSE(firstVal != secondVal),
                       INT_IF_ELSE(firstVal != secondVal));

DEFINE_BINARY_OPERATOR(Min,
                       MIN(firstVal, secondVal),
                       MIN(firstVal, secondVal),
                       MIN(firstVal, secondVal),
                       MIN(firstVal, secondVal));

DEFINE_BINARY_OPERATOR(Max,
                       MAX(firstVal, secondVal),
                       MAX(firstVal, secondVal),
                       MAX(firstVal, secondVal),
                       MAX(firstVal, secondVal));

DEFINE_BINARY_OPERATOR(Pow,
                       (float)(pow(firstVal, secondVal)),
                       (double)(pow(firstVal, secondVal)),
                       (int32_t)(pow((float)firstVal, (float)secondVal)),
                       (int64_t)(pow((float)firstVal, (float)secondVal)));

DEFINE_BINARY_OPERATOR(And,
                       FLOAT_IF_ELSE(firstVal != 0.0f && secondVal != 0.0f),
                       FLOAT_IF_ELSE(firstVal != 0.0f && secondVal != 0.0f),
                       INT_IF_ELSE(firstVal && secondVal),
                       INT_IF_ELSE(firstVal && secondVal));

DEFINE_BINARY_OPERATOR(Or,
                       FLOAT_IF_ELSE(firstVal != 0.0f || secondVal != 0.0f),
                       FLOAT_IF_ELSE(firstVal != 0.0f || secondVal != 0.0f),
                       INT_IF_ELSE(firstVal || secondVal),
                       INT_IF_ELSE(firstVal || secondVal));

typedef BinaryExprEvaluator<AddOp> AddExprEvaluator;
typedef BinaryExprEvaluator<SubOp> SubExprEvaluator;
typedef BinaryExprEvaluator<MulOp> MulExprEvaluator;
typedef BinaryExprEvaluator<DivOp> DivExprEvaluator;
typedef BinaryExprEvaluator<IDivOp> IDivExprEvaluator;

typedef BinaryExprEvaluator<BitAndOp> BitAndExprEvaluator;
typedef BinaryExprEvaluator<BitOrOp> BitOrExprEvaluator;

typedef BinaryExprEvaluator<LTOp> LTExprEvaluator;
typedef BinaryExprEvaluator<GTOp> GTExprEvaluator;
typedef BinaryExprEvaluator<LTEOp> LTEExprEvaluator;
typedef BinaryExprEvaluator<GTEOp> GTEExprEvaluator;
typedef BinaryExprEvaluator<EQOp> EQExprEvaluator;
typedef BinaryExprEvaluator<NEOp> NEExprEvaluator;

typedef BinaryExprEvaluator<MinOp> MinExprEvaluator;
typedef BinaryExprEvaluator<MaxOp> MaxExprEvaluator;

typedef BinaryExprEvaluator<PowOp> PowExprEvaluator;

typedef BinaryExprEvaluator<AndOp> AndExprEvaluator;
typedef BinaryExprEvaluator<OrOp> OrExprEvaluator;


FX_NS_END

#endif //__FX_BINARYEXPREVALUATOR_H
