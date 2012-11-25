#include "HTTPSearchClusters.h"
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

FX_NS_USE(network);
FX_NS_USE(search);
FX_NS_USE(thread);
FX_NS_USE(utility);
FX_NS_USE(index);

FX_NS_DEF(app);

HTTPSearchClusters::HTTPSearchClusters(int32_t nTimeOut)
    : SearchClustersBase(nTimeOut)
{
}

HTTPSearchClusters::~HTTPSearchClusters()
{
}

SearchClusterBase* HTTPSearchClusters::newCluster(AsyncDeliver* pDeliver)
{
    return new HTTPSearchCluster(pDeliver);
}

FX_NS_END
