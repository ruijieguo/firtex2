#include "SearchShardsBase.h"
#include "firtex/search/XMLResultFormatter.h"
#include "firtex/search/IndexSearcher.h"
#include "firtex/search/ResultCollectorImpl.h"
#include "firtex/search/XMLResultParser.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/TimeProbe.h"

#ifdef HAVE_THRIFT
#include "../thrift/ThriftSearchServiceHelper.h"
#endif 

#include <sstream>

#define SEARCH_LATENCY_BEGIN()                  \
    TimeProbe prob;                             \
    prob.start()

#define SEARCH_LATENCY_END(latency)             \
    prob.stop();                                \
    latency = (prob.elapsed() / 1000)

using namespace std;

FX_NS_USE(network);
FX_NS_USE(search);
FX_NS_USE(thread);
FX_NS_USE(utility);
FX_NS_USE(index);

FX_NS_DEF(app);

SETUP_LOGGER(app, SearchShardsBase);

SearchShardsBase::SearchShardsBase(int32_t nTimeOut)
    : m_currentPhase(NONE_PHASE)
    , m_nTimeout(nTimeOut)
    , m_nPhaseOneLatency(0)
    , m_nPhaseOneCollectLatency(0)
    , m_nPhaseTwoLatency(0)
    , m_nPhaseTwoCollectLatency(0)
{
}

SearchShardsBase::~SearchShardsBase()
{
    // stop replica health checker
    m_pHealthChecker->stop();
    m_pCheckThread->join();

    m_deliver.stop();

    for (size_t i = 0; i < m_reqests.size(); ++i)
    {
        delete m_reqests[i];
    }
    m_reqests.clear();

    for (size_t i = 0; i < m_shards.size(); ++i)
    {
        delete m_shards[i];
    }
    m_shards.clear();
}

bool SearchShardsBase::init(const ProxyServerConf::sResource& resConf)
{
    m_shards.reserve(resConf.shards.size());
    m_requestedShards.reserve(resConf.shards.size());

    m_reqests.resize(resConf.shards.size());
    for (size_t i = 0; i < m_reqests.size(); ++i)
    {
        m_reqests[i] = new stringstream();
    }

    m_shardResults.init(resConf.shards.size());

    shardid_t shardId = 0;
    for (ProxyServerConf::ShardList::const_iterator it = resConf.shards.begin();
         it != resConf.shards.end(); ++it, ++shardId)
    {
        const ProxyServerConf::sShard& shard = *it;

        SearchShardBase* pShard = newShard(&m_deliver);
        if (!pShard->init(shard, resConf.load_ballance))
        {
            FX_LOG(ERROR, "Init shard: [%s] FAILED.", shard.name.c_str());
            delete pShard;
            return false;
        }

        pShard->setShardId(shardId);
        m_shards.push_back(pShard);
    }
    
    m_deliver.start();

    m_pHealthChecker = new ReplicaHealthChecker(m_shards);
    m_pCheckThread = new Thread();
    m_pCheckThread->start(*m_pHealthChecker);

    return true;
}

#ifdef HAVE_THRIFT
void SearchShardsBase::search(string& sResult, const QueryCommand& cmd)
{
    string sStat;
    sStat.reserve(256);
    ThriftSearchServiceHelper::queryCommandToStatmentString(sStat, cmd);
    syntaxSearch(sResult, sStat);
}
#endif

void SearchShardsBase::syntaxSearch(string& sResult, const string& sUri)
{
    FX_DEBUG("Handle search request: [%s].", sUri.c_str());
     
    if (m_shards.size() > 1)
    {
        // do two one phase search
        doPhaseOneSearch(sUri);
    }
    else
    {
        // do only one phase search
        m_currentPhase = ONE_PHASE;
        forwardRequest(sUri);
    }

    sResult = m_sResult;
}

void SearchShardsBase::doPhaseOneSearch(const std::string& sUri)
{
    SEARCH_LATENCY_BEGIN();

    m_stat.clearClauses();
    m_stat.fromString(sUri);

    std::string sPhaseParam;
    NumberFormatter::append(sPhaseParam, IndexSearcher::PHASE_RETRIEVE_DOCID);
    m_stat.setParameter("phase", sPhaseParam);

    std::stringstream ss;
    m_stat.toString(ss);

    m_currentPhase = TWO_PHASE_ONE;
    FX_DEBUG("Begin first phase search");

    m_requestedShards.assign(m_shards.begin(), m_shards.end());

    /// Get the first phase result
    for (ShardVector::iterator it = m_requestedShards.begin();
         it != m_requestedShards.end(); ++it)
    {
        SearchShardBase* pShard = *it;
        FX_DEBUG("Search shard: [%s], query: [%s]", 
                 pShard->getName().c_str(), ss.str().c_str());
        pShard->asyncSearch(ss.str());
    }

    m_deliver.waitFinish();
    SEARCH_LATENCY_END(m_nPhaseOneLatency);

    collectResult();
}

