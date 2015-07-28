#include "ProxyServiceBase.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/search/XMLResultFormatter.h"
#include "AppendResultCombiner.h"

#ifdef HAVE_THRIFT
#include "../thrift/ThriftSearchServiceHelper.h"
#endif 

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(network);
FX_NS_USE(search);

FX_NS_DEF(app);

SETUP_LOGGER(app, ProxyServiceBase);

ProxyServiceBase::ProxyServiceBase()
    : m_nTimeout(DEFAULT_SEARCH_TIMEOUT)
{
}

ProxyServiceBase::~ProxyServiceBase()
{
}

bool ProxyServiceBase::init(const ProxyServerConfPtr& pConf)
{
    FX_LOG(INFO, "Initialize RPC proxy service, work thread count: [%d]",
           pConf->serverConf.thread_pool);

    int32_t nThreadCount = pConf->serverConf.thread_pool;
    m_pClusterQueue.reset(new ClusterQueue(nThreadCount + 1));

    FX_LOG(INFO, "Setup process queue, size: [%d]",
           nThreadCount + 1);
    for (int32_t i = 0; i < nThreadCount + 1; ++i)
    {
        FX_LOG(INFO, "Setup the [%d] processor", i);
        SearchShardsBasePtr pRPCSearchShards(newShards(m_nTimeout));
        if (!(pRPCSearchShards->init(pConf->resourceConf)))
        {
            FX_LOG(ERROR, "Init cluster FAILED.");
            return false;
        }
        m_pClusterQueue->enqueue(pRPCSearchShards);
    }
    return true;
}

#ifdef HAVE_THRIFT
void ProxyServiceBase::search(std::string& sResult, const QueryCommand& cmd)
{
    string sStat;
    sStat.reserve(256);
    ThriftSearchServiceHelper::queryCommandToStatmentString(sStat, cmd);
    syntaxSearch(sResult, sStat);
}
#endif

void ProxyServiceBase::syntaxSearch(std::string& sResult, const std::string& sUri)
{
    FX_TRACE("Proxy receive search request: [%s].", sUri.c_str());

    SearchShardsBasePtr pShards = m_pClusterQueue->waitDequeue();
    if (pShards)
    {
        pShards->syntaxSearch(sResult, sUri);
        m_pClusterQueue->enqueue(pShards);
    }
}

FX_NS_END
