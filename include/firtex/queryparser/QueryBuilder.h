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
// Created	: 2010-11-20 22:20:27

#ifndef __FX_QUERYBUILDER_H
#define __FX_QUERYBUILDER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/queryparser/QueryExpr.h"

FX_NS_DEF(queryparser);
class QueryBuilder
{
public:
    QueryBuilder() {}
    virtual ~QueryBuilder() {}

public:
    /**
     * Create term query from term query expression
     * Derived class implements this method to process 
     * term query expression
     * @param expr term query expression
     */
    virtual FX_NS(search)::Query* createQuery(const TermQueryExpr& expr) = 0;

    /**
     * Create any query object from any query expression
     * Derived class implements this method to process 
     * any query expression
     * @param expr any query expression
     */
    virtual FX_NS(search)::Query* createQuery(const AnyQueryExpr& expr) = 0;

    /**
     * Create phrase query from phrase query expression
     * Derived class implements this method to process 
     * phrase query expression
     * @param expr phrase query expression
     */
    virtual FX_NS(search)::Query* createQuery(const PhraseQueryExpr& expr) = 0;

    /**
     * Create boolean query from boolean query expression
     * Derived class implements this method to process 
     * boolean query expression
     * @param expr boolean query expression
     */
    virtual FX_NS(search)::Query* createQuery(const BooleanQueryExpr& expr) = 0;

    /**
     * Create range query from range query expression
     * Derived class implements this method to process 
     * range query expression
     * @param expr range query expression
     */
    virtual FX_NS(search)::Query* createQuery(const RangeQueryExpr& expr) = 0;
};

FX_NS_END

#endif //__FX_QUERYBUILDER_H