void SearchShardsBase::forwardRequest(const std::string& sUri)
{
    m_requestedShards.assign(m_shards.begin(), m_shards.end());
    for (ShardVector::iterator it = m_requestedShards.begin();
         it != m_requestedShards.end(); ++it)
    {
        FX_DEBUG("Send request to shard: [%s]", (*it)->getName().c_str());
        (*it)->asyncSearch(sUri);
    }

    m_deliver.waitFinish();
    collectResult();
}

void SearchShardsBase::collectResult()
{
    switch (m_currentPhase)
    {
    case ONE_PHASE:
        m_sResult = m_shards[0]->getResponse();
        break;
    
    case TWO_PHASE_ONE:
    {
        SEARCH_LATENCY_BEGIN();
        collectPhaseOneResult();
        SEARCH_LATENCY_END(m_nPhaseOneCollectLatency);        
        break;
    }

    case TWO_PHASE_TWO:
        collectPhaseTwoResult();
        break;

    default:
        FIRTEX_ASSERT2(false);
        break;
    }
}

void SearchShardsBase::collectPhaseOneResult()
{
    FX_DEBUG("Collect phase one result");
    size_t nTopDocs = ParameterClause::DEFAULT_TOPDOCS;
    ParameterClausePtr pParamClause = m_stat.getParameterClause();
    if (pParamClause.isNotNull())
    {
        nTopDocs = pParamClause->getTopDocs();
    }
    
    if (m_pResultCollector.isNull() || m_pResultCollector->capacity() != nTopDocs)
    {
        m_pResultCollector.reset(new ResultCollectorImpl(nTopDocs));
    }
    else
    {
        m_pResultCollector->clear();
    }

    for (size_t i = 0; i < m_requestedShards.size(); ++i)
    {
        SearchShardBase* pShard = m_requestedShards[i];
        const string& sResult = pShard->getResponse();
        if (pShard->getStatus() != SearchReplicaBase::EC_OK)
        {
            stringstream ss;
            ss << "Has error on shard: [" << pShard->getName() 
               << "]: " << parseError(sResult);
            setError(ss.str());
            FX_LOG(WARN, ss.str().c_str());
            return;
        }

        XMLResultParser parser;
        QueryResult queryResult;
        parser.parse(sResult, queryResult);
        queryResult.setShardId(pShard->getShardId());

        m_pResultCollector->collect(queryResult);
    }

    doPhaseTwoSearch();
}

void SearchShardsBase::doPhaseTwoSearch()
{
    SEARCH_LATENCY_BEGIN();
    
    const QueryResult& qResult = m_pResultCollector->getResult();
    if (qResult.hasError())
    {
        FX_LOG(WARN, qResult.getErrorMsg().c_str());
        setError(qResult.getErrorMsg());
        SEARCH_LATENCY_END(m_nPhaseTwoLatency);
        return;
    }

    m_stat.setParameter(ParameterClause::PHASE_PREFIX, 
                        IndexSearcher::PHASE_RETRIEVE_DOC_STR);
    size_t i;
    for (i = 0; i < m_reqests.size(); ++i)
    {
        m_reqests[i]->str("");
    }

    FX_DEBUG("Collected doc count: [%d]", (int32_t)qResult.size());
    QueryResult::Iterator it = qResult.iterator();
    while (it.hasNext())
    {
        const ResultDocPtr& pDoc = it.next();
        
        shardid_t shardId = pDoc->getShardId();
        FIRTEX_ASSERT2((size_t)shardId <= m_reqests.size());

        if (m_reqests[shardId]->str().empty())
        {
            m_stat.toString(*(m_reqests[shardId]));
            *(m_reqests[shardId]) << DocSelectClause::PREFIX 
                                  << Statement::CLAUSE_EQUAL_SYMBOL;
        }

        FX_TRACE("doc: [%d], shardId: [%d]", pDoc->getDocId(), shardId);
        (*m_reqests[shardId]) << pDoc->getDocId() << ",";
    }

    m_requestedShards.clear();
    for (i = 0; i < m_reqests.size(); ++i)
    {
        if (!m_reqests[i]->str().empty())
        {
            m_requestedShards.push_back(m_shards[i]);
        }
    }

    m_currentPhase = TWO_PHASE_TWO;
    for (i = 0; i < m_requestedShards.size(); ++i)
    {
        SearchShardBase* pShard = m_requestedShards[i];
        FX_DEBUG("Search shard: [%s], query: [%s]",
                 pShard->getName().c_str(),
                 m_reqests[pShard->getShardId()]->str().c_str());

        pShard->asyncSearch(m_reqests[pShard->getShardId()]->str());
    }

    m_deliver.waitFinish();

    SEARCH_LATENCY_END(m_nPhaseTwoLatency);

    collectResult();
}

