#include "RPCProxyService.h"
#include "RPCSearchShards.h"

using namespace std;

FX_NS_DEF(app);

RPCProxyService::RPCProxyService()
{
}

RPCProxyService::~RPCProxyService()
{
}

SearchShardsBase* RPCProxyService::newShards(int32_t nTimeout)
{
    return new RPCSearchShards(nTimeout);
}

void RPCProxyService::syntaxSearch(std::string& sResult, const std::string& sUri)
{
    return ProxyServiceBase::syntaxSearch(sResult, sUri);
}

void RPCProxyService::search(std::string& sResult, const QueryCommand& cmd)
{
    return ProxyServiceBase::search(sResult, cmd);
}

bool RPCProxyService::ping()
{
    return true;
}

FX_NS_END
