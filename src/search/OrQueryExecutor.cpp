#include "firtex/search/OrQueryExecutor.h"

FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, OrQueryExecutor);

OrQueryExecutor::OrQueryExecutor(FeatureProviderPtr& pProvider,
                                 size_t nMinShouldMatch,
                                 PoolPtr& pPool)
    : QueryExecutor(pProvider, pPool)
    , m_pExeQueue(NULL)
    , m_nMinShouldMatch(nMinShouldMatch)
    , m_lastDocId(0)
{
}

OrQueryExecutor::~OrQueryExecutor() 
{
    delete m_pExeQueue;
    m_pExeQueue = NULL;
}

size_t OrQueryExecutor::advance(MatchedDocSet& matchedDocs)
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

docid_t OrQueryExecutor::skipTo(docid_t docId)
{
    if (m_pExeQueue == NULL)
    {
        initExecutorQueue();
    }

    if (m_pExeQueue->size() < m_nMinShouldMatch)
    {
        return INVALID_DOCID;
    }
    
    ExecutorEntry* pTop = NULL;
    do 
    {
        pTop = m_pExeQueue->top();
        docid_t nextDocId = pTop->executor->skipTo(docId);
        if (nextDocId != INVALID_DOCID) 
        {
            pTop->docId = nextDocId;
            m_pExeQueue->adjustTop();
        } 
        else 
        {
            pTop = m_pExeQueue->pop();
            delete pTop;
            if (m_pExeQueue->size() < m_nMinShouldMatch)
            {
                return INVALID_DOCID;
            }
        }
        pTop = m_pExeQueue->top();
    } while (docId > pTop->docId);

    return pTop->docId;
}

void OrQueryExecutor::currentMatchedDoc(MatchedDoc& matchedDoc)
{
    doGetMatchedDoc(matchedDoc);
}

void OrQueryExecutor::extractFeature(QueryFeature& queryFeature) const
{
    TermQueryFeature& termQueryFeature = queryFeature.next();
    termQueryFeature.setQueryType(TermQueryFeature::QT_OR);
    termQueryFeature.setNumSuccessors(size() - 1);

    for (ExecutorVector::const_iterator it = m_executors.begin();
         it != m_executors.end(); ++it)
    {
        (*it)->extractFeature(queryFeature);
    }
}

void OrQueryExecutor::initExecutorQueue()
{
    m_pExeQueue = new ExecutorQueue(m_executors.size());
    for (ExecutorVector::iterator it = m_executors.begin();
         it != m_executors.end(); ++it)
    {
        docid_t docId = (*it)->skipTo(0);
        if (docId != INVALID_DOCID)
        {
            ExecutorEntry* pEntry = new ExecutorEntry();
            pEntry->docId = docId;
            pEntry->executor = *it;
            m_pExeQueue->insert(pEntry);
        }
    }
}

size_t OrQueryExecutor::size() const
{
    size_t size = 1; //plus a root myself
    for (ExecutorVector::const_iterator it = m_executors.begin();
         it != m_executors.end(); ++it)
    {
        size += (*it)->size();
    }
    return size;
}

FX_NS_END

