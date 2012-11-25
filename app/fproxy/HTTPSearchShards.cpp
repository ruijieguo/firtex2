#include "HTTPSearchShards.h"
#include "HTTPSearchShard.h"
#include <sstream>

using namespace std;

FX_NS_DEF(app);

HTTPSearchShards::HTTPSearchShards(int32_t nTimeOut)
    : SearchShardsBase(nTimeOut)
{
}

HTTPSearchShards::~HTTPSearchShards()
{
}

SearchShardBase* HTTPSearchShards::newShard(AsyncDeliver* pDeliver)
{
    return new HTTPSearchShard(pDeliver);
}

FX_NS_END
