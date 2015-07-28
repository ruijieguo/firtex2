#include "SearchServerBase.h"

FX_NS_USE(utility);
FX_NS_USE(thread);
FX_NS_DEF(app);

SETUP_LOGGER(app, SearchServerBase);

SearchServerBase::SearchServerBase() 
{
}

SearchServerBase::~SearchServerBase() 
{
}

bool SearchServerBase::init(const std::string& sConfFile)
{
    if (!loadConf(sConfFile))
    {
        return false;
    }
    setupLogger();

    try
    {
        FX_LOG(INFO, "Begin load search resources.");
        m_resource.init(m_conf);
        FX_LOG(INFO, "End load search resources.");
    }
    catch(const FirteXException& e)
    {
        FX_LOG(ERROR, "Initialize search resource FAILED, configure file: [%s],"
               " reason: [%s]", sConfFile.c_str(), e.what().c_str());
        return false;
    }

    if (GLOBAL_CONF().Search.refresh_timer > 0)
    {
        initRefreshThread(GLOBAL_CONF().Search.refresh_timer);
    }
    return true;
}

void SearchServerBase::stop()
{
    if (m_pRefreshRunner)
    {
        m_pRefreshRunner->stop();
    }
}

void SearchServerBase::join()
{
    if (m_pRefreshThread)
    {
        m_pRefreshThread->join();
    }
}

void SearchServerBase::initRefreshThread(int32_t nRefreshTimer)
{
    m_pRefreshRunner.reset(new RefreshRunner(m_resource, 
                    nRefreshTimer));
    m_pRefreshThread.reset(new Thread());
    m_pRefreshThread->start(*m_pRefreshRunner);
}

FX_NS_END

