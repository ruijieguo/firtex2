#include "firtex/search/AnyQueryExecutor.h"

FX_NS_USE(index);
FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, AnyQueryExecutor);

AnyQueryExecutor::AnyQueryExecutor(IndexReaderPtr& pIndexReader, 
                                   FeatureProviderPtr& pProvider,
                                   PoolPtr& pPool)
    : QueryExecutor(pProvider, pPool)
    , m_pIndexReader(pIndexReader)
    , m_lastDocId(INVALID_DOCID)
{
}

AnyQueryExecutor::~AnyQueryExecutor() 
{
}

size_t AnyQueryExecutor::advance(MatchedDocSet& matchedDocs)
{
    docid_t maxDocId = (docid_t)(m_pIndexReader->getMaxDocs() - 1);
    const size_t nBulkDocs = 128;
    matchedDocs.resize(nBulkDocs);

    size_t i;
    docid_t nextDocId;
    docid_t lastDocId = m_lastDocId;
    size_t totalDocs = 0;
    for (i = 0, nextDocId = m_lastDocId + 1;
         i < nBulkDocs && nextDocId <= maxDocId; ++i, ++nextDocId)
    {
        if (!m_pIndexReader->isDeleted(nextDocId))
        {
            MatchedDoc& matchedDoc = matchedDocs[totalDocs];
            matchedDoc.setDocId(nextDocId);
            lastDocId = nextDocId;
            totalDocs++;
        }
    }

    if (totalDocs > 0)
    {
        matchedDocs.resize(totalDocs);
    }
    m_lastDocId = lastDocId;

    return totalDocs;
}

docid_t AnyQueryExecutor::skipTo(docid_t docId)
{
    docid_t newDocId = docId > (m_lastDocId + 1) ? docId : m_lastDocId + 1;
    docid_t maxDocId = (m_pIndexReader->getMaxDocs() - 1);

    while (newDocId <= maxDocId && m_pIndexReader->isDeleted(newDocId))
    {
        ++newDocId;
    }

    if (newDocId > maxDocId)
    {
        return INVALID_DOCID;
    }
    m_lastDocId = newDocId;
    return newDocId;
}

void AnyQueryExecutor::currentMatchedDoc(MatchedDoc& matchedDoc)
{
    //Nothing to do
}

void AnyQueryExecutor::extractFeature(QueryFeature& queryFeature) const
{
    //Nothing to do
}

FX_NS_END

