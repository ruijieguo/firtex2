#ifndef __FX_HTTPPROXYSERVER_H
#define __FX_HTTPPROXYSERVER_H

#include "firtex/extension/network/EvHttpServer.h"
#include "../common/ServerBase.h"
#include <string>

FX_NS_DEF(app);

class HTTPProxyServer : public ServerBase
{
public:
    HTTPProxyServer(const std::string& sRole);
    ~HTTPProxyServer();

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
    FX_NS(network)::EvHttpServerPtr m_pServer;
    FX_NS(network)::EvHttpServiceFactoryPtr m_pServiceFactory;
    std::string m_sRole;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(HTTPProxyServer);

/////////////////////////////////////////////
//

FX_NS_END

#endif
