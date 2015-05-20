#include "firtex/search/BM25Scorer.h"

FX_NS_USE(index);
FX_NS_DEF(search);

///////////////////////////////////////////////
SETUP_STREAM_LOGGER(search, BM25Scorer);

///////////////////////////////////////////////
BM25Scorer::BM25Scorer() 
{
}

BM25Scorer::~BM25Scorer() 
{
}

void BM25Scorer::beginQuery(const FeatureProviderPtr& pProvider,
                            const QueryFeature& queryFeature)
{
    FIRTEX_ASSERT2(queryFeature.size() > 0);
    m_docScorer.init(pProvider.get(), queryFeature);
}
    
void BM25Scorer::score(MatchedDocSet& matchedDocs)
{
    m_docScorer.score(matchedDocs);
}

void BM25Scorer::endQuery()
{
    m_docScorer.clear();
}

FX_NS_END

