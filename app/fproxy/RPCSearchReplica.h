#ifndef __FX_RPCSEARCHREPLICA_H
#define __FX_RPCSEARCHREPLICA_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/network/FunctionBinder.h"
#include "ProxyServerConf.h"
#include "SearchReplicaBase.h"
#include "../common/RPCSyncClient.h"
#include "../thrift/gen-cpp/ThriftSearchService.h"

FX_NS_DEF(app);

class RPCSearchReplica : public SearchReplicaBase
{
public:
    typedef boost::shared_ptr<apache::thrift::protocol::TProtocolFactory> TProtocolFactoryPtr;

    typedef RPCSyncClient<ThriftSearchServiceClient> RPCClient;
    DEFINE_TYPED_PTR(RPCClient);

    using SearchReplicaBase::ErrorCode;

public:
    RPCSearchReplica();
    ~RPCSearchReplica();

public:
    /**
     * Initialize client
     *
     * @param sAddr host address
     * @param nPort host port
     */
    virtual void initClient(const std::string& sAddr, int32_t nPort);

    /**
     * Send request to searcher (non-blocking)
     *
     * @param sResponse response of request
     * @param sRequest request sending to searcher
     */
    virtual ErrorCode search(std::string& sResponse, const std::string& sRequest);

    /// Ping the server
    virtual bool ping();

    /// Return host name
    const std::string& getHost() const;

    /// Return port 
    int32_t getPort() const;

private:
    RPCClientPtr m_pSearchClient;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(RPCSearchReplica);

/////////////////////////////////////////////////////
inline const std::string& RPCSearchReplica::getHost() const
{
    return m_pSearchClient->getHost();
}

inline int32_t RPCSearchReplica::getPort() const
{
    return m_pSearchClient->getPort();
}

FX_NS_END

#endif
