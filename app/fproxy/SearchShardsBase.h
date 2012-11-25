#ifndef __FX_SEARCHSHARDSBASE_H
#define __FX_SEARCHSHARDSBASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/FastMutex.h"
#include "firtex/thread/Thread.h"
#include "firtex/search/ResultCollector.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/search/Statement.h"
#include "RPCSearchShard.h"
#include "ProxyServerConf.h"
#include "RequestAsyncDeliver.h"
#include <vector>
#include <sstream>

namespace firtex 
{
class QueryCommand;
}

FX_NS_DEF(app);

class SearchShardsBase
{
public:
    const static size_t MAX_RETRY_COUNT = 3;

    enum SearchPhase
    {
        NONE_PHASE = 0,
        ONE_PHASE,
        TWO_PHASE_ONE,
        TWO_PHASE_TWO,
    };

    typedef RequestAsyncDeliver<SearchShardBase> AsyncDeliver;

public:
    SearchShardsBase(int32_t nTimeOut);
    virtual ~SearchShardsBase();

public:
    /// Initialize search cluster
    bool init(const ProxyServerConf::sResource& resConf);

#ifdef HAVE_THRIFT
    /**
     * Handle search request
     *
     * @param sResult search result
     * @param cmd search command
     */
    virtual void search(std::string& sResult, const QueryCommand& cmd);
#endif

    /**
     * Handle search request
     *
     * @param sResult search result
     * @param sUri request uri
     */
    virtual void syntaxSearch(std::string& sResult, const std::string& sUri);

protected:
    /**
     * Create search shard implement
     *
     * @param pDeliver async request deliver
     */
    virtual SearchShardBase* newShard(AsyncDeliver* pDeliver) = 0;

private:
    typedef std::vector<SearchShardBase*> ShardVector;

private:
    void forwardRequest(const std::string& sUri);
    void doPhaseOneSearch(const std::string& sUri);
    void doPhaseTwoSearch();

    void collectResult();
    void collectPhaseOneResult();
    void collectPhaseTwoResult();

    void setError(const std::string& sErrorMsg);
    std::string parseError(const std::string& sResult);
    
private:
    class ReplicaHealthChecker : public FX_NS(thread)::Runnable
    {
    public:
        static const size_t DEFAULT_REPLICA_CHECK_TIME = 30 * 1000; //30 seconds

    public:
        ReplicaHealthChecker(ShardVector& shards)
            : m_shards(shards), m_bStopRequested(false) {}

        void stop()
        {
            m_bStopRequested = true;
        }

        void run()
        {
            while (!m_bStopRequested)
            {
                for (ShardVector::iterator it = m_shards.begin();
                     it != m_shards.end(); ++it)
                {
                    (*it)->checkUnavailableReplicas();
                    if (m_bStopRequested)
                        break;
                }
                FX_NS(thread)::Thread::sleep(DEFAULT_REPLICA_CHECK_TIME);
            }
        }

    private:
        ShardVector& m_shards;
        volatile bool m_bStopRequested;
    };
    DEFINE_TYPED_PTR(ReplicaHealthChecker);

    typedef std::pair<size_t, FX_NS(search)::QueryResultPtr> ResultPair;
    typedef std::vector<ResultPair> ResultVect;
    struct ShardResults : public ResultVect
    {
        void init(size_t n)
        {
            resize(n);
            for (size_t i = 0; i < n; ++i)
            {
                (*this)[i].first = 0;
                (*this)[i].second = new FX_NS(search)::QueryResult();
            }    
        }

        void clear()
        {
            for (size_t i = 0; i < size(); ++i)
            {
                (*this)[i].first = 0;
                (*this)[i].second->clear();                
            }
        }
    };

    typedef std::vector<std::stringstream*> StreamVector;

    /// Replica check thread
    FX_NS(thread)::ThreadPtr m_pCheckThread;

    /// Replica checker
    ReplicaHealthCheckerPtr m_pHealthChecker;

    /// Total search shards
    ShardVector m_shards;

    /// The requested shards 
    ShardVector m_requestedShards;

    /// Store phase two requests for each shard
    StreamVector m_reqests;

    /// The request deliver
    AsyncDeliver m_deliver;

    /// Search result collector
    FX_NS(search)::ResultCollectorPtr m_pResultCollector;

    /// Store combined search result
    std::string m_sResult;

    /// Current search phase
    SearchPhase m_currentPhase;

    /// Statement of current request
    FX_NS(search)::Statement m_stat;

    /// Store response state of shards
    ShardResults m_shardResults;

    /// Search timeout, in milliseconds
    int32_t m_nTimeout;

    /// Latency of phase one search
    int64_t m_nPhaseOneLatency;

    /// Collection latency of  phase one result
    int64_t m_nPhaseOneCollectLatency;

    /// Latency of phase two
    int64_t m_nPhaseTwoLatency;

    /// Collection latency of phase two result
    int64_t m_nPhaseTwoCollectLatency;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(SearchShardsBase);

/////////////////////////////////////////////////////
/// Inline functions

FX_NS_END

#endif