void SearchShardsBase::collectPhaseTwoResult()
{
    FX_DEBUG("Collect phase two result");

    SEARCH_LATENCY_BEGIN();

    QueryResult phaseTwoResult;
    const QueryResult& qResult = m_pResultCollector->getResult();
    phaseTwoResult.setTracer(qResult.getTracer());

    int64_t nMaxTimeCost = 0;
    m_shardResults.clear();
    for (size_t i = 0; i < m_requestedShards.size(); ++i)
    {
        SearchShardBase* pShard = m_requestedShards[i];
        const string& sResult = pShard->getResponse();
        if (pShard->getStatus() != SearchReplicaBase::EC_OK)
        {
            stringstream ss;
            ss << "Has error on shard: [" << pShard->getName() 
               << "]: " << parseError(sResult);

            FX_LOG(ERROR, ss.str().c_str());
            setError(ss.str());
            SEARCH_LATENCY_END(m_nPhaseTwoCollectLatency);
            return;
        }

        FX_TRACE("Phase2 result: [%s]", sResult.c_str());

        XMLResultParser parser;
        QueryResultPtr& pTmpQRes = m_shardResults[pShard->getShardId()].second;
        parser.parse(sResult, *(pTmpQRes));
        if (pTmpQRes->getTimeCost() > nMaxTimeCost)
        {
            nMaxTimeCost = pTmpQRes->getTimeCost();
        }

        // merge trace information
        QueryTracerPtr& pTracer = pTmpQRes->getTracer();
        if (pTracer.isNotNull())
        {
            if (phaseTwoResult.getTracer().isNull())
            {
                phaseTwoResult.setTracer(new QueryTracer("proxy", pTracer->getLevel()));
            }
            string str;
            NumberFormatter::append(str, pShard->getShardId());
            phaseTwoResult.getTracer()->addChildTracer(pTracer)->prependPath(str);
        }
    }

    // Merge hit docs
    phaseTwoResult.setTotalHits(qResult.getTotalHits());
    phaseTwoResult.setTimeCost(nMaxTimeCost + qResult.getTimeCost());
    QueryResult::Iterator it = qResult.iterator();
    while (it.hasNext())
    {
        const ResultDocPtr& pDoc = it.next();
        shardid_t shardId = pDoc->getShardId();
        FIRTEX_ASSERT2((size_t)shardId <= m_shards.size());
        
        ResultPair& resPair = m_shardResults[shardId];
        QueryResultPtr& pQResult2 = resPair.second;
        if (pQResult2->hasError())
        {
            FX_LOG(ERROR, "Has error on shard: [%d], msg: [%s]",
                   pQResult2->getErrorMsg().c_str());
            phaseTwoResult.setError(pQResult2->getErrorMsg());
            break;
        }
        ResultDocPtr& pDoc2 = (*pQResult2)[resPair.first++];
        pDoc2->setScore(pDoc->getScore());
        phaseTwoResult.addDoc(pDoc2);
    }

    SEARCH_LATENCY_END(m_nPhaseTwoCollectLatency);

    // Add query trace information
    QueryTracerPtr& pResTracer = phaseTwoResult.getTracer();
    if (pResTracer.isNotNull())
    {
        FX_QUERY_TRACE(DEBUG, pResTracer, "Phase one search latency: [%d] ms",
                       m_nPhaseOneLatency);
        FX_QUERY_TRACE(DEBUG, pResTracer, "Phase one collection latency: [%d] ms",
                       m_nPhaseOneCollectLatency);
        FX_QUERY_TRACE(DEBUG, pResTracer, "Phase two search latency: [%d] ms",
                       m_nPhaseTwoLatency);
        FX_QUERY_TRACE(DEBUG, pResTracer, "Phase two collection latency: [%d] ms",
                       m_nPhaseTwoCollectLatency);
    }

    FX_DEBUG("Begin format result");
    stringstream ss;
    XMLResultFormatter formatter;
    formatter.format(phaseTwoResult, ss);
    m_sResult = ss.str();
    FX_DEBUG("End format result");
}
    
void SearchShardsBase::setError(const string& sErrorMsg)
{
    stringstream ss;
    XMLResultFormatter formatter;
    QueryResult qResult;
    qResult.setError(sErrorMsg);
    formatter.format(qResult, ss);
    m_sResult = ss.str();
}

string SearchShardsBase::parseError(const string& sResult)
{
    XMLResultParser parser;
    QueryResult qResult;
    parser.parse(sResult, qResult);
    return qResult.getErrorMsg();
}

#undef SEARCH_LATENCY_BEGIN
#undef SEARCH_LATENCY_END


FX_NS_END
