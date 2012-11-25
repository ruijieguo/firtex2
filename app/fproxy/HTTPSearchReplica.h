#ifndef __FX_HTTPSEARCHREPLICA_H
#define __FX_HTTPSEARCHREPLICA_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "ProxyServerConf.h"
#include "SearchReplicaBase.h"
#include "firtex/extension/network/EvHttpSyncClient.h"

FX_NS_DEF(app);

class HTTPSearchReplica : public SearchReplicaBase
{
public:
    using SearchReplicaBase::ErrorCode;

public:
    HTTPSearchReplica();
    ~HTTPSearchReplica();

public:
    /**
     * Initialize client
     *
     * @param sAddr host address
     * @param nPort host port
     */
    virtual void initClient(const std::string& sAddr, int32_t nPort);

    /**
     * Send request to searcher (blocking)
     *
     * @param sResponse response of request
     * @param sRequest request sending to searcher
     * @param return EC_OK if everything is ok
     */
    virtual ErrorCode search(std::string& sResponse, 
                             const std::string& sRequest);

    /// Ping the server
    virtual bool ping();

    /// Return host name
    const std::string& getHost() const;

    /// Return port 
    int32_t getPort() const;

private:
    FX_NS(network)::EvHttpSyncClientPtr m_pSearchClient;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(HTTPSearchReplica);

/////////////////////////////////////////////////////
inline const std::string& HTTPSearchReplica::getHost() const
{
    return m_pSearchClient->getHost();
}

inline int32_t HTTPSearchReplica::getPort() const
{
    return m_pSearchClient->getPort();
}

FX_NS_END

#endif
