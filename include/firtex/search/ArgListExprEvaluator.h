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
// Created	: 2011-01-04 00:02:24

#ifndef __FX_ARGLISTEXPREVALUATOR_H
#define __FX_ARGLISTEXPREVALUATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ExprEvaluator.h"
#include <vector>

FX_NS_DEF(search);

class ArgListExprEvaluator : public ExprEvaluator
{
public:
    typedef std::vector<ExprEvaluator*> ArgVector;

public:
    ArgListExprEvaluator(ExprEvaluator* pLeft, ExprEvaluator* pRight);
    ~ArgListExprEvaluator();

public:
    /**
     * Add one argument
     * @param pExprEval expression evaluator,
     * This class will be the owner of it.
     */
    void addArg(ExprEvaluator* pExprEval);

    /**
     * Return argument list
     */
    const ArgVector& getArgList() const;
    ArgVector& getArgList();

public:
    /**
     * Evaluate the float value of expression.
     * Not implement in this class.
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    float evaluateFloat(const ScoredDoc& scoredDoc);

    /**
     * Evaluate the double value of expression
     * Not implement in this class.
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    double evaluateDouble(const ScoredDoc& scoredDoc);

    /**
     * Evaluate the int32 value of expression
     * Not implement in this class.
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    int32_t evaluateInt32(const ScoredDoc& scoredDoc);

    /**
     * Evaluate the int64 value of expression
     * Not implement in this class.
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    int64_t evaluateInt64(const ScoredDoc& scoredDoc);

    /**
     * Return true if argument list expression
     */
    bool isArgList() const { return true; }

private:
    ArgVector m_arguments;

private:
    DECLARE_STREAM_LOGGER();
};

///////////////////////////////////////////////////
//
inline const ArgListExprEvaluator::ArgVector&
ArgListExprEvaluator::getArgList() const
{
    return m_arguments;
}

inline ArgListExprEvaluator::ArgVector&
ArgListExprEvaluator::getArgList()
{
    return m_arguments;
}

FX_NS_END

#endif //__FX_ARGLISTEXPREVALUATOR_H
