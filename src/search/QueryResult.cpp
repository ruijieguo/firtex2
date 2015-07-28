#include "firtex/search/QueryResult.h"
#include "firtex/search/QueryHits.h"
#include "firtex/utility/TimeProbe.h"

FX_NS_USE(index);
FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, QueryResult);

QueryResult::QueryResult() 
    : m_nTotalHits(0)
    , m_nTimeCost(0)
    , m_bHasShardId(false)
    , m_bHasDocId(true)
    , m_bHasScore(true)
    , m_bHasFields(true)
    , m_bHasError(false)
{
}

QueryResult::~QueryResult() 
{
    clear();
}

void QueryResult::init(const IndexReaderPtr& pIndexReader,
                       const QueryHits& hits)
{
    FieldSelector selector(pIndexReader->getDocSchema(), true, false);
    init(selector, pIndexReader, hits);
}

void QueryResult::init(const FieldSelector& selector, 
                       const IndexReaderPtr& pIndexReader,
                       const QueryHits& hits)
{
    StoredFieldsReaderPtr pStoredFieldsReader = pIndexReader->createStoredFieldsReader();
    if (!pStoredFieldsReader)
    {
        return;
    }

    TimeProbe probe;
    probe.start();

    m_docs.reserve(hits.size());
    setTracer(hits.getTracer());

    setTotalHits(hits.getTotalHits());

    QueryHits::Iterator it = hits.iterator();
    while (it.hasNext())
    {
        const QueryHits::HitDoc& hitDoc = it.next();
        
        ResultDocPtr pResDoc(new ResultDoc(hitDoc.getDocId(), 
                        hitDoc.getScore(), selector.size()));
        addDoc(pResDoc);

        pStoredFieldsReader->getDocument(selector, *pResDoc);
    }

    probe.stop();
    FX_QUERY_TRACE(INFO, getTracer(), "fetch field time [%d] ms",
                   (int32_t)probe.elapsed() / 1000);
}

void QueryResult::addField(const std::string& sFieldName, 
                           const std::string& sFieldValue)
{
    for (size_t i = 0; i < m_docs.size(); ++i)
    {
        m_docs[i]->addField(sFieldName, sFieldValue);
    }
}

void QueryResult::setShardId(shardid_t shardId)
{
    for (size_t i = 0; i < m_docs.size(); ++i)
    {
        m_docs[i]->setShardId(shardId);
    }
}

bool QueryResult::hasField(const std::string& sFieldName) const
{
    for (size_t i = 0; i < m_docs.size(); ++i)
    {
        if (m_docs[i]->hasField(sFieldName))
        {
            return true;
        }
    }
    return false;
}

void QueryResult::clear()
{
    m_pQueryTracer.reset();
    m_docs.clear();
    m_sErrorMsg.clear();
    m_nTotalHits = 0;
    m_nTimeCost = 0;

    m_bHasShardId = false;
    m_bHasDocId = true;
    m_bHasScore = true;
    m_bHasFields = true;
    m_bHasError = false;
}

FX_NS_END

