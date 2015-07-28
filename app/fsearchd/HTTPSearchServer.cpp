#include "HTTPSearchServer.h"
#include "firtex/extension/network/EvHttpServiceFactory.h"
#include "HTTPSearchService.h"
#include "../common/HTTPPingService.h"

using namespace std;
FX_NS_USE(network);
FX_NS_USE(utility);

FX_NS_DEF(app);

SETUP_LOGGER(app, HTTPSearchServer);

HTTPSearchServer::HTTPSearchServer() 
{
}

HTTPSearchServer::~HTTPSearchServer() 
{
}

void HTTPSearchServer::run()
{
    m_pServiceFactory.reset(new EvHttpServiceFactory());
    m_pServiceFactory->registerService(new HTTPSearchService(m_resource));
    m_pServiceFactory->registerService(new HTTPPingService());
    m_pServer.reset(new EvHttpServer("localhost",
                    m_conf.Searcher.port,
                    m_pServiceFactory,
                    m_conf.Searcher.thread_pool));
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

void HTTPSearchServer::stop()
{
    if (m_pServer)
    {
        m_pServer->stop();
    }
}

void HTTPSearchServer::join()
{
    if (m_pServer)
    {
        m_pServer->join();
    }
}

FX_NS_END

