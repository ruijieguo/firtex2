#include "ServerBase.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/config/XMLConfigurator.h"
#include "firtex/common/Logger.h"

using namespace std;

FX_NS_USE(utility);
FX_NS_USE(common);
FX_NS_USE(config);

FX_NS_DEF(app);

SETUP_LOGGER(app, ServerBase);

ServerBase::ServerBase() 
{
}

ServerBase::~ServerBase() 
{
}

bool ServerBase::loadConf(const string& sConfFile)
{
    try
    {
        XMLConfigurator conf;
        conf.load(sConfFile);

        m_pConf = createConf();
        m_pConf->configure(conf);
        m_pConf->makePathAbsolute(conf.getDir());

        FX_LOG(INFO, "Load configure file: [%s] succeed", sConfFile.c_str());
    }
    catch(const FirteXException& e)
    {
		FX_LOG(ERROR, "Load configure file: [%s] FAILED: [%s]", sConfFile.c_str(), e.what().c_str());
        return false;
    }
    return true;
}

void ServerBase::setupLogger()
{
    string sLogConf;
    if (m_pConf.isNotNull())
        sLogConf = m_pConf->serverConf.logger_conf;
    else sLogConf = "logger_conf.xml";
    File logConf(sLogConf);
    if (!sLogConf.empty() && logConf.exists())
    {
        try
        {
            FX_LOG(INFO, "Load logger conf: [%s] succeed.", sLogConf.c_str());
            LoggerBuilder::instance()->configure(sLogConf);
        }
        catch(const FirteXException& e)
        {
            FX_LOG(ERROR, "Setup logger FAILED, conf: [%s], reason: [%s]",
                   sLogConf.c_str(), e.what().c_str());
        }
    }
}


FX_NS_END

