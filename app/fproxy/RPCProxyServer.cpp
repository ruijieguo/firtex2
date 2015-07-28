#include "RPCProxyServer.h"
#include "RPCBlenderService.h"
#include "RPCProxyService.h"
#include "ProxyServerConf.h"
#include "BlenderServerConf.h"
#include "ProxyServerConf.h"
#include "../thrift/gen-cpp/ThriftSearchService.h"

using namespace std;
using namespace apache::thrift;

FX_NS_USE(network);
FX_NS_USE(utility);

FX_NS_DEF(app);

SETUP_LOGGER(app, RPCProxyServer);

RPCProxyServer::RPCProxyServer(const std::string& sRole)
    : m_sRole(sRole)
{
}
 
RPCProxyServer::~RPCProxyServer()
{
}

ServerConfBase* RPCProxyServer::createConf()
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

bool RPCProxyServer::init(const std::string& sConfFile)
{
    try
    {
        if (!loadConf(sConfFile))
        {
            return false;
        }
        setupLogger();

        boost::shared_ptr<ThriftSearchServiceIf> service;
        if (m_sRole == "blender")
        {
            BlenderServerConfPtr pBlenderConf = m_pConf.cast<BlenderServerConf>();
            RPCBlenderService* pService = new RPCBlenderService();
            service.reset(pService);
            if (!pService->init(pBlenderConf))
            {
                FX_LOG(ERROR, "Init proxy service (role = blender) FAILED");
                return false;
            }
        }
        else 
        {
            ProxyServerConfPtr pProxyConf = m_pConf.cast<ProxyServerConf>();
            RPCProxyService* pService = new RPCProxyService();
            service.reset(pService);
            if (!pService->init(pProxyConf))
            {
                FX_LOG(ERROR, "Init proxy service (role = proxy) FAILED");
                return false;
            }
        }

        TProcessorPtr processor(new ThriftSearchServiceProcessor(service));
        m_pProcessor = processor;

        return true;
    }
    catch(const std::exception& e)
    {
        FX_LOG(ERROR, "Start Proxy server FAILED: [%s]", e.what());
        return false;
    }
    catch(const FirteXException& e)
    {
        FX_LOG(ERROR, "Running Proxy server FAILED: [%s].", e.what().c_str());
        return false;
    }
    
    return true;
}

void RPCProxyServer::run()
{
    FIRTEX_ASSERT2(m_pProcessor != NULL);
    m_pServer = new RPCServer(m_pProcessor, m_pConf->serverConf.listen_port,
                              m_pConf->serverConf.thread_pool);
    m_pServer->start(false);
}

void RPCProxyServer::stop()
{
    if (m_pServer)
    {
        m_pServer->stop();
    }
}

void RPCProxyServer::join()
{
    if (m_pServer)
    {
        m_pServer->join();
    }
}

FX_NS_END
