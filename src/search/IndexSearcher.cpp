#include "firtex/search/IndexSearcher.h"
#include "firtex/search/SimpleHitCollector.h"
#include "firtex/search/SortedHitCollector.h"
#include "firtex/search/ScorerProfileFactory.h"
#include "firtex/queryparser/QueryParser.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/LooseBoundedPool.h"

FX_NS_USE(index);
FX_NS_USE(utility);
FX_NS_USE(config);
FX_NS_USE(queryparser);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, IndexSearcher);

std::string IndexSearcher::PHASE_RETRIEVE_ALL_STR = "0";
std::string IndexSearcher::PHASE_RETRIEVE_DOC_STR = "1";
std::string IndexSearcher::PHASE_RETRIEVE_DOCID_STR = "2";

IndexSearcher::IndexSearcher(const IndexReaderPtr& pIndexReader)
    : m_pIndexReader(pIndexReader)
{
    m_pPool = new LooseBoundedPool(RANK_MEMORY_POOL_SIZE);
    initFeatureProvider();
}

IndexSearcher::~IndexSearcher() 
{
}

void IndexSearcher::initFeatureProvider()
{
    const ScorerProfile* pScorerProf =
        ScorerProfileFactory::instance()->getProfile();
    m_pFeatureProvider = new FeatureProvider();
    m_pFeatureProvider->init(m_pIndexReader, pScorerProf->getSimilarity());
}

QueryHitsPtr IndexSearcher::search(const string& sStatements,
                                   QueryParser& queryParser)
{
    Statement stat;
    if (!stat.fromString(sStatements))
    {
        FX_LOG(WARN, "Parse search statements: [%s] FAILED.", 
               sStatements.c_str()); 
        return QueryHitsPtr();
    }
    return search(stat, queryParser);
}

QueryHitsPtr IndexSearcher::search(const Statement& statement,
                                   QueryParser& queryParser)
{
    ParameterClausePtr pParamClause = statement.getParameterClause();
    SortClausePtr pSortClause = statement.getSortClause();
    QueryClausePtr pQueryClause = statement.getQueryClause();
    FilterClausePtr pFilterClause = statement.getFilterClause();
    QueryTracerPtr pTracer;
    if (pParamClause.isNotNull())
    {
        if (pParamClause->hasTrace())
        {
            pTracer = new QueryTracer("searcher", pParamClause->getTraceLevel());
        }

        if ((pParamClause->getQueryPhase() == PHASE_RETRIEVE_ALL)
            || (pParamClause->getQueryPhase() == PHASE_RETRIEVE_DOCID))
        {
            return doSearch(pQueryClause, pSortClause, pFilterClause, 
                            pParamClause, pTracer, queryParser);
        }
        else if (pParamClause->getQueryPhase() == PHASE_RETRIEVE_DOC)
        {
            DocSelectClausePtr pDocSelectClause = statement.getDocSelectClause();
            return doRetrieveDoc(pDocSelectClause, pTracer);
        }
        else 
        {
            FX_STREAM_LOG(WARN) << "Invalid search phase: [" 
                                << pParamClause->getQueryPhase() << "]"
                                << FIRTEX_ENDL;
            return QueryHitsPtr();
        }
    }
    else 
    {
        return doSearch(pQueryClause, pSortClause, pFilterClause, 
                        pParamClause, pTracer, queryParser);
    }
}

