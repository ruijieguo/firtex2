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
// Created	: 2011-01-03 20:41:04

#ifndef __FX_UNARYEXPREVALUATOR_H
#define __FX_UNARYEXPREVALUATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include <math.h>

FX_NS_DEF(search);

template<typename EvalOp>
class UnaryExprEvaluator : public ExprEvaluator
{
public:
    typedef EvalOp OpType;
    
public:
    UnaryExprEvaluator(ValueType type, ExprEvaluator* pExprEval);
    ~UnaryExprEvaluator();

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
    ExprEvaluator* m_pExprEval;
};

///////////////////////////////////////////
///
template<typename EvalOp>
UnaryExprEvaluator<EvalOp>::UnaryExprEvaluator(ValueType type, 
        ExprEvaluator* pExprEval)
    : ExprEvaluator(type)
    , m_pExprEval(pExprEval)
{
}

template<typename EvalOp>
UnaryExprEvaluator<EvalOp>::~UnaryExprEvaluator()
{
    delete m_pExprEval;
}

template<typename EvalOp>
float UnaryExprEvaluator<EvalOp>::evaluateFloat(const ScoredDoc& scoredDoc)
{
    return m_operator(m_pExprEval->evaluateFloat(scoredDoc));
}

template<typename EvalOp>
double UnaryExprEvaluator<EvalOp>::evaluateDouble(const ScoredDoc& scoredDoc)
{
    return m_operator(m_pExprEval->evaluateDouble(scoredDoc));
}

template<typename EvalOp>
int32_t UnaryExprEvaluator<EvalOp>::evaluateInt32(const ScoredDoc& scoredDoc)
{
    return m_operator(m_pExprEval->evaluateInt32(scoredDoc));
}

template<typename EvalOp>
int64_t UnaryExprEvaluator<EvalOp>::evaluateInt64(const ScoredDoc& scoredDoc)
{
    return m_operator(m_pExprEval->evaluateInt64(scoredDoc));
}

/////////////////////////////////////////////
// Add operation
#define DEFINE_UNARY_OPERATOR(name, exprFloat, exprDouble, exprInt32, exprInt64) \
    struct name##Op                                                     \
    {                                                                   \
        float operator() (float value)                                  \
        {                                                               \
            return exprFloat;                                           \
        }                                                               \
        double operator() (double value)                                \
        {                                                               \
            return exprDouble;                                          \
        }                                                               \
        int32_t operator() (int32_t value)                              \
        {                                                               \
            return exprInt32;                                           \
        }                                                               \
        int64_t operator() (int64_t value)                              \
        {                                                               \
            return exprInt64;                                           \
        }                                                               \
    }

#define IABS(val) ((val) > 0 ? (val) : (-val))

#ifndef M_LOG2E
#define M_LOG2E		1.44269504088896340736
#endif

#ifndef M_LOG10E
#define M_LOG10E	0.434294481903251827651
#endif

DEFINE_UNARY_OPERATOR(Neg,
                      -value,
                      -value,
                      -value,
                      -value);

DEFINE_UNARY_OPERATOR(Abs,
                      fabs(value),
                      fabs(value),
                      IABS(value),
                      IABS(value));

DEFINE_UNARY_OPERATOR(Ceil,
                      (float)ceil(value),
                      (double)ceil(value),
                      (int32_t)ceil((double)value),
                      (int64_t)ceil((double)value));

DEFINE_UNARY_OPERATOR(Floor,
                      (float)floor(value),
                      (double)floor(value),
                      (int32_t)floor((double)value),
                      (int64_t)floor((double)value));

DEFINE_UNARY_OPERATOR(Sin,
                      (float)sin(value),
                      (double)sin(value),
                      (int32_t)sin((double)value),
                      (int64_t)sin((double)value));

DEFINE_UNARY_OPERATOR(Cos,
                      (float)cos(value),
                      (double)cos(value),
                      (int32_t)cos((double)value),
                      (int64_t)cos((double)value));

DEFINE_UNARY_OPERATOR(Ln,
                      (float)log(value),
                      (double)log(value),
                      (int32_t)log((double)value),
                      (int64_t)log((double)value));

DEFINE_UNARY_OPERATOR(Log2,
                      (float)(log(value) * M_LOG2E),
                      (double)(log(value) * M_LOG2E),
                      (int32_t)(log((double)value) * M_LOG2E),
                      (int64_t)(log((double)value) * M_LOG2E));

DEFINE_UNARY_OPERATOR(Log10,
                      (float)(log(value) * M_LOG10E),
                      (double)(log(value) * M_LOG10E),
                      (int32_t)(log((double)value) * M_LOG10E),
                      (int64_t)(log((double)value) * M_LOG10E));

DEFINE_UNARY_OPERATOR(Exp,
                      (float)exp(value),
                      (double)exp(value),
                      (int32_t)exp((double)value),
                      (int64_t)exp((double)value));

DEFINE_UNARY_OPERATOR(Sqrt,
                      (float)sqrt(value),
                      (double)sqrt(value),
                      (int32_t)sqrt((double)value),
                      (int64_t)sqrt((double)value));

DEFINE_UNARY_OPERATOR(Int32,
                      (float)(value),
                      (double)(value),
                      (int32_t)(value),
                      (int64_t)(value));

DEFINE_UNARY_OPERATOR(Double,
                      (float)(value),
                      (double)(value),
                      (int32_t)(value),
                      (int64_t)(value));

DEFINE_UNARY_OPERATOR(Not,
                      (float)(((float)value == 0.0f) ? 0.0f : 1.0f),
                      (double)(((double)value == 0.0f) ? 0.0f : 1.0f),
                      (int32_t)(value ? 0 : 1),
                      (int64_t)(value ? 0 : 1));

typedef UnaryExprEvaluator<NegOp> NotExprEvaluator;
typedef UnaryExprEvaluator<NotOp> NegExprEvaluator;
typedef UnaryExprEvaluator<AbsOp> AbsExprEvaluator;
typedef UnaryExprEvaluator<CeilOp> CeilExprEvaluator;
typedef UnaryExprEvaluator<FloorOp> FloorExprEvaluator;
typedef UnaryExprEvaluator<SinOp> SinExprEvaluator;
typedef UnaryExprEvaluator<CosOp> CosExprEvaluator;
typedef UnaryExprEvaluator<LnOp> LnExprEvaluator;
typedef UnaryExprEvaluator<Log2Op> Log2ExprEvaluator;
typedef UnaryExprEvaluator<Log10Op> Log10ExprEvaluator;
typedef UnaryExprEvaluator<ExpOp> ExpExprEvaluator;
typedef UnaryExprEvaluator<SqrtOp> SqrtExprEvaluator;
typedef UnaryExprEvaluator<Int32Op> Int32ExprEvaluator;

FX_NS_END

#endif //__FX_UNARYEXPREVALUATOR_H
