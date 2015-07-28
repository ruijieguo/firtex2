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
// Created	: 2011-01-05 23:20:50

#ifndef __FX_ATTREXPREVALUATOR_H
#define __FX_ATTREXPREVALUATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ExprEvaluator.h"
#include "firtex/index/TypedForwardIndexIterator.h"
#include "firtex/index/forwardindex/PrimitiveTypeForwardIndexIterator.h"

FX_NS_DEF(search);

template<typename T>
struct AttrExprEvaluatorTraits
{
    typedef T ValueType;
    typedef FX_NS(index)::FloatForwardIndexIterator AttrIterType;
    DEFINE_TYPED_PTR(AttrIterType);

    static ValueType EMPTY_VALUE;
};

template<typename T> 
typename AttrExprEvaluatorTraits<T>::ValueType AttrExprEvaluatorTraits<T>::EMPTY_VALUE = 0;


template<typename T, typename Traits = AttrExprEvaluatorTraits<T> >
class AttrExprEvaluator : public ExprEvaluator
{
public:
    typedef typename Traits::ValueType ValueType;
    typedef typename Traits::AttrIterType AttrIterType;
    typedef typename Traits::AttrIterTypePtr IterTypePtr;

    typedef typename AttrIterType::ValueType IterValueType;

    static ValueType EMPTY_VALUE;

public:
    AttrExprEvaluator(const IterTypePtr& iter);
    ~AttrExprEvaluator();

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
    ValueType evaluate(const ScoreDoc& scoredDoc);

private:
    IterTypePtr m_pAttrIter;

private:
//    DECLARE_STREAM_LOGGER();
};

template<typename T, typename Traits>
typename AttrExprEvaluator<T, Traits>::ValueType
AttrExprEvaluator<T, Traits>::EMPTY_VALUE = Traits::EMPTY_VALUE;

////////////////////////////////////////////////////////////
//

//SETUP_STREAM_LOGGER_TEMP2(search, AttrExprEvaluator);

template<typename T, typename Traits>
AttrExprEvaluator<T, Traits>::AttrExprEvaluator(const IterTypePtr& iter)
    : ExprEvaluator(ExprTypeTraits<T>::VALUE_TYPE)
    , m_pAttrIter(iter)
{
}

template<typename T, typename Traits>
AttrExprEvaluator<T, Traits>::~AttrExprEvaluator()
{
}

template<typename T, typename Traits>
float AttrExprEvaluator<T, Traits>::evaluateFloat(const ScoreDoc& scoredDoc)
{
    return (float)this->evaluate(scoredDoc);
}

template<typename T, typename Traits>
double AttrExprEvaluator<T, Traits>::evaluateDouble(const ScoreDoc& scoredDoc)
{
    return (double)evaluate(scoredDoc);
}

template<typename T, typename Traits>
int32_t AttrExprEvaluator<T, Traits>::evaluateInt32(const ScoreDoc& scoredDoc)
{
    return (int32_t)evaluate(scoredDoc);
}

template<typename T, typename Traits>
int64_t AttrExprEvaluator<T, Traits>::evaluateInt64(const ScoreDoc& scoredDoc)
{
    return (int64_t)evaluate(scoredDoc);
}

template<typename T, typename Traits>
inline typename AttrExprEvaluator<T, Traits>::ValueType 
AttrExprEvaluator<T, Traits>::evaluate(const ScoreDoc& scoredDoc)
{
    IterValueType value = EMPTY_VALUE;
    bool ret = m_pAttrIter->seek(scoredDoc.getDocId(), value);
    FIRTEX_ASSERT2(ret);
    ret = ret;
    return value;    
}

#define DECLARE_ATTREXPR_EVALUATOR_TRAITS(type, iterType, emptyVal)     \
    template<> struct AttrExprEvaluatorTraits<type>                     \
    {                                                                   \
        typedef type ValueType;                                         \
        typedef iterType AttrIterType;                                  \
        DEFINE_TYPED_PTR(AttrIterType);                                 \
        static ValueType EMPTY_VALUE;                                   \
    };                                                                  \
    FX_NS(search)::AttrExprEvaluatorTraits<type>::ValueType             \
    FX_NS(search)::AttrExprEvaluatorTraits<type>::EMPTY_VALUE = emptyVal

 #define DECLARE_ATTREXPR_EVALUATOR_CAST_TRAITS(name, type, iterType, emptyVal) \
     template<typename T> struct name##AttrExprEvaluatorCastTraits       \
     {                                                                   \
         typedef T ValueType;                                            \
         typedef iterType AttrIterType;                                  \
         DEFINE_TYPED_PTR(AttrIterType);                                 \
         static ValueType EMPTY_VALUE;                                   \
     };                                                                  \
     template<typename T> typename name##AttrExprEvaluatorCastTraits<T>::ValueType \
     name##AttrExprEvaluatorCastTraits<T>::EMPTY_VALUE = emptyVal;       \
     template<> struct name##AttrExprEvaluatorCastTraits<type>           \
     {                                                                   \
         typedef type ValueType;                                         \
         typedef iterType AttrIterType;                                  \
         DEFINE_TYPED_PTR(AttrIterType);                                 \
         static ValueType EMPTY_VALUE;                                   \
     };                                                                  \
     name##AttrExprEvaluatorCastTraits<type>::ValueType                  \
        name##AttrExprEvaluatorCastTraits<type>::EMPTY_VALUE = emptyVal

DECLARE_ATTREXPR_EVALUATOR_TRAITS(int32_t, FX_NS(index)::Int32ForwardIndexIterator, 0);
DECLARE_ATTREXPR_EVALUATOR_TRAITS(int64_t, FX_NS(index)::Int64ForwardIndexIterator, 0);
DECLARE_ATTREXPR_EVALUATOR_TRAITS(float, FX_NS(index)::FloatForwardIndexIterator, 0.0f);
DECLARE_ATTREXPR_EVALUATOR_TRAITS(double, FX_NS(index)::DoubleForwardIndexIterator, 0.0f);

DECLARE_ATTREXPR_EVALUATOR_CAST_TRAITS(UInt32ToInt32, int32_t, FX_NS(index)::UInt32ForwardIndexIterator, 0);
DECLARE_ATTREXPR_EVALUATOR_CAST_TRAITS(UInt64ToInt64, int64_t, FX_NS(index)::UInt64ForwardIndexIterator, 0);

FX_NS_END

#endif //__FX_ATTREXPREVALUATOR_H
