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
// Created	: 2011-01-03 12:08:21

#ifndef __FX_EXPREVALUATOR_H
#define __FX_EXPREVALUATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ScoreDoc.h"
#include "firtex/queryparser/ExprNode.h"

FX_NS_DEF(search);

class ExprEvaluator
{
public:
    typedef FX_NS(queryparser)::ValueType ValueType;

public:
    ExprEvaluator(ValueType type) : m_type(type) {}
    virtual ~ExprEvaluator() {}

public:
    ValueType getType() const;

public:
    /**
     * Evaluate the float value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    virtual float evaluateFloat(const ScoreDoc& scoredDoc) = 0;

    /**
     * Evaluate the double value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    virtual double evaluateDouble(const ScoreDoc& scoredDoc) = 0;

    /**
     * Evaluate the int32 value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    virtual int32_t evaluateInt32(const ScoreDoc& scoredDoc) = 0;

    /**
     * Evaluate the int64 value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    virtual int64_t evaluateInt64(const ScoreDoc& scoredDoc) = 0;

public:
    /**
     * Return true if argument list expression
     */
    virtual bool isArgList() const { return false; }

private:
    ValueType m_type;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(ExprEvaluator);

//////////////////////////////////////////////
//
inline ExprEvaluator::ValueType ExprEvaluator::getType() const
{
    return m_type;
}


template<typename T>
struct ExprTypeTraits
{
    static const FX_NS(queryparser)::ValueType VALUE_TYPE = FX_NS(queryparser)::VALUE_NONE;
};

#define DEFINE_EXPR_TYPE_TRAITS(_type, _valType)        \
    template<> struct ExprTypeTraits<_type>             \
    {                                                   \
        static const FX_NS(queryparser)::ValueType VALUE_TYPE = FX_NS(queryparser)::_valType; \
    }

DEFINE_EXPR_TYPE_TRAITS(int32_t, VALUE_INT32);
DEFINE_EXPR_TYPE_TRAITS(int64_t, VALUE_INT64);
DEFINE_EXPR_TYPE_TRAITS(float, VALUE_FLOAT);
DEFINE_EXPR_TYPE_TRAITS(double, VALUE_DOUBLE);

FX_NS_END

#endif //__FX_EXPREVALUATOR_H
