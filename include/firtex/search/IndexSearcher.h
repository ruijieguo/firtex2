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
// Created	: 2010-07-10 21:31:42

#ifndef __FX_INDEXSEARCHER_H
#define __FX_INDEXSEARCHER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/IndexReader.h"
#include "firtex/search/QueryHits.h"
#include "firtex/search/Query.h"
#include "firtex/search/Statement.h"
#include "firtex/search/QueryTracer.h"
#include "firtex/utility/Pool.h"

FX_NS_DEF(queryparser);
class QueryParser;
FX_NS_END

FX_NS_DEF(search);

class IndexSearcher
{
public:
    typedef uint32_t phase_t;
    const static phase_t PHASE_RETRIEVE_ALL = 0;
    const static phase_t PHASE_RETRIEVE_DOC = 1;
    const static phase_t PHASE_RETRIEVE_DOCID = 2;

    static std::string PHASE_RETRIEVE_ALL_STR;
    static std::string PHASE_RETRIEVE_DOC_STR;
    static std::string PHASE_RETRIEVE_DOCID_STR;

    const static size_t RANK_MEMORY_POOL_SIZE = 1024 * 1024;//1MB

public:
    IndexSearcher(const FX_NS(index)::IndexReaderPtr& pIndexReader);
    ~IndexSearcher();

public:
    /**
     * Execute search statement over index database and return hit doc
     * @param statement search statement
     * @return query result
     */
    QueryHitsPtr search(const std::string& sStatements, 
                        FX_NS(queryparser)::QueryParser& queryParser);

    /**
     * Execute search statement over index database and return hit doc
     * @param statement search statement
     * @return query result
     */
    QueryHitsPtr search(const Statement& statement, 
                        FX_NS(queryparser)::QueryParser& queryParser);

    /**
     * Execute query over index database and return hit doc
     * @param pQuery the query to execute
     * @param pSort custom sorter
     * @param pFilter custom filter
     * @param pFilter pTracer query tracer
     * @param nStart start doc to retrieval in hits
     * @param nTopDocs max number of documents to return
     * @param phase search phase
     *        PHASE_RETRIEVE_ALL: execute query and retrieve document
     *        PHASE_RETRIEVE_DOCID: execute query and retrieve docId
     * @return query result
     */
    QueryHitsPtr search(const QueryPtr& pQuery,
                        const SorterPtr& pSort,
                        const FilterPtr& pFilter,
                        QueryTracerPtr& pTracer,
                        size_t nStart, size_t nTopDocs,
                        phase_t phase = PHASE_RETRIEVE_ALL);

    /**
     * Return index reader of searcher
     */
    FX_NS(index)::IndexReaderPtr getIndexReader() const;

    /**
     * Create stored fields reader 
     * @return stored fields reader object which is thread unsafe
     */
    FX_NS(index)::StoredFieldsReaderPtr createStoredFieldsReader() const;

protected:
    void initFeatureProvider();

    QueryHitsPtr doRetrieveDoc(const DocSelectClausePtr& pDocClause,
                               QueryTracerPtr& pTracer);
    QueryHitsPtr doSearch(const QueryClausePtr& pQueryClause, 
                          const SortClausePtr& pSortClause,
                          const FilterClausePtr& pFilterClause,
                          const ParameterClausePtr& pParamClause, 
                          QueryTracerPtr& pTracer,
                          FX_NS(queryparser)::QueryParser& queryParser);

protected:
    FX_NS(index)::IndexReaderPtr m_pIndexReader;
    FX_NS(utility)::PoolPtr m_pPool; 
    FeatureProviderPtr m_pFeatureProvider;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(IndexSearcher);

/////////////////////////////////////////////////
//
inline FX_NS(index)::IndexReaderPtr IndexSearcher::getIndexReader() const
{
    return m_pIndexReader;
}

FX_NS_END

#endif //__FX_INDEXSEARCHER_H
