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
// Created	: 2011-01-03 15:11:49

#ifndef __FX_EXPREVALUATORBUILDER_H
#define __FX_EXPREVALUATORBUILDER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/queryparser/ExprNode.h"
#include "firtex/search/ExprEvaluator.h"

FX_NS_DEF(queryparser);

class ExprEvaluatorBuilder
{
public:
    ExprEvaluatorBuilder() {}
    virtual ~ExprEvaluatorBuilder() {}

public:
    /**
     * Create ExprEvaluator for Expression node
     * @return evaluator for expression node
     */
    virtual FX_NS(search)::ExprEvaluator* createExpr(const ExprNode& exprNode) = 0;
};

FX_NS_END

#endif //__FX_EXPREVALUATORBUILDER_H
