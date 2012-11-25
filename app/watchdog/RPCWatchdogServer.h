#ifndef __FX_RPCDAEMONSERVER_H
#define __FX_RPCDAEMONSERVER_H

#include "../common/ServerBase.h"
#include "../common/RPCServer.h"
#include <string>

FX_NS_DEF(app);

typedef boost::shared_ptr<apache::thrift::TProcessor> TProcessorPtr;

class RPCWatchdogServer : public ServerBase
{
public:
    typedef FX_NS(common)::SharedPtr<apache::thrift::server::TServer> TServerPtr;
    const static size_t DEFAULT_THREAD_POOL_SIZE = 1;

public:
    RPCWatchdogServer(const std::string& sWorkDir, int32_t nPort);
    ~RPCWatchdogServer();

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
    std::string m_sWorkDir;
    int32_t m_nListenPort;    

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(RPCWatchdogServer);

/////////////////////////////////////////////
//

FX_NS_END

#endif
