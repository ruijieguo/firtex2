#include "firtex/search/QueryHits.h"
#include "firtex/search/IndexSearcher.h"
#include "firtex/utility/StringTokenizer.h"

FX_NS_USE(utility);
FX_NS_USE(document);
FX_NS_USE(index);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, QueryHits);

//////////////////////////////////////////////////////////////////////////
//Document::Iterator		
QueryHits::Iterator::Iterator(const QueryHits* pHits)
    : m_pQueryHits(pHits)
    , m_nIterator(0)
{
}

QueryHits::Iterator::Iterator(const Iterator& _iter)
    : m_pQueryHits(_iter.m_pQueryHits)
    , m_nIterator(_iter.m_nIterator)
{
}

QueryHits::Iterator::~Iterator(void)
{
    m_pQueryHits = NULL;
    m_nIterator = 0;
}

void QueryHits::Iterator::reset()
{
    m_nIterator = 0;
}

bool QueryHits::Iterator::hasNext() const
{
    return (m_nIterator < m_pQueryHits->m_hitDocs.size());
}

const QueryHits::HitDoc& QueryHits::Iterator::next()
{
    return *(m_pQueryHits->m_hitDocs[m_nIterator++]);
}

size_t QueryHits::Iterator::size()const
{
    return m_pQueryHits->size();
}

////////////////////////////////////////////
//
QueryHits::QueryHits()
    : m_nTotalHits(0)
{
}

QueryHits::~QueryHits() 
{
    clear();
}

void QueryHits::init(HitQueuePtr& hq, size_t nStart)
{
    if (nStart >= hq->size())
    {
        return;
    }

    m_hitDocs.resize(hq->size() - nStart);

    ScoredDoc* pScoredDoc = hq->pop();
    int32_t i = (int32_t)m_hitDocs.size() - 1;
    while (pScoredDoc && i >= 0)
    {
        m_hitDocs[i--] = new HitDoc(pScoredDoc->getDocId(),
                pScoredDoc->getScore());
        hq->freeScoredDoc(pScoredDoc);
        pScoredDoc = hq->pop();
    }
}

void QueryHits::init(const std::string& sHits)
{
    StringTokenizer st(sHits, ",", StringTokenizer::TOKEN_TRIM | 
                       StringTokenizer::TOKEN_IGNORE_EMPTY);
    if (st.getNumTokens() > 0)
    {
        m_hitDocs.resize(st.getNumTokens());

        for (size_t i = 0; i < m_hitDocs.size(); ++i)
        {
            int32_t docId;
            bool ret = NumberParser::tryParseInt32(st[i], docId);
            if (!ret)
            {
                FIRTEX_THROW(BadParameterException, _T("Invalid docId: [%s]"),
                        st[i].c_str());
            }
            
            m_hitDocs[i] = new HitDoc(docId, (score_t)0.0);
        }
    }
    setTotalHits((uint64_t) m_hitDocs.size());
}

void QueryHits::clear()
{
    for (size_t i = 0; i < m_hitDocs.size(); i++)
    {
        delete m_hitDocs[i];
    }
    m_hitDocs.clear();
}

QueryHits::Iterator QueryHits::iterator() const
{
    return QueryHits::Iterator(this);
}

FX_NS_END

