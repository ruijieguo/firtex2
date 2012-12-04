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
// Created	: 2012-11-30 20:35:40

#ifndef __FX_ANYQUERY_H
#define __FX_ANYQUERY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/search/Query.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(search);

class AnyQuery : public Query
{
public:
    AnyQuery();
    AnyQuery(const AnyQuery& src);
    ~AnyQuery();

public:
    /**
     * Get identifier of query
     */
    std::string getIdentifier() const;

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
    std::string toString() const;

    /**
     * Return all terms of this query
     */   
    void terms(TermVector& terms) const {}

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(AnyQuery);


FX_NS_END

#endif //__FX_ANYQUERY_H
