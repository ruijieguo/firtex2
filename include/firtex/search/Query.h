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
// Created	: 2010-07-09 20:10:15

#ifndef __FX_QUERY_H
#define __FX_QUERY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/FeatureProvider.h"
#include "firtex/search/QueryExecutor.h"
#include "firtex/index/Term.h"
#include "firtex/index/IndexReader.h"
#include "firtex/utility/Pool.h"
#include <vector>

FX_NS_DEF(search);

class Query
{
public:
    typedef std::vector<FX_NS(index)::TermPtr> TermVector; 

public:
    Query();
    virtual ~Query();

public:
    /**
     * Get identifier of query
     */
    virtual tstring getIdentifier() const = 0;

    /**
     *  Get number of queries
     */
    virtual size_t size() const = 0;

    /**
     * Create query executor for executing the query
     * @param pIndexReader for read index database
     * @param pProvider provide ranking features for scorer
     * @param pPool provide memory for query executor
     * @return query executor for executing the query
     */
    virtual QueryExecutorPtr createExecutor(FX_NS(index)::IndexReaderPtr& pIndexReader,
                                            FeatureProviderPtr& pProvider,
                                            FX_NS(utility)::PoolPtr& pPool) const = 0;

    /**
     * Clone a new query object
     */
    virtual Query* clone() const = 0;

    /**
     * Convert the query to readable string
     */
    virtual tstring toString() const = 0;

    /**
     * Return all terms of this query
     */   
    virtual void terms(TermVector& terms) const = 0;

public:
    /**
     * Is an instance of a query
     */
    bool isInstanceOf(const tstring& iden) const;

    /** 
     * Get/set boost value of query
     */
    void setBoost(float fBoost) { m_fBoost = fBoost;}
    float getBoost() const {return m_fBoost;}

protected:
    float m_fBoost;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(Query);

FX_NS_END

#endif //__FX_QUERY_H
