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
// Created	: 2010-07-09 23:23:49

#ifndef __FX_PHRASEQUERY_H
#define __FX_PHRASEQUERY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/Query.h"

FX_NS_DEF(search);

class PhraseQuery : public Query
{
public:
    PhraseQuery();
    PhraseQuery(const PhraseQuery& src);
    ~PhraseQuery();

public:
    /** 
     * add term 
     * @param pTerm term
     */
    void addTerm(const FX_NS(index)::TermPtr& pTerm);

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
     * set slop
     */
    void setSlop(size_t nSlop);

public:
    const TermVector& getTerms() const; 

protected:
    TermVector m_terms;
    size_t m_nSlop;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(PhraseQuery);

////////////////////////////////////////////////
//
inline const PhraseQuery::TermVector& PhraseQuery::getTerms() const
{
    return m_terms;
}

inline void PhraseQuery::setSlop(size_t nSlop)
{
    m_nSlop = nSlop;
}

FX_NS_END

#endif //__FX_PHRASEQUERY_H
