#include "firtex/search/ResultCollectorImpl.h"
#include "firtex/utility/NumberFormatter.h"

using namespace std;
FX_NS_USE(index);
FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_LOGGER(search, ResultCollectorImpl);

ResultCollectorImpl::ResultCollectorImpl(size_t nCapacity, 
        CompareField comp, const std::string& sField)
    : m_pResultQueue(NULL)
    , m_nCapacity(nCapacity)
{
    switch (comp)
    {
    case COMP_SCORE:
        m_pResultQueue = new ResultDocQueue(nCapacity, false, 
                NULL, new ScoreComparator);
        break;

    case COMP_DOCID:
        m_pResultQueue = new ResultDocQueue(nCapacity, false,
                NULL, new DocIdComparator);
        break;

    case COMP_FIELD_INT32:
        m_pResultQueue = new ResultDocQueue(nCapacity, false,
                NULL, new Int32FieldComparator(sField));
        break;

    case COMP_FIELD_UINT32:
         m_pResultQueue = new ResultDocQueue(nCapacity, false,
                 NULL, new UInt32FieldComparator(sField));
        break;

    case COMP_FIELD_INT64:
        m_pResultQueue = new ResultDocQueue(nCapacity, false,
                NULL, new Int64FieldComparator(sField));
        break;

    case COMP_FIELD_UINT64:
        m_pResultQueue = new ResultDocQueue(nCapacity, false, 
                NULL, new UInt64FieldComparator(sField));
        break;

    case COMP_FIELD_FLOAT:
        m_pResultQueue = new ResultDocQueue(nCapacity, false,
                NULL, new FloatFieldComparator(sField));
        break;

    case COMP_FIELD_DOUBLE:
        m_pResultQueue = new ResultDocQueue(nCapacity, false,
                NULL, new DoubleFieldComparator(sField));
        break;
    }
}

ResultCollectorImpl::~ResultCollectorImpl() 
{
    delete m_pResultQueue;
    m_pResultQueue = NULL;
}

void ResultCollectorImpl::collect(const QueryResult& queryResults)
{
    if (queryResults.hasError())
    {
        m_result.setError(m_result.getErrorMsg() + queryResults.getErrorMsg());
    }
    else
    {
        m_result.setTotalHits(m_result.getTotalHits() + queryResults.getTotalHits());
        if (queryResults.getTimeCost() > m_result.getTimeCost())
        {
            m_result.setTimeCost(queryResults.getTimeCost());
        }

        shardid_t shardId = -1;
        QueryResult::Iterator it = queryResults.iterator();
        while (it.hasNext())
        {
            const ResultDocPtr& pDoc = it.next();
            m_pResultQueue->insert(pDoc);
            shardId = pDoc->getShardId();
        }
        
        const QueryTracerPtr& pTracer = queryResults.getTracer();
        if (pTracer)
        {
            if (!m_result.getTracer())
            {
                QueryTracerPtr pTmp(new QueryTracer("proxy", pTracer->getLevel()));
                m_result.setTracer(pTmp);
            }
            QueryTracerPtr& pResTracer = m_result.getTracer();
            string str;
            NumberFormatter::append(str, shardId);
            pResTracer->addChildTracer(pTracer)->prependPath(str);
        }
    }
}

const QueryResult& ResultCollectorImpl::getResult()
{
    while (m_pResultQueue->size() > 0)
    {
        ResultDocPtr pDoc = m_pResultQueue->pop();
        m_result.addDoc(pDoc);
    }
    return m_result;
}

size_t ResultCollectorImpl::capacity() const
{
    return m_nCapacity;
}

void ResultCollectorImpl::clear()
{
    if (m_pResultQueue)
    {
        m_pResultQueue->clear();
    }
    m_result.clear();
}

FX_NS_END

