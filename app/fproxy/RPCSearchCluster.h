#ifndef __FX_RPCSEARCHCLUSTER_H
#define __FX_RPCSEARCHCLUSTER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "BlenderServerConf.h"
#include "../common/RPCSyncClient.h"
#include "SearchClusterBase.h"
#include "../thrift/gen-cpp/ThriftSearchService.h"

FX_NS_DEF(app);

class RPCSearchCluster : public SearchClusterBase
{
public:
    using SearchClusterBase::AsyncDeliver;

public:
    typedef RPCSyncClient<ThriftSearchServiceClient> RPCClient;
    DEFINE_TYPED_PTR(RPCClient);

public:
    RPCSearchCluster(AsyncDeliver* pDeliver);
    ~RPCSearchCluster();

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
    RPCClientPtr m_pSearchClient;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(RPCSearchCluster);

/////////////////////////////////////////////////////
inline const std::string& RPCSearchCluster::getHost() const
{
    return m_pSearchClient->getHost();
}

inline uint32_t RPCSearchCluster::getPort() const
{
    return m_pSearchClient->getPort();
}

FX_NS_END

#endif
