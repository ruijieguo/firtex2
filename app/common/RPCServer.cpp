#include "RPCServer.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/extension/network/NetworkException.h"
#include "firtex/utility/NumberParser.h"


#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TTransportUtils.h>

using namespace std;

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using namespace apache::thrift::concurrency;

FX_NS_USE(utility);
FX_NS_USE(common);
FX_NS_USE(thread);

FX_NS_DEF(app);

SETUP_LOGGER(app, RPCServer);
SETUP_LOGGER_EMBED(app, RPCServer, ServerRunner);

RPCServer::RPCServer(const TProcessorPtr& pProcessor, int32_t nPort, 
                     int32_t nThreadCount)
    : m_pProcessor(pProcessor)
    , m_nPort(nPort)
    , m_nThreadCount(nThreadCount)
{
}

RPCServer::~RPCServer()
{
}

void RPCServer::start(bool bBlocking)
{
    try
    {
        if (bBlocking)
        {
            FX_LOG(INFO, "Start server in blocking mode, port: [%d], "
                   "thread count: [%d]...", m_nPort, m_nThreadCount);
            startServer();
        }
        else
        {
            FX_LOG(INFO, "Start server in non-blocking mode, port: [%d], "
                   "thread count: [%d]...", m_nPort, m_nThreadCount);
            m_pServerRunner.reset(new ServerRunner(*this));
            m_pServerThread.reset(new FX_NS(thread)::Thread());
            m_pServerThread->start(*m_pServerRunner);
        }
    }
    catch(const std::exception& e)
    {
        FX_LOG(ERROR, "Start server FAILED: [%s]", e.what());
    }
    catch(const FirteXException& e)
    {
        FX_LOG(ERROR, "Start server FAILED: ", e.what().c_str());
    }
}

void RPCServer::stop()
{
    if (m_pServerRunner)
    {
        FX_LOG(INFO, "Stopping server in non-blocking mode, port: [%d]", m_nPort);
        m_pServerRunner->stop();
    }
    else 
    {
        stopServer();
    }
}
 
void RPCServer::join()
{
    if (m_pServerThread)
    {
        m_pServerThread->join();
    }
}

void RPCServer::startServer()
{
    try
    {
        boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

        boost::shared_ptr<ThreadManager> threadManager =
            ThreadManager::newSimpleThreadManager(m_nThreadCount);
        boost::shared_ptr<PlatformThreadFactory> threadFactory =
            boost::shared_ptr<PlatformThreadFactory>(new PlatformThreadFactory());

        threadManager->threadFactory(threadFactory);
        threadManager->start();
        
        m_pServer.reset(new TNonblockingServer(m_pProcessor, protocolFactory,
                        m_nPort, threadManager));
    
        m_pServer->serve();
    }
    catch (const TException& e)
    {
        FIRTEX_THROW(NetworkException, "%s", e.what());
    }
}

void RPCServer::stopServer()
{
    if (m_pServer)
    {
        m_pServer->stop();
    }
}

void RPCServer::ServerRunner::run()
{
    try
    {
        m_server.startServer();
    }
    catch(const FirteXException& e)
    {
        FX_LOG(ERROR, "Start server FAILED: [%s]", e.what().c_str());
    }
}

FX_NS_END
