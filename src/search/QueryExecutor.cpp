#include "firtex/search/QueryExecutor.h"

FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, QueryExecutor);

QueryExecutor::QueryExecutor()
{
}

QueryExecutor::QueryExecutor(FeatureProviderPtr& pProvider,
                             PoolPtr& pPool)
    : m_pFeatureProvider(pProvider)
    , m_pPool(pPool)
{
}

QueryExecutor::~QueryExecutor() 
{
}

void QueryExecutor::execute(HitCollectorPtr& pCollector,
                            ScorerPtr& pScorer,
                            const QueryTracerPtr& pTracer)
{
    //TODO: allocate from mempool
    MatchedDocSet machedDocs(m_pPool, DEFAULT_MATCHED_DOC_BUFFER_SIZE, size());
    machedDocs.setTracer(pTracer);

    QueryFeature queryFeature;
    queryFeature.reserve(size());
    extractFeature(queryFeature);

    pScorer->beginQuery(m_pFeatureProvider, queryFeature);
    while (advance(machedDocs) > 0)
    {
        FX_TRACE("Matched doc count: [%u]", (uint32_t)machedDocs.size());
        pScorer->score(machedDocs);
        pCollector->collect(machedDocs);
        machedDocs.reset();
    }
    pScorer->endQuery();
}

FX_NS_END

