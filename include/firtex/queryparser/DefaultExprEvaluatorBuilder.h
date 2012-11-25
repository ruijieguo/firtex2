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

FX_NS_DEF(queryparser);

class ExprParser;
class DefaultExprEvaluatorBuilder : public ExprEvaluatorBuilder
{
public:
    DefaultExprEvaluatorBuilder(ExprParser& parser, 
                                const FX_NS(index)::IndexReaderPtr& pIndexReader);
    ~DefaultExprEvaluatorBuilder();

public:
    /**
     * Create ExprEvaluator for Expression node
     * @return evaluator for expression node
     */
    FX_NS(search)::ExprEvaluator* createExpr(const ExprNode& exprNode);
    
protected:
    static void swapArglist(FX_NS(search)::ExprEvaluator* pLeftEva, 
                            FX_NS(search)::ArgListExprEvaluator::ArgVector& args);

protected:
    FX_NS(search)::ExprEvaluator* createFuncExprEvaluator(
            const ExprNode& exprNode, FX_NS(search)::ExprEvaluator* pLeftEva);
    FX_NS(search)::ExprEvaluator* createAttrEvaluator(
            const ExprNode& exprNode);
    FX_NS(search)::ExprEvaluator* createInNode(const ExprNode& exprNode);

private:
    ExprParser& m_parser;
    FX_NS(index)::IndexReaderPtr m_pIndexReader;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DEFAULTEXPREVALUATORBUILDER_H
