#include "firtex/search/PhraseQueryExecutor.h"
#include "firtex/search/TermQueryFeature.h"

FX_NS_USE(utility);
FX_NS_USE(index);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, PhraseQueryExecutor);

PhraseQueryExecutor::PhraseQueryExecutor(const PhraseQuery* pQuery,
                                         FeatureProviderPtr& pProvider,
                                         PoolPtr& pPool)
    : QueryExecutor(pProvider, pPool)
    , m_pQuery(pQuery)
    , m_nSlop(0)
    , m_lastDocId(0)
{
}

PhraseQueryExecutor::~PhraseQueryExecutor() 
{
}

size_t PhraseQueryExecutor::advance(MatchedDocSet& matchedDocs)
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

docid_t PhraseQueryExecutor::skipTo(docid_t docId)
{
    size_t nTotalPost = m_postings.size();
    docid_t curDocId = docId;
    do 
    {
        size_t cursor = 0;
        size_t nMatchedCnt = 0;
        do 
        {
            docid_t nextDocId = m_postings[cursor]->skipTo(curDocId);
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

        if (INVALID_DOCID == curDocId)
        {
            return INVALID_DOCID; // no matched doc
        }
        else 
        {
            FX_TRACE("Matched doc: %d", curDocId);
            float tf = phraseFreq();
            if (tf > 0.0) // match one doc
            {
                m_phraseFreq = tf;
                FX_TRACE("Matched doc: %d, tf: %f", curDocId, tf);
                return curDocId;
            }
            else
            {
                ++curDocId;
            }
        }
    } while(true);
    
    return INVALID_DOCID;
}

void PhraseQueryExecutor::currentMatchedDoc(MatchedDoc& matchedDoc)
{
    doGetMatchedDoc(matchedDoc);
}

void PhraseQueryExecutor::extractFeature(QueryFeature& queryFeature) const
{
    const PhraseQuery::TermVector& terms = m_pQuery->getTerms();
    FIRTEX_ASSERT2(terms.size() == m_postings.size());

    TermQueryFeature& termQueryFeature = queryFeature.next();
    termQueryFeature.setBoost(m_pQuery->getBoost());
    termQueryFeature.setQueryType(TermQueryFeature::QT_PHRASE);
    termQueryFeature.setTerm(terms[0]);
    termQueryFeature.setNumSuccessors(size() - 1);

    PhraseQuery::TermVector::const_iterator termIt = terms.begin();
    for (PostingVector::const_iterator it = m_postings.begin();
         it != m_postings.end(); ++it, ++termIt)
    {
        TermQueryFeature& termQueryFeature = queryFeature.next();
        termQueryFeature.setTerm(*termIt);
        termQueryFeature.setBoost(m_pQuery->getBoost());
        const TermMeta& tm = (*it)->getTermMeta();
        termQueryFeature.setTermMeta(&tm);
        termQueryFeature.setQueryType(TermQueryFeature::QT_TERM);
    }
}

size_t PhraseQueryExecutor::size() const
{
    return m_postings.size() + 1;
}

FX_NS_END

