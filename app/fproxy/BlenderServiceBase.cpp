#include "BlenderServiceBase.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/search/XMLResultFormatter.h"
#include "AppendResultCombiner.h"

#ifdef HAVE_THRIFT
#include "../thrift/ThriftSearchServiceHelper.h"
#endif 

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(search);

FX_NS_DEF(app);

SETUP_LOGGER(app, BlenderServiceBase);

BlenderServiceBase::BlenderServiceBase()
    : m_nTimeout(DEFAULT_SEARCH_TIMEOUT)
{
}

BlenderServiceBase::~BlenderServiceBase()
{
}

bool BlenderServiceBase::init(const BlenderServerConfPtr& pConf)
{
    FX_LOG(INFO, "Initialize RPC blender service, work thread count: [%d]",
           pConf->serverConf.thread_pool);

    int32_t nThreadCount = pConf->serverConf.thread_pool;
    m_pClustersQueue = new ClustersQueue(nThreadCount + 1);

    FX_LOG(INFO, "Setup process queue, size: [%d]",
           nThreadCount + 1);

    for (int32_t i = 0; i < nThreadCount + 1; ++i)
    {
        FX_LOG(INFO, "Setup the [%d] processor", i);
        SearchClustersBasePtr pSearchShards = newClusters(m_nTimeout);
        if (!(pSearchShards->init(pConf->resourceConf)))
        {
            FX_LOG(ERROR, "Init cluster FAILED.");
            return false;
        }
        m_pClustersQueue->enqueue(pSearchShards);
    }
    return true;
}

void BlenderServiceBase::syntaxSearch(std::string& sResult, const std::string& sUri)
{
    FX_TRACE("Blender receive search request: [%s].", sUri.c_str());

    SearchClustersBasePtr pClusters = m_pClustersQueue->waitDequeue();
    if (pClusters.isNotNull())
    {
        pClusters->search(sResult, sUri);
        m_pClustersQueue->enqueue(pClusters);
    }
}

#ifdef HAVE_THRIFT
void BlenderServiceBase::search(std::string& sResult, const QueryCommand& cmd)
{
    string sStat;
    sStat.reserve(256);
    ThriftSearchServiceHelper::queryCommandToStatmentString(sStat, cmd);
    syntaxSearch(sResult, sStat);
}
#endif

FX_NS_END
