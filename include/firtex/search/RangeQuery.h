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
// Created	: 2010-07-10 09:38:42

#ifndef __FX_RANGEQUERY_H
#define __FX_RANGEQUERY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/Query.h"

FX_NS_DEF(search);

class RangeQuery : public Query
{
public:
    RangeQuery(const FX_NS(index)::TermPtr& pLowerTerm,
               const FX_NS(index)::TermPtr& pUpperTerm,
               bool bInclusive);
    RangeQuery(const RangeQuery& src);
    ~RangeQuery();

public:
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

protected:
    FX_NS(index)::TermPtr m_pLowerTerm;
    FX_NS(index)::TermPtr m_pUpperTerm;
    bool m_bInclusive;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_RANGEQUERY_H
