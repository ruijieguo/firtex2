#include "WatchdogAppRunner.h"
#include "HTTPWatchdogServer.h"
#include "firtex/extension/util/Application.h"
#include "firtex/extension/util/SharedOptionCallback.h"
#include "firtex/extension/util/OptionException.h"

#ifdef HAVE_THRIFT
#include "RPCWatchdogServer.h"
#endif

using namespace std;
// using namespace apache;
// using namespace apache::thrift;

FX_NS_USE(common);
FX_NS_USE(config);
FX_NS_USE(thread);
FX_NS_USE(util);
FX_NS_USE(utility);

FX_NS_DEF(app);

SETUP_LOGGER(app, WatchdogAppRunner);

WatchdogAppRunner::WatchdogAppRunner()
    : m_sServiceType("rpc")
    , m_nListenPort(0)
{
}

WatchdogAppRunner::~WatchdogAppRunner()
{
}

AppRunner::Status WatchdogAppRunner::run()
{
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

bool WatchdogAppRunner::initHttpServer()
{
    m_pServer = new HTTPWatchdogServer(m_sWorkDir, m_nListenPort);
    if (!m_pServer->init(""))
    {
        FX_LOG(ERROR, "Initialize HTTP watchdog server FAILED.");
        return false;
    }
    return true;
}

bool WatchdogAppRunner::initRPCServer()
{
#ifdef HAVE_THRIFT
    m_pServer = new RPCWatchdogServer(m_sWorkDir, m_nListenPort);
    if (!m_pServer->init(""))
    {
        FX_LOG(ERROR, "Initialize RPC server FAILED.");
        return false;
    }
    return true;
#else
    FX_LOG(ERROR, "No RPC server, please re-compile with thrift");
    return false;
#endif
}

void WatchdogAppRunner::registerOption(Application* pApp)
{
    pApp->addDescription("Setup daemon server : ");
    pApp->addDescription(string("  ") + pApp->getProgName() + 
                         " -p [LISTEN_PORT] -t [SERVICE_TYPE]");

    pApp->addOption(Option("listen-port", "p", "listen port")
                    .required(true)
                    .repeatable(false)
                    .argument("[LISTEN_PORT]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("work-dir", "w", "work directory")
                    .required(true)
                    .repeatable(false)
                    .argument("[WORK_DIR]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("service-type", "t", "specify service type (rpc/http)")
                    .repeatable(false)
                    .argument("[SERVICE_TYPE]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 
}

void WatchdogAppRunner::optionCallback(const Option& option,
        const string& sValue)
{
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

    if (option.matchFull("listen-port") || option.matchShort("p"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--listen-port/-p");
        }

        m_nListenPort = NumberParser::parseInt32(sValue);
    }

    if (option.matchFull("work-dir") || option.matchShort("w"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--work-dir/-w");
        }

        m_sWorkDir = sValue;
    }
}

void WatchdogAppRunner::stop()
{
    if (m_pServer.isNotNull())
    {
        m_pServer->stop();
    }
}

void WatchdogAppRunner::waitStop()
{
    if (m_pServer.isNotNull())
    {
        m_pServer->join();
    }
}

FX_NS_END
