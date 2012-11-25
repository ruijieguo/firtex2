#include "HTTPSearchShard.h"
#include "HTTPSearchReplica.h"

FX_NS_DEF(app);

HTTPSearchShard::HTTPSearchShard(AsyncDeliver* pDeliver)
    : SearchShardBase(pDeliver)
{
}

HTTPSearchShard::~HTTPSearchShard()
{
}

SearchReplicaBase* HTTPSearchShard::newReplica()
{
    return new HTTPSearchReplica();
}

FX_NS_END
