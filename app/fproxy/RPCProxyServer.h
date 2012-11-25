#ifndef __FX_RPCPROXYSERVER_H
#define __FX_RPCPROXYSERVER_H

#include "firtex/extension/network/EvHttpServer.h"
#include "../common/ServerBase.h"
#include "../common/RPCServer.h"
#include <string>

FX_NS_DEF(app);

typedef boost::shared_ptr<apache::thrift::TProcessor> TProcessorPtr;

class RPCProxyServer : public ServerBase
{
public:
    typedef FX_NS(common)::SharedPtr<apache::thrift::server::TServer> TServerPtr;

public:
    RPCProxyServer(const std::string& sRole);
    ~RPCProxyServer();

public:
    /// Initialize resource
    virtual bool init(const std::string& sConfFile);

    /// Run the server
    virtual void run();

    /// Stop the server
    virtual void stop();

    /// Wait for finish
    virtual void join();

protected:
    /// Create server configure
    virtual ServerConfBase* createConf();

private:
    TProcessorPtr m_pProcessor;
    RPCServerPtr m_pServer;
    std::string m_sRole;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(RPCProxyServer);

/////////////////////////////////////////////
//

FX_NS_END

#endif
