#include "firtex/search/BM25Scorer.h"

FX_NS_USE(index);
FX_NS_DEF(search);

///////////////////////////////////////////////
SETUP_STREAM_LOGGER_EMBED(search, BM25Scorer, Weight);
SETUP_STREAM_LOGGER_EMBED(search, BM25Scorer, DocScorer);

SETUP_STREAM_LOGGER(search, BM25Scorer);

score_t BM25Scorer::K1 = 2.0;
score_t BM25Scorer::B = 0.75;

///////////////////////////////////////////////
BM25Scorer::BM25Scorer() 
{
}

BM25Scorer::~BM25Scorer() 
{
    clear();
}

void BM25Scorer::beginQuery(const FeatureProviderPtr& pProvider,
                            const QueryFeature& queryFeature)
{
    m_pFeatureProvider = pProvider;

    FIRTEX_ASSERT2(queryFeature.size() > 0);
    setupDocScorers(queryFeature);
}
    
void BM25Scorer::score(MatchedDocSet& matchedDocs)
{
    for (DocScorerVect::const_iterator it = m_docScorers.begin();
         it != m_docScorers.end(); ++it)
    {
        (*it)->score(matchedDocs);
    }
}

void BM25Scorer::endQuery()
{
    clear();
}

void BM25Scorer::setupDocScorers(const QueryFeature& queryFeature)
{
    const IndexFeature& indexFeature = m_pFeatureProvider->getIndexFeature();

    for (size_t i = 0; i < queryFeature.size(); ++i)
    {
        const TermQueryFeature& tqFeature = queryFeature[i];

        FIRTEX_ASSERT2(tqFeature.getNumSuccessors() < queryFeature.size() - i);
        Weight w;
        w.init(indexFeature, &tqFeature);
        
        DocScorer* pDocScore = new DocScorer(indexFeature, w, 
                &tqFeature, (uint32_t)i);
        m_docScorers.push_back(pDocScore);
    }
}

void BM25Scorer::clear()
{
    for (DocScorerVect::iterator it = m_docScorers.begin();
         it != m_docScorers.end(); ++it)
    {
        delete *it;
    }
    m_docScorers.clear();
}

FX_NS_END

