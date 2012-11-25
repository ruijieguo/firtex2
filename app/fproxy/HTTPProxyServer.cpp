#include "HTTPProxyServer.h"
#include "firtex/utility/NumberFormatter.h"
#include "HTTPBlenderService.h"
#include "HTTPProxyService.h"
#include "../common/HTTPPingService.h"
#include "BlenderServerConf.h"
#include "ProxyServerConf.h"

using namespace std;
FX_NS_USE(network);

FX_NS_DEF(app);

SETUP_LOGGER(app, HTTPProxyServer);

HTTPProxyServer::HTTPProxyServer(const std::string& sRole)
    : m_sRole(sRole)
{

}
 
HTTPProxyServer::~HTTPProxyServer()
{
}

ServerConfBase* HTTPProxyServer::createConf()
{
    if (m_sRole == "blender")
    {
        return new BlenderServerConf();
    }
    else
    {
        return new ProxyServerConf();
    }
}

bool HTTPProxyServer::init(const std::string& sConfFile)
{
    if (!loadConf(sConfFile))
    {
        return false;
    }

    setupLogger();

    EvHttpServiceFactoryPtr pServiceFactory = new EvHttpServiceFactory();
    if (m_sRole == "blender")
    {
        BlenderServerConfPtr pBlenderConf = m_pConf.cast<BlenderServerConf>();
        HTTPBlenderService* pService = new HTTPBlenderService();
        if (!pService->init(pBlenderConf))
        {
            FX_LOG(ERROR, "Init proxy service (role = blender) FAILED");
            delete pService;
            return false;
        }
        pServiceFactory->registerService(pService);
    }
    else
    {
        ProxyServerConfPtr pProxyConf = m_pConf.cast<ProxyServerConf>();
        HTTPProxyService* pService = new HTTPProxyService();
        if (!pService->init(pProxyConf))
        {
            FX_LOG(ERROR, "Init proxy service (role = proxy) FAILED");
            delete pService;
            return false;
        }
        pServiceFactory->registerService(pService);
    }

    pServiceFactory->registerService(new HTTPPingService());

    m_pServiceFactory = pServiceFactory;

    return true;
}

void HTTPProxyServer::run()
{
    FIRTEX_ASSERT2(m_pServiceFactory.isNotNull());
    m_pServer = new EvHttpServer(m_pConf->serverConf.host,
                                 m_pConf->serverConf.listen_port,
                                 m_pServiceFactory,
                                 m_pConf->serverConf.thread_pool);    
    m_pServer->start(false);
}

void HTTPProxyServer::stop()
{
    if (m_pServer.isNotNull())
    {
        m_pServer->stop();
    }
}

void HTTPProxyServer::join()
{
    if (m_pServer.isNotNull())
    {
        m_pServer->join();
    }
}

FX_NS_END
