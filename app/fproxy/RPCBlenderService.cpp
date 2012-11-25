#include "RPCBlenderService.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/search/XMLResultFormatter.h"
#include "AppendResultCombiner.h"

using namespace std;

FX_NS_DEF(app);

RPCBlenderService::RPCBlenderService()
{
}

RPCBlenderService::~RPCBlenderService()
{
}

SearchClustersBase* RPCBlenderService::newClusters(int32_t nTimeout)
{
    return new RPCSearchClusters(nTimeout);
}

void RPCBlenderService::syntaxSearch(std::string& sResult, const std::string& sUri)
{
    BlenderServiceBase::syntaxSearch(sResult, sUri);
}

void RPCBlenderService::search(std::string& sResult, const QueryCommand& cmd)
{
    BlenderServiceBase::search(sResult, cmd);
}

bool RPCBlenderService::ping()
{
    return true;
}

FX_NS_END
