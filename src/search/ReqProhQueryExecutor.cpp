#include "firtex/search/ReqProhQueryExecutor.h"

FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, ReqProhQueryExecutor);

ReqProhQueryExecutor::ReqProhQueryExecutor(
        FeatureProviderPtr& pProvider,
        PoolPtr& pPool, 
        const QueryExecutorPtr& pReqExe,
        const QueryExecutorPtr& pProhExe) 
    : QueryExecutor(pProvider, pPool)
    , m_pReqExecutor(pReqExe)
    , m_pProhExecutor(pProhExe)
    , m_lastDocId(0)
{
}

ReqProhQueryExecutor::~ReqProhQueryExecutor() 
{
}

size_t ReqProhQueryExecutor::advance(MatchedDocSet& matchedDocs)
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

docid_t ReqProhQueryExecutor::skipTo(docid_t docId)
{
    if (!m_pReqExecutor)
    {		
        return INVALID_DOCID;
    }

    if (!m_pProhExecutor)
    {
        docId = m_pReqExecutor->skipTo(docId);
    }
    else 
    {
        --docId;
        do
        {
            docId = m_pReqExecutor->skipTo(++docId);
        } while (docId == m_pProhExecutor->skipTo(docId) 
                && docId != INVALID_DOCID);
    }

    if (INVALID_DOCID != docId)
    {
        return docId;
    }
    return INVALID_DOCID;
}

void ReqProhQueryExecutor::currentMatchedDoc(MatchedDoc& matchedDoc)
{
    doGetMatchedDoc(matchedDoc);
}

void ReqProhQueryExecutor::extractFeature(QueryFeature& queryFeature) const
{
    TermQueryFeature& termQueryFeature = queryFeature.next();
    termQueryFeature.setQueryType(TermQueryFeature::QT_REQPROH);
    termQueryFeature.setNumSuccessors(m_pReqExecutor->size());

    m_pReqExecutor->extractFeature(queryFeature);
}

size_t ReqProhQueryExecutor::size() const
{
    size_t size = m_pReqExecutor->size();
    if (m_pProhExecutor)
    {
        size += m_pProhExecutor->size();
    }
    return size + 1;
}

FX_NS_END

