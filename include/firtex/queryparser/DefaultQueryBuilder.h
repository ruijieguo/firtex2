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
// Created	: 2010-11-20 22:20:35

#ifndef __FX_DEFAULTQUERYBUILDER_H
#define __FX_DEFAULTQUERYBUILDER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/queryparser/QueryExpr.h"
#include "firtex/queryparser/QueryBuilder.h"
#include "firtex/analyzer/TokenView.h"
#include "firtex/search/TermQuery.h"
#include "firtex/search/PhraseQuery.h"
#include "firtex/search/BooleanQuery.h"
#include "firtex/search/AnyQuery.h"
#include <list>

FX_NS_DEF(queryparser);

class QueryParser;

class DefaultQueryBuilder : public QueryBuilder
{
public:
    DefaultQueryBuilder(const QueryParser& queryParser);
    ~DefaultQueryBuilder();

public:
    /**
     * Create term query from term query expression
     * Derived class implements this method to process 
     * term query expression
     * @param expr term query expression
     */
    FX_NS(search)::Query* createQuery(const TermQueryExpr& expr);

    /**
     * Create any query object from any query expression
     * Derived class implements this method to process 
     * any query expression
     * @param expr any query expression
     */
    FX_NS(search)::Query* createQuery(const AnyQueryExpr& expr);

    /**
     * Create phrase query from phrase query expression
     * Derived class implements this method to process 
     * phrase query expression
     * @param expr phrase query expression
     */
    FX_NS(search)::Query* createQuery(const PhraseQueryExpr& expr);

    /**
     * Create boolean query from boolean query expression
     * Derived class implements this method to process 
     * boolean query expression
     * @param expr boolean query expression
     */
    FX_NS(search)::Query* createQuery(const BooleanQueryExpr& expr);

    /**
     * Create range query from range query expression
     * Derived class implements this method to process 
     * range query expression
     * @param expr range query expression
     */
    FX_NS(search)::Query* createQuery(const RangeQueryExpr& expr);

protected:
    FX_NS(search)::PhraseQuery* createPhraseQuery(const std::string& sField,
            FX_NS(analyzer)::TokenView::Iterator& it);
    FX_NS(search)::BooleanQuery* createBooleanQuery(const std::string& sField,
            FX_NS(analyzer)::TokenView::Iterator& it, bool bReq, bool bProh);

    FX_NS(analyzer)::TokenViewPtr tokenize(const std::string& sField,
            const std::string& sValue);
    const std::string& getFieldName(const QueryExpr& expr);

protected:
    const QueryParser& m_queryParser;

private:
    DECLARE_STREAM_LOGGER();
};

////////////////////////////////////////////////////////////////////

FX_NS_END

#endif //__FX_DEFAULTQUERYBUILDER_H
