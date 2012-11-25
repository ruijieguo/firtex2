#include "RPCSearchShard.h"
#include "RPCSearchReplica.h"

FX_NS_DEF(app);

RPCSearchShard::RPCSearchShard(AsyncDeliver* pDeliver)
    : SearchShardBase(pDeliver)
{
}

RPCSearchShard::~RPCSearchShard()
{
}

SearchReplicaBase* RPCSearchShard::newReplica()
{
    return new RPCSearchReplica();
}

FX_NS_END
