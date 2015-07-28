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
// Created	: 2010-07-09 20:35:40

#ifndef __FX_TERMQUERY_H
#define __FX_TERMQUERY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/search/Query.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(search);

class TermQuery : public Query
{
public:
    TermQuery(FX_NS(index)::Term* pTerm);
    TermQuery(const FX_NS(index)::TermPtr& pTerm);
    TermQuery(const TermQuery& src);
    ~TermQuery();

public:
    /**
     * Get identifier of query
     */
    string getIdentifier() const;

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
    string toString() const;

    /**
     * Return all terms of this query
     */   
    void terms(TermVector& terms) const;

public:
    inline FX_NS(index)::TermPtr getTerm() const;

protected:
    FX_NS(index)::TermPtr m_pTerm;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(TermQuery);

//////////////////////////////////////////////////////////////
// 
inline FX_NS(index)::TermPtr TermQuery::getTerm() const
{
    return m_pTerm;
}

FX_NS_END

#endif //__FX_TERMQUERY_H
