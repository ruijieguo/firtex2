#include "HTTPWatchdogServer.h"
#include "HTTPWatchdogService.h"
#include "../common/HTTPPingService.h"

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(network);

FX_NS_DEF(app);

SETUP_LOGGER(app, HTTPWatchdogServer);

HTTPWatchdogServer::HTTPWatchdogServer(const string& sWorkDir, int32_t nPort)
    : m_sWorkDir(sWorkDir), m_nListenPort(nPort)
{
}

HTTPWatchdogServer::~HTTPWatchdogServer() 
{
}

bool HTTPWatchdogServer::init(const std::string& sConfFile)
{
    try
    {
        setupLogger();

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

void HTTPWatchdogServer::run()
{
    m_pServiceFactory.reset(new EvHttpServiceFactory());
    HTTPWatchdogService* pService = new HTTPWatchdogService();
    pService->init(m_sWorkDir);
    m_pServiceFactory->registerService(pService);
    m_pServiceFactory->registerService(new HTTPPingService());

    m_pServer.reset(new EvHttpServer("localhost",
                    m_nListenPort,
                    m_pServiceFactory,
                    DEFAULT_THREAD_POOL_SIZE));
    try 
    {
        m_pServer->start(false);
    }
    catch (const FirteXException& e)
    {
        stop();
        join();
        FIRTEX_RETHROW(e);
    }
}

void HTTPWatchdogServer::stop()
{
    if (m_pServer)
    {
        m_pServer->stop();
    }
}

void HTTPWatchdogServer::join()
{
    if (m_pServer)
    {
        m_pServer->join();
    }
}

FX_NS_END

