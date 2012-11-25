#include "RPCSearchShards.h"
#include "RPCSearchShard.h"
#include <sstream>

using namespace std;

FX_NS_DEF(app);

RPCSearchShards::RPCSearchShards(int32_t nTimeOut)
    : SearchShardsBase(nTimeOut)
{
}

RPCSearchShards::~RPCSearchShards()
{
}

SearchShardBase* RPCSearchShards::newShard(AsyncDeliver* pDeliver)
{
    return new RPCSearchShard(pDeliver);
}

FX_NS_END
