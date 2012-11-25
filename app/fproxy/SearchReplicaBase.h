#ifndef __FX_SEARCHREPLICABASE_H
#define __FX_SEARCHREPLICABASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "SearchServiceClient.h"
#include "ProxyServerConf.h"
#include "firtex/extension/network/NetworkDefines.h"

FX_NS_DEF(app);

class SearchReplicaBase : public SearchServiceClient
{
public:
    using SearchServiceClient::ErrorCode;

public:
    SearchReplicaBase();
    virtual ~SearchReplicaBase();

public:
    /// Initialize the search Shard
    bool init(const ProxyServerConf::sReplica& repConf);

    /**
     * Send request to searcher (non-blocking)
     *
     * @param sResponse response of request
     * @param sRequest request sending to searcher
     * @param return EC_OK if everything is ok
     */
    virtual ErrorCode search(std::string& sResponse, 
                             const std::string& sRequest) 
    {
        return EC_ERROR;
    }

    /// Ping the server
    virtual bool ping() = 0;

    /// Return the replica id
    int32_t getReplicaId() const;
    
    /// Set replica id
    void setReplicaId(int32_t repId);

    /// Return the weight of this replica
    int32_t getWeight() const;

private:
    /// Replica Id
    int32_t m_replicaId;

    /// Weight of the replica
    int32_t m_nWeight;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(SearchReplicaBase);

/////////////////////////////////////////////////////
inline int32_t SearchReplicaBase::getReplicaId() const
{
    return m_replicaId;
}
    
inline void SearchReplicaBase::setReplicaId(int32_t repId)
{
    m_replicaId = repId;
}

inline int32_t SearchReplicaBase::getWeight() const
{
    return m_nWeight;
}

FX_NS_END

#endif
