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

    SearchServerConfPtr pSearchConf = m_pConf.cast<SearchServerConf>();
    try
    {
        FX_LOG(INFO, "Begin load index: [%s]", 
               pSearchConf->resourceConf.index_data.c_str());
        m_resource.init(*pSearchConf);
        FX_LOG(INFO, "End load index: [%s]", 
               pSearchConf->resourceConf.index_data.c_str());
    }
    catch(const FirteXException& e)
    {
        FX_LOG(ERROR, "Initialize search resource FAILED, configure file: [%s],"
               " reason: [%s]", sConfFile.c_str(), e.what().c_str());
        return false;
    }

    if (pSearchConf->resourceConf.refresh_timer > 0)
    {
        initRefreshThread();
    }
    return true;
}

void SearchServerBase::stop()
{
    if (m_pRefreshRunner.isNotNull())
    {
        m_pRefreshRunner->stop();
    }
}

void SearchServerBase::join()
{
    if (m_pRefreshThread.isNotNull())
    {
        m_pRefreshThread->join();
    }
}

ServerConfBase* SearchServerBase::createConf()
{
    return new SearchServerConf();
}

void SearchServerBase::initRefreshThread()
{
    SearchServerConfPtr pSearchConf = m_pConf.cast<SearchServerConf>();
    m_pRefreshRunner = new RefreshRunner(m_resource, 
            pSearchConf->resourceConf.refresh_timer);
    m_pRefreshThread = new Thread();
    m_pRefreshThread->start(*m_pRefreshRunner);
}

FX_NS_END

