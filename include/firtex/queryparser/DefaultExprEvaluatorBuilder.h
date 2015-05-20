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
// Created	: 2011-01-03 16:24:53

#ifndef __FX_DEFAULTEXPREVALUATORBUILDER_H
#define __FX_DEFAULTEXPREVALUATORBUILDER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/queryparser/ExprEvaluatorBuilder.h"
#include "firtex/search/ArgListExprEvaluator.h"

FX_NS_DEF(index);
class IndexReader;
DEFINE_TYPED_PTR(IndexReader);
FX_NS_END

FX_NS_DEF(search);
class FeatureContext;
FX_NS_END

FX_NS_DEF(queryparser);

class ExprParser;
class DefaultExprEvaluatorBuilder : public ExprEvaluatorBuilder
{
public:
    DefaultExprEvaluatorBuilder(ExprParser& parser, 
                                const FX_NS(index)::IndexReaderPtr& pIndexReader,
                                FX_NS(search)::FeatureContext* pFeatureCtx = NULL);
    ~DefaultExprEvaluatorBuilder();

public:
    /**
     * Create ExprEvaluator for Expression node
     * @return evaluator for expression node
     */
    FX_NS(search)::ExprEvaluator* createExpr(const ExprNode& exprNode);
    
protected:
    static void swapArglist(FX_NS(search)::ExprEvaluator*& pLeftEva, 
                            FX_NS(search)::ArgListExprEvaluator::ArgVector& args);

protected:
    FX_NS(search)::ExprEvaluator* createFuncExprEvaluator(
            const ExprNode& exprNode, FX_NS(search)::ExprEvaluator* pLeftEva);
    FX_NS(search)::ExprEvaluator* createFeatureExprEvaluator(
            const ExprNode& exprNode, FX_NS(search)::ExprEvaluator* pLeftEva);
    FX_NS(search)::ExprEvaluator* createAttrEvaluator(
            const ExprNode& exprNode);
    FX_NS(search)::ExprEvaluator* createInEvaluator(const ExprNode& exprNode);

private:
    template <typename T>
    static bool getValueFromConstList(T& value, const ExprNode& exprNode) {return false;}

private:
    ExprParser& m_parser;
    FX_NS(index)::IndexReaderPtr m_pIndexReader;
    FX_NS(search)::FeatureContext* m_pFeatureCtx;

private:
    DECLARE_STREAM_LOGGER();
};

////////////////////////////
template <>
inline bool DefaultExprEvaluatorBuilder::getValueFromConstList(int64_t& value, const ExprNode& node)
{
    if (node.tokenType != ExprNode::TOK_CONST_LIST)
    {
        FX_LOG(ERROR, "The arguments must be constants.");
        return false;
    }
    const ConstList* pConstList = node.constList;
    FIRTEX_ASSERT2(pConstList);
    const ConstList::IntVector& list = pConstList->getIntList();
    FIRTEX_ASSERT2(list.size() > 0);
    value = list[0];
    return true;
}

template <>
inline bool DefaultExprEvaluatorBuilder::getValueFromConstList(double& value, const ExprNode& node)
{
    if (node.tokenType != ExprNode::TOK_CONST_LIST)
    {
        FX_LOG(ERROR, "The arguments must be constants.");
        return false;
    }
    const ConstList* pConstList = node.constList;
    FIRTEX_ASSERT2(pConstList);
    const ConstList::DoubleVector& list = pConstList->getDoubleList();
    FIRTEX_ASSERT2(list.size() > 0);
    value = list[0];
    return true;
}

template <>
inline bool DefaultExprEvaluatorBuilder::getValueFromConstList(
        std::string& value, const ExprNode& node)
{
    if (node.tokenType != ExprNode::TOK_CONST_LIST)
    {
        FX_LOG(ERROR, "The arguments must be constants.");
        return false;
    }
    const ConstList* pConstList = node.constList;
    FIRTEX_ASSERT2(pConstList);
    const ConstList::StringVector& list = pConstList->getStringList();
    FIRTEX_ASSERT2(list.size() > 0);
    value = list[0];
    return true;
}

FX_NS_END

#endif //__FX_DEFAULTEXPREVALUATORBUILDER_H
