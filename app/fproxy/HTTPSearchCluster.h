#ifndef __FX_HTTPSEARCHCLUSTER_H
#define __FX_HTTPSEARCHCLUSTER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "SearchClusterBase.h"
#include "BlenderServerConf.h"
#include "firtex/extension/network/EvHttpSyncClient.h"

FX_NS_DEF(app);

class HTTPSearchCluster : public SearchClusterBase
{
public:
    using SearchClusterBase::AsyncDeliver;

public:
    HTTPSearchCluster(AsyncDeliver* pDeliver);
    ~HTTPSearchCluster();

public:
    /// Return host name of searcher 
    const std::string& getHost() const;

    /// Return ip port of searcher
    uint32_t getPort() const;

protected:
    /**
     * Initialize client
     *
     * @param sAddr host address
     * @param nPort host port
     */
    virtual void initClient(const std::string& sAddr, int32_t nPort);

    /// Send request in blocking mode
    virtual void run();

private:
    FX_NS(network)::EvHttpSyncClientPtr m_pSearchClient;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(HTTPSearchCluster);

/////////////////////////////////////////////////////
inline const std::string& HTTPSearchCluster::getHost() const
{
    return m_pSearchClient->getHost();
}

inline uint32_t HTTPSearchCluster::getPort() const
{
    return m_pSearchClient->getPort();
}

FX_NS_END

#endif
