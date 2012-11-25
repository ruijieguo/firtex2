#include "firtex/search/TermQueryExecutor.h"
#include "firtex/search/TermQueryFeature.h"

FX_NS_USE(index);
FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, TermQueryExecutor);

TermQueryExecutor::TermQueryExecutor(const TermQuery* pTermQuery,
                                     TermPostingIteratorPtr& pPostingIterator, 
                                     FeatureProviderPtr& pProvider,
                                     PoolPtr& pPool)
    : QueryExecutor(pProvider, pPool)
    , m_pQuery(pTermQuery)
    , m_pPostingIterator(pPostingIterator)
{
}

TermQueryExecutor::~TermQueryExecutor() 
{
}

size_t TermQueryExecutor::advance(MatchedDocSet& matchedDocs)
{
    docid_t* docs = NULL;
    tf_t* freqs = NULL;
    size_t num = (size_t)m_pPostingIterator->nextDocs(docs, freqs);
    matchedDocs.resize((size_t)num);
    
    FX_TRACE("doc count: [%u]", (uint32_t)num);

    size_t nNumDocs = 0;
    for (size_t i = 0; i < num; ++i)
    {
        if (docs[i] != INVALID_DOCID)
        {
            MatchedDoc& matchedDoc = matchedDocs[i];
            matchedDoc.setDocId(docs[i]);
            TermScoreFeature& termScoreFeature =
                matchedDoc.scoreFeature().nextTermFeature();
            termScoreFeature.setTermFreq((float)freqs[i]);
            FX_TRACE("doc id: [%d], tf: [%d]", docs[i], freqs[i]);
            ++nNumDocs;
        }
    }
    return nNumDocs;
}

docid_t TermQueryExecutor::skipTo(docid_t docId)
{
    return m_pPostingIterator->skipTo(docId);
}

void TermQueryExecutor::currentMatchedDoc(MatchedDoc& matchedDoc)
{
    docid_t curDocId = m_pPostingIterator->doc();
    TermScoreFeature& termScoreFeature = matchedDoc.scoreFeature().nextTermFeature();
    if (matchedDoc.getDocId() == curDocId)
    {
        termScoreFeature.setTermFreq((float)m_pPostingIterator->freq());
        FX_TRACE("Extract term score feature of term query: %d, tf: %f",
                        matchedDoc.getDocId(), termScoreFeature.getTermFreq());
    }
    else
    {
        termScoreFeature.setTermFreq(0.0);
        FX_TRACE("Extract term score feature of term query: %d, tf: %f",
                        matchedDoc.getDocId(), termScoreFeature.getTermFreq());
    }
}

void TermQueryExecutor::extractFeature(QueryFeature& queryFeature) const
{
    TermQueryFeature& termQueryFeature = queryFeature.next();
    termQueryFeature.setTerm(m_pQuery->getTerm());
    termQueryFeature.setBoost(m_pQuery->getBoost());
    const TermMeta& tm = m_pPostingIterator->getTermMeta();
    termQueryFeature.setTermMeta(&tm);
    termQueryFeature.setQueryType(TermQueryFeature::QT_TERM);
}

FX_NS_END

