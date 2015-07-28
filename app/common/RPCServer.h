#ifndef __FX_RPC_SERVER_H
#define __FX_RPC_SERVER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/thread/FastMutex.h"
#include "firtex/thread/Condition.h"
#include "firtex/common/SharedPtr.h"

#ifdef FX_WINDOWS
#include <ctype.h>
#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <io.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <thrift/server/TServer.h>
#include <thrift/TProcessor.h>

FX_NS_DEF(app);

class RPCServer
{
public:
    typedef boost::shared_ptr<apache::thrift::server::TServer> TServerPtr;
    typedef boost::shared_ptr<apache::thrift::TProcessor> TProcessorPtr;

public:
    RPCServer(const TProcessorPtr& pProcessor, int32_t nPort, 
              int32_t nThreadCount);
    ~RPCServer();

public:
    void start(bool bBlocking);

    void stop();
    void join();

private:
    class ServerRunner : public FX_NS(thread)::Runnable
    {
    public:
        ServerRunner(RPCServer& server) 
            : m_server(server)
        {
        }

        void run();

        void stop()
        {
            m_server.stopServer();
        }

    private:
        RPCServer& m_server;

    private:
        DECLARE_LOGGER();
    };
    DEFINE_TYPED_PTR(ServerRunner);

private:
    void startServer();
    void stopServer();

private:
    TServerPtr m_pServer;

    FX_NS(thread)::ThreadPtr m_pServerThread;
    ServerRunnerPtr m_pServerRunner;

    TProcessorPtr m_pProcessor;
    int32_t m_nPort;
    int32_t m_nThreadCount;

    friend class ServerRunner;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(RPCServer);


FX_NS_END

#endif
