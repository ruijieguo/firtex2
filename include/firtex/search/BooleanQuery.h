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
// Created	: 2010-07-10 00:05:44

#ifndef __FX_BOOLEANQUERY_H
#define __FX_BOOLEANQUERY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/Query.h"
#include <vector>

FX_NS_DEF(search);

struct BooleanClause
{
    BooleanClause(const QueryPtr& pQuery, bool r, bool p)
        : query(pQuery)
        , required(r)
        , prohibited(p)
    {
    }

    QueryPtr query;
    bool required;
    bool prohibited;
};

typedef std::vector<BooleanClause> BooleanClauseVector;

class BooleanQuery : public Query
{
public:
    const static size_t MAX_CLAUSE_COUNT = 1024;

public:
    BooleanQuery();
    BooleanQuery(const BooleanQuery& src);
    ~BooleanQuery();

public:
    /**
     * Add a clause
     */
    void addClause(const QueryPtr& pQuery, bool bRequired, bool bProhibited);

    /**
     * Add a clause
     */
    void addClause(const BooleanClause& clause);

    /**
     * Get identifier of query
     */
    tstring getIdentifier() const;

    /**
     *  Get number of queries
     */
    size_t size() const;

    /**
     * Create query executor for executing the query
     * @param pIndexReader for read index database
     * @param pProvider provide ranking features for scorer
     * @param pPool provide memory for query executor
     * @return query executor for executing the query
     */
    QueryExecutorPtr createExecutor(FX_NS(index)::IndexReaderPtr& pIndexReader,
                                    FeatureProviderPtr& pProvider,
                                    FX_NS(utility)::PoolPtr& pPool) const;

    /**
     * Clone a new query object
     */
    Query* clone() const;

    /**
     * Convert the query to readable string
     */
    tstring toString() const;

    /**
     * Return all terms of this query
     */   
    void terms(TermVector& terms) const;

    /**
     * Get clauses
     */
    BooleanClauseVector& getClauses();
    const BooleanClauseVector& getClauses() const;

    /**
     * Set min should match of OR boolean query
     */
    void setMinShouldMatch(size_t nMinShouldMatch);

protected:
    BooleanClauseVector m_clauses;
    size_t m_nMinShouldMatch;
    
private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(BooleanQuery);

/////////////////////////////////////////////////////////
//
inline BooleanClauseVector& BooleanQuery::getClauses()
{
    return m_clauses;
}

inline const BooleanClauseVector& BooleanQuery::getClauses() const
{
    return m_clauses;
}

inline void BooleanQuery::setMinShouldMatch(size_t nMinShouldMatch)
{
    m_nMinShouldMatch = nMinShouldMatch;
}

FX_NS_END

#endif //__FX_BOOLEANQUERY_H
