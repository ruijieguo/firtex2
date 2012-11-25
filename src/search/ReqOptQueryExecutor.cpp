#include "firtex/search/ReqOptQueryExecutor.h"

FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, ReqOptQueryExecutor);

ReqOptQueryExecutor::ReqOptQueryExecutor(
        FeatureProviderPtr& pProvider,
        PoolPtr& pPool,
        const QueryExecutorPtr& pReqExe,
        const QueryExecutorPtr& pOptExe)
    : QueryExecutor(pProvider, pPool)
    , m_pReqExecutor(pReqExe)
    , m_pOptExecutor(pOptExe)
    , m_lastDocId(0)
{
}

ReqOptQueryExecutor::~ReqOptQueryExecutor() 
{
}

size_t ReqOptQueryExecutor::advance(MatchedDocSet& matchedDocs)
{
    size_t nMatchedDocs = 0;
    docid_t curDocId = m_lastDocId;
    matchedDocs.resize(matchedDocs.capacity());
    while ((curDocId != INVALID_DOCID) && (nMatchedDocs < matchedDocs.size()))
    {
        curDocId = skipTo(curDocId);
        if (curDocId != INVALID_DOCID)
        {
            MatchedDoc& md = matchedDocs[nMatchedDocs];
            md.setDocId(curDocId);
            doGetMatchedDoc(md);
            ++curDocId;
            ++nMatchedDocs;
        }
    }
    matchedDocs.resize(nMatchedDocs);
    m_lastDocId = curDocId;
    return nMatchedDocs;
}

docid_t ReqOptQueryExecutor::skipTo(docid_t docId)
{
    return m_pReqExecutor->skipTo(docId);
}

void ReqOptQueryExecutor::currentMatchedDoc(MatchedDoc& matchedDoc)
{
    doGetMatchedDoc(matchedDoc);
}

void ReqOptQueryExecutor::extractFeature(QueryFeature& queryFeature) const
{
    TermQueryFeature& termQueryFeature = queryFeature.next();
    termQueryFeature.setQueryType(TermQueryFeature::QT_REQOPT);
    termQueryFeature.setNumSuccessors(size() - 1);

    m_pReqExecutor->extractFeature(queryFeature);
    if (m_pOptExecutor.isNotNull())
    {
        m_pOptExecutor->extractFeature(queryFeature);
    }
}

size_t ReqOptQueryExecutor::size() const
{
    size_t size = m_pReqExecutor->size();
    if (m_pOptExecutor.isNotNull())
    {
        size += m_pOptExecutor->size();
    }
    return size + 1;
}

FX_NS_END

