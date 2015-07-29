#include "BuilderAppRunner.h"
#include "firtex/utility/TimeProbe.h"
#include "firtex/extension/util/SharedOptionCallback.h"
#include "firtex/extension/util/Application.h"
#include "firtex/extension/util/ServerApplication.h"
#include "firtex/extension/collection/ConsoleProgressObserver.h"

using namespace std;
FX_NS_USE(util);
FX_NS_USE(index);
FX_NS_USE(utility);
FX_NS_USE(collection);

FX_NS_DEF(app);

BuilderAppRunner::BuilderAppRunner()
    : m_pServerApp(NULL)
    , m_bMonitor(false)
{
}

AppRunner::Status BuilderAppRunner::run()
{
    if (m_sConfFile.empty())
        return S_NONE;
    if (!m_bMonitor)
    {
        bool bBlock = m_pServerApp ? (m_pServerApp->isRunAsDaemon() == false) : true;
        buildIndex(bBlock);
        return bBlock ? S_EXIT: S_RUNNING;
    }
    else
    {
        buildIndex(false);
        return S_RUNNING;
    }
}
 
void BuilderAppRunner::registerOption(Application* pApp)
{
    pApp->addDescription("Build index from configure file: ");
    pApp->addDescription(std::string("  ") + pApp->getProgName() + 
                         " -c [CONF_FILE]");

    pApp->addOption(Option("config", "c", "build index database")
                    .required(true)
                    .repeatable(false)
                    .argument("[CONF_FILE]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("monitor", "m", "monitor modification of data source")
                    .required(false)
                    .repeatable(false)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    m_pServerApp = dynamic_cast<ServerApplication*>(pApp);
}

void BuilderAppRunner::optionCallback(const Option& option,
        const std::string& sValue)
{
    if (option.matchFull("config") || option.matchShort("c"))
    {
        if (sValue.empty())
        {
            FIRTEX_THROW(MissingArgumentException, "--config/-c");
        }
        m_sConfFile = sValue;
    }

    if (option.matchFull("monitor") || option.matchShort("m"))
    {
        m_bMonitor = true;
    }
}

void BuilderAppRunner::buildIndex(bool bBlock)
{
    try
    {
        cout << "[INFO] Begin build index, config file: " << m_sConfFile << endl;
        ProgressObserverPtr pOb(new ConsoleProgressObserver(1000));
        m_pColletion = FX_NS(collection)::Collection::buildIndex(
                m_sConfFile, pOb, bBlock);
    }
    catch(const FirteXException& fe)
    {
        cout << "[ERROR] Build index FAILED: " << fe.what() << endl;
    }
}

void BuilderAppRunner::stop()
{
    if (m_pColletion)
    {
        m_pColletion->stop();
    }
}

void BuilderAppRunner::waitStop()
{
    if (m_pColletion)
    {
        m_pColletion->waitStop();
        m_pColletion.reset();
    }
}


FX_NS_END