QueryHitsPtr IndexSearcher::search(const QueryPtr& pQuery,
                                   const SorterPtr& pSort,
                                   const FilterPtr& pFilter,
                                   QueryTracerPtr& pTracer,
                                   size_t nStart, size_t nTopDocs,
                                   phase_t phase)
{
    FIRTEX_ASSERT2(pQuery.isNotNull());
    if (nStart > nTopDocs)
    {
        nStart = 0;
    }

    QueryExecutorPtr pQueryExecutor = pQuery->createExecutor(
            m_pIndexReader, m_pFeatureProvider, m_pPool);
    if (pQueryExecutor.isNull())
    {
        return QueryHitsPtr();
    }

    FX_QUERY_TRACE(DEBUG, pTracer, "Begin search");

    HitCollectorPtr pHitCollector;
    if (pSort.isNull())
    {
        pHitCollector.assign(new SimpleHitCollector(nTopDocs));
    }
    else
    {
        pHitCollector.assign(new SortedHitCollector(pSort, nTopDocs));
    }
    if (pFilter.isNotNull())
    {
        pHitCollector->setFilter(pFilter);
    }

    const ScorerProfile* pScorerProfile = 
        ScorerProfileFactory::instance()->getProfile("");
    ScorerPtr pScorer = pScorerProfile->createScorer();
    pQueryExecutor->execute(pHitCollector, pScorer, pTracer);
    size_t nTotalHits = pHitCollector->totalHits();
    if (nTotalHits == 0)
    {
        FX_QUERY_TRACE(DEBUG, pTracer, "End search: no results");
        return QueryHitsPtr();
    }

    QueryHitsPtr pHits;
    HitQueuePtr pHitQu = pHitCollector->getHitQueue();
    if (pHitQu.isNotNull() && pHitQu->size() > 0)
    {
        pHits.assign(new QueryHits());
        if (phase == PHASE_RETRIEVE_DOCID)
        {
            pHits->init(pHitQu, nStart);
        }
        else
        {
            pHits->init(pHitQu, nStart);
        }
        pHits->setTotalHits(nTotalHits);
        pHits->setEncoding(m_pIndexReader->getEncoding());
        pHits->setTracer(pTracer);
    }
    FX_QUERY_TRACE(DEBUG, pTracer, "End search");
    return pHits;
}

QueryHitsPtr IndexSearcher::doRetrieveDoc(
        const DocSelectClausePtr& pDocClause,
        QueryTracerPtr& pTracer)
{
    if (pDocClause.isNull())
    {
        return QueryHitsPtr();
    }

    const string& sDocs = pDocClause->getValue();
    
    QueryHitsPtr pHits(new QueryHits());
    pHits->setTracer(pTracer);
    pHits->init(sDocs);
    pHits->setEncoding(m_pIndexReader->getEncoding());
    return pHits;
}

QueryHitsPtr IndexSearcher::doSearch(const QueryClausePtr& pQueryClause, 
                                     const SortClausePtr& pSortClause, 
                                     const FilterClausePtr& pFilterClause, 
                                     const ParameterClausePtr& pParamClause, 
                                     QueryTracerPtr& pTracer,
                                     QueryParser& queryParser)
{
    if (pQueryClause.isNull())
    {
        return QueryHitsPtr();
    }

    try
    {
        QueryPtr pQuery = queryParser.parse(pQueryClause->getValue());
        if (pQuery.isNull())
        {
            return QueryHitsPtr();
        }

        size_t nStart = 0;
        size_t nTop = ParameterClause::DEFAULT_TOPDOCS;
        if (pParamClause.isNotNull())
        {
            nStart = pParamClause->getStartDoc();
            nTop = pParamClause->getTopDocs();
        }

        SorterPtr pSort;
        if (pSortClause.isNotNull())
        {
            pSort = new Sorter();
            if (!pSort->init(m_pIndexReader, pSortClause))
            {
                FX_LOG(ERROR, "Create sort expression: [%s] FAILED, "
                       "sort by relevance.", pSortClause->getValue().c_str());
                return QueryHitsPtr();
            }
        }

        FilterPtr pFilter;
        if (pFilterClause.isNotNull())
        {
            pFilter = new Filter();
            if (!pFilter->init(m_pIndexReader, pFilterClause))
            {
                FX_LOG(ERROR, "Create filter expression evaluator: [%s] FAILED.",
                       pFilterClause->getFilterString().c_str());
                return QueryHitsPtr();
            }
        }
        return search(pQuery, pSort, pFilter, pTracer, nStart, nTop, 
                      pParamClause.isNull() ? PHASE_RETRIEVE_ALL 
                      : pParamClause->getQueryPhase());
    }
    catch (const FirteXException& e)
    {
        FX_STREAM_LOG(ERROR) << "Parse query statement FAILED: ["
                             << pQueryClause->getValue() 
                             << ", reason: " << e.what() << FIRTEX_ENDL;
    }
    return QueryHitsPtr();
}


StoredFieldsReaderPtr IndexSearcher::createStoredFieldsReader() const
{
    return m_pIndexReader->createStoredFieldsReader();
}

FX_NS_END
