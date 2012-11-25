#include "ProxyServerAppRunner.h"
#include "HTTPProxyService.h"
#include "HTTPBlenderService.h"
#include "HTTPProxyServer.h"
#include "firtex/extension/util/Application.h"
#include "firtex/extension/util/SharedOptionCallback.h"
#include "firtex/extension/util/OptionException.h"

#ifdef HAVE_THRIFT
#include "RPCProxyService.h"
#include "RPCProxyServer.h"
#include "RPCBlenderService.h"
using namespace apache;
using namespace apache::thrift;
#endif

using namespace std;

FX_NS_USE(common);
FX_NS_USE(config);
FX_NS_USE(network);
FX_NS_USE(thread);
FX_NS_USE(util);
FX_NS_USE(utility);

FX_NS_DEF(app);

SETUP_LOGGER(app, ProxyServerAppRunner);

ProxyServerAppRunner::ProxyServerAppRunner()
    : m_sRole("proxy")
    , m_sServiceType("rpc")
{
}

ProxyServerAppRunner::~ProxyServerAppRunner()
{
}

AppRunner::Status ProxyServerAppRunner::run()
{
    if (m_sConfFile.empty())
    {
        return S_NONE;
    }

    if (m_sServiceType == "http")
    {
        if (!initHttpServer())
        {
            return S_EXIT;
        }
    }
    else
    {
        if (!initRPCServer())
        {
            return S_EXIT;
        }
    }

    try
    {
        m_pServer->run();
        return S_RUNNING;
    }
    catch(const FirteXException& e)
    {
        FX_LOG(ERROR, "Run proxy server FAILED: [%s]", e.what().c_str());
        return S_EXIT;
    }
    return S_EXIT;
}

bool ProxyServerAppRunner::initHttpServer()
{
    m_pServer = new HTTPProxyServer(m_sRole);
    if (!m_pServer->init(m_sConfFile))
    {
        return false;
    }
    return true;
}

bool ProxyServerAppRunner::initRPCServer()
{
#ifdef HAVE_THRIFT
    m_pServer = new RPCProxyServer(m_sRole);
    if (!m_pServer->init(m_sConfFile))
    {
        return false;
    }
    return true;
#else
    FX_LOG(ERROR, "No RPC server, please re-compile with thrift");
    return false;
#endif
}

void ProxyServerAppRunner::registerOption(Application* pApp)
{
    pApp->addDescription("Setup Proxy server : ");
    pApp->addDescription(tstring("  ") + pApp->getProgName() + 
                         " -c [CONF_FILE] -");

    pApp->addOption(Option("conf", "c", "specify configure file path")
                    .required(true)
                    .repeatable(false)
                    .argument("[CONF_FILE]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("role", "r", "specify proxy role (proxy/blender)")
                    .repeatable(false)
                    .argument("[PROXY_ROLE]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("service-type", "t", "specify service type (rpc/http)")
                    .repeatable(false)
                    .argument("[SERVICE_TYPE]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 
}

void ProxyServerAppRunner::optionCallback(const Option& option,
        const tstring& sValue)
{
    if (option.matchFull("conf") || option.matchShort("c"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--conf/-c");
        }
        m_sConfFile = sValue;
    }

    if (option.matchFull("role") || option.matchShort("r"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--role/-r");
        }

        if (sValue != "proxy" && sValue != "blender")
        {
            FIRTEX_THROW(BadParameterException, "the value of --role "
                         "must be proxy or blender");
        }
        m_sRole = sValue;
    }

    if (option.matchFull("service-type") || option.matchShort("t"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--service-type/-t");
        }

        if (sValue != "rpc" && sValue != "http")
        {
            FIRTEX_THROW(BadParameterException, "the value of --service-type "
                         "must be rpc or http");
        }
        m_sServiceType = sValue;
    }
}

void ProxyServerAppRunner::stop()
{
    if (m_pServer.isNotNull())
    {
        m_pServer->stop();
    }
}

void ProxyServerAppRunner::waitStop()
{
    if (m_pServer.isNotNull())
    {
        m_pServer->join();
    }
}

FX_NS_END
