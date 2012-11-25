#include "HTTPBlenderService.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/search/XMLResultFormatter.h"
#include "AppendResultCombiner.h"

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(network);
FX_NS_USE(search);

FX_NS_DEF(app);

HTTPBlenderService::HTTPBlenderService()
{
}

HTTPBlenderService::~HTTPBlenderService()
{
}

std::string HTTPBlenderService::requestCanHandle() const
{
    return "/search";
}

void HTTPBlenderService::handleRequest(EvHttpRequestContext* pCtx)
{
    string sResult;
    syntaxSearch(sResult, pCtx->getQuery());
    pCtx->getOutBuffer()->addData(sResult.c_str(), sResult.length());
}

SearchClustersBase* HTTPBlenderService::newClusters(int32_t nTimeout)
{
    return new HTTPSearchClusters(nTimeout);
}

FX_NS_END
