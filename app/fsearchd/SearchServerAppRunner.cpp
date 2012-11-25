#include "SearchServerAppRunner.h"
#include "firtex/extension/util/Application.h"
#include "firtex/extension/util/SharedOptionCallback.h"
#include "firtex/extension/util/OptionException.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/common/Logger.h"
#include "firtex/extension/network/NetworkException.h"
#include "HTTPSearchServer.h"
#ifdef HAVE_THRIFT
#include "RPCSearchServer.h"
#endif

using namespace std;

FX_NS_USE(util);
FX_NS_USE(utility);
FX_NS_USE(common);
FX_NS_USE(config);
FX_NS_USE(network);
FX_NS_USE(thread);

FX_NS_DEF(app);

SETUP_LOGGER(app, SearchServerAppRunner);

SearchServerAppRunner::SearchServerAppRunner()
{
}

SearchServerAppRunner::~SearchServerAppRunner()
{
}

AppRunner::Status SearchServerAppRunner::run()
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
        FX_LOG(ERROR, "Run search server FAILED: [%s]", e.what().c_str());
        m_pServer->stop();
        m_pServer->join();
        return S_EXIT;
    }
    return S_EXIT;
}

bool SearchServerAppRunner::initHttpServer()
{
    m_pServer = new HTTPSearchServer();
    if (!m_pServer->init(m_sConfFile))
    {
        return false;
    }
    return true;
}

bool SearchServerAppRunner::initRPCServer()
{
#ifdef HAVE_THRIFT
    m_pServer = new RPCSearchServer();
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

void SearchServerAppRunner::registerOption(Application* pApp)
{
    pApp->addDescription("Setup search server : ");
    pApp->addDescription(tstring("  ") + pApp->getProgName() + 
                         " -c [CONF_FILE]");

    pApp->addOption(Option("conf", "c", "setup search server")
                    .required(true)
                    .repeatable(false)
                    .argument("[CONF_FILE]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("service-type", "t", "specify service type (rpc/http)")
                    .repeatable(false)
                    .argument("[SERVICE_TYPE]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 
}

void SearchServerAppRunner::optionCallback(const Option& option,
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

void SearchServerAppRunner::stop()
{
    if (m_pServer.isNotNull())
    {
        m_pServer->stop();
    }
}
 
void SearchServerAppRunner::waitStop()
{
    if (m_pServer.isNotNull())
    {
        m_pServer->join();
    }
}

FX_NS_END
