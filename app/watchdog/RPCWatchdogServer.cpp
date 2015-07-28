#include "RPCWatchdogServer.h"
#include "RPCWatchdogService.h"
#include "../thrift/gen-cpp/ThriftWatchdogService.h"

using namespace std;
using namespace apache::thrift;

FX_NS_USE(utility);

FX_NS_DEF(app);

SETUP_LOGGER(app, RPCWatchdogServer);

RPCWatchdogServer::RPCWatchdogServer(const string& sWorkDir, int32_t nPort)
    : m_sWorkDir(sWorkDir), m_nListenPort(nPort)
{
}
 
RPCWatchdogServer::~RPCWatchdogServer()
{
}

bool RPCWatchdogServer::init(const std::string& sConfFile)
{
    try
    {
        setupLogger();

        boost::shared_ptr<ThriftWatchdogServiceIf> service;
        RPCWatchdogService* pService = new RPCWatchdogService();
        service.reset(pService);
        pService->init(m_sWorkDir);

        TProcessorPtr processor(new ThriftWatchdogServiceProcessor(service));
        m_pProcessor = processor;

        return true;
    }
    catch(const std::exception& e)
    {
        FX_LOG(ERROR, "Start daemon server FAILED: [%s]", e.what());
        return false;
    }
    catch(const FirteXException& e)
    {
        FX_LOG(ERROR, "Running daemon server FAILED: [%s].", e.what().c_str());
        return false;
    }
    
    return true;
}

void RPCWatchdogServer::run()
{
    FIRTEX_ASSERT2(m_pProcessor != NULL);
    m_pServer.reset(new RPCServer(m_pProcessor, m_nListenPort,
                                  DEFAULT_THREAD_POOL_SIZE));
    m_pServer->start(false);
}

void RPCWatchdogServer::stop()
{
    if (m_pServer)
    {
        m_pServer->stop();
    }
}

void RPCWatchdogServer::join()
{
    if (m_pServer)
    {
        m_pServer->join();
    }
}

FX_NS_END
