#ifndef __FX_SEARCHSHARDBASE_H
#define __FX_SEARCHSHARDBASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/FastMutex.h"
#include "SearchReplicaBase.h"
#include "firtex/extension/network/FunctionBinder.h"
#include "firtex/extension/network/NetworkDefines.h"
#include "ProxyServerConf.h"
#include "LoadBallancePolicy.h"
#include "RequestAsyncDeliver.h"

FX_NS_DEF(app);

class SearchShardBase
{
public:
    typedef RequestAsyncDeliver<SearchShardBase> AsyncDeliver;
    typedef SearchReplicaBase::ErrorCode ErrorCode;

public:
    SearchShardBase(AsyncDeliver* pDeliver);
    virtual ~SearchShardBase();

public:
    /// Initialize the search Shard
    bool init(const ProxyServerConf::sShard& shardConf,
              const std::string& sLoadBallancePolicy);

    /**
     * Send request to searcher (non-blocking)
     *
     * @param sRequest request sending to searcher
     */
    void asyncSearch(const std::string& sRequest);

    /// Return shard id
    shardid_t getShardId() const;

    /// Set shard id
    void setShardId(shardid_t shardId);

    /// Return shard name
    const std::string& getName() const;

    /// Return status of the replica
    ErrorCode getStatus() const;

    /// Return response
    const std::string& getResponse() const;

    /// Send request in blocking mode
    virtual void run();

    /// Check unavailable replicas
    virtual void checkUnavailableReplicas();

protected:
    /**
     * Create the replica
     */
    virtual SearchReplicaBase* newReplica() = 0;

private:
    typedef std::vector<SearchReplicaBasePtr> ReplicaVector;

    /// Id of shard
    shardid_t m_shardId;
    
    /// Name of the replica
    std::string m_sName;

    /// All replicas
    ReplicaVector m_replicas;
    
    /// All unavailable replicas
    ReplicaVector m_unavailableReplicas;
    
    /// The load ballance policy
    LoadBallancePolicyPtr m_pBallancePolicy;

    /// The current active replica
    SearchReplicaBasePtr m_pActiveReplica;

    /// Current processing request
    std::string m_sRequest;

    /// Response of last request
    std::string m_sResponse;

    /// Status of search operation
    ErrorCode m_status;

    /// Async request deliver
    AsyncDeliver* m_pDeliver;

    /// Lock for unavailable replicas
    FX_NS(thread)::FastMutex m_lock;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(SearchShardBase);

/////////////////////////////////////////////////////
inline const std::string& SearchShardBase::getResponse() const
{
    return m_sResponse;
}

inline shardid_t SearchShardBase::getShardId() const
{
    return m_shardId;
}

inline void SearchShardBase::setShardId(shardid_t shardId)
{
    m_shardId = shardId;
}

inline const std::string& SearchShardBase::getName() const
{
    return m_sName;
}

inline SearchShardBase::ErrorCode SearchShardBase::getStatus() const
{
    return m_status;
}

FX_NS_END

#endif
