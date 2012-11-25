#include "HTTPProxyService.h"
#include "HTTPSearchShards.h"

using namespace std;
FX_NS_USE(network);

FX_NS_DEF(app);


HTTPProxyService::HTTPProxyService()
{
}

HTTPProxyService::~HTTPProxyService()
{
}

std::string HTTPProxyService::requestCanHandle() const
{
    return "/search";
}

void HTTPProxyService::handleRequest(EvHttpRequestContext* pCtx)
{
    string sResult;
    syntaxSearch(sResult, pCtx->getQuery());
    pCtx->getOutBuffer()->addData(sResult.c_str(), sResult.length());
}

SearchShardsBase* HTTPProxyService::newShards(int32_t nTimeout)
{
    return new HTTPSearchShards(nTimeout);
}

FX_NS_END
