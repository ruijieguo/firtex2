#include "HTTPSearchServer.h"
#include "firtex/extension/network/EvHttpServiceFactory.h"
#include "HTTPSearchService.h"
#include "../common/HTTPPingService.h"

using namespace std;
FX_NS_USE(network);

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
    m_pServiceFactory = new EvHttpServiceFactory();
    m_pServiceFactory->registerService(new HTTPSearchService(m_resource));
    m_pServiceFactory->registerService(new HTTPPingService());
    m_pServer = new EvHttpServer(m_pConf->serverConf.host,
                                 m_pConf->serverConf.listen_port,
                                 m_pServiceFactory,
                                 m_pConf->serverConf.thread_pool);    
    m_pServer->start(false);
}

void HTTPSearchServer::stop()
{
    if (m_pServer.isNotNull())
    {
        m_pServer->stop();
    }
}

void HTTPSearchServer::join()
{
    if (m_pServer.isNotNull())
    {
        m_pServer->join();
    }
}

FX_NS_END

