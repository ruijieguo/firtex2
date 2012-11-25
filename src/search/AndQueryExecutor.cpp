#include "firtex/search/AndQueryExecutor.h"
#include "firtex/search/TermQueryFeature.h"

FX_NS_USE(utility);
FX_NS_USE(index);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, AndQueryExecutor);

AndQueryExecutor::AndQueryExecutor(FeatureProviderPtr& pProvider,
                                   PoolPtr& pPool)
    : QueryExecutor(pProvider, pPool)
    , m_lastDocId(0)
{
}

AndQueryExecutor::~AndQueryExecutor() 
{
}

size_t AndQueryExecutor::advance(MatchedDocSet& matchedDocs)
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

docid_t AndQueryExecutor::skipTo(docid_t docId)
{
    size_t nTotalPost = m_executors.size();
    docid_t curDocId = docId;
    size_t cursor = 0;
    size_t nMatchedCnt = 0;
    do 
    {
        docid_t nextDocId = m_executors[cursor]->skipTo(curDocId);
        if (nextDocId != curDocId) //not matched
        {
            curDocId = nextDocId;
            nMatchedCnt = 1;
        } 
        else //matched
        {
            ++nMatchedCnt;
        }
        ++cursor; //move to next posting
        cursor %= nTotalPost; //reset to first posting if reach the last posting
    } while (INVALID_DOCID != curDocId && nMatchedCnt < nTotalPost);

    if (INVALID_DOCID != curDocId)
    {
        return curDocId;
    }
    return INVALID_DOCID;
}

void AndQueryExecutor::currentMatchedDoc(MatchedDoc& matchedDoc)
{
    doGetMatchedDoc(matchedDoc);
}

void AndQueryExecutor::extractFeature(QueryFeature& queryFeature) const
{
    TermQueryFeature& termQueryFeature = queryFeature.next();
    termQueryFeature.setQueryType(TermQueryFeature::QT_AND);
    termQueryFeature.setNumSuccessors(size() - 1);

    for (ExecutorVector::const_iterator it = m_executors.begin();
         it != m_executors.end(); ++it)
    {
        (*it)->extractFeature(queryFeature);
    }
}

size_t AndQueryExecutor::size() const
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

