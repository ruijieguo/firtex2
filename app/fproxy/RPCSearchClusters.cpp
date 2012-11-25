#include "RPCSearchClusters.h"
#include "firtex/search/XMLResultFormatter.h"
#include "firtex/search/IndexSearcher.h"
#include "firtex/search/XMLResultParser.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/URLDecoder.h"
#include "firtex/utility/StringTokenizer.h"
#include "ClusterClause.h"
#include "AppendResultCombiner.h"
#include <sstream>

using namespace std;

FX_NS_USE(utility);

FX_NS_DEF(app);

SETUP_LOGGER(app, RPCSearchClusters);

RPCSearchClusters::RPCSearchClusters(int32_t nTimeOut)
    : SearchClustersBase(nTimeOut)
{
}

RPCSearchClusters::~RPCSearchClusters()
{
}

SearchClusterBase* RPCSearchClusters::newCluster(AsyncDeliver* pDeliver)
{
    return new RPCSearchCluster(pDeliver);
}

FX_NS_END
