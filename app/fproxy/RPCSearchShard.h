#ifndef __FX_RPCSEARCHSHARD_H
#define __FX_RPCSEARCHSHARD_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "SearchShardBase.h"

FX_NS_DEF(app);

class RPCSearchShard : public SearchShardBase
{
public:
    using SearchShardBase::AsyncDeliver;

public:
    RPCSearchShard(AsyncDeliver* pDeliver);
    ~RPCSearchShard();

public:
    /**
     * Create the replica
     */
    virtual SearchReplicaBase* newReplica();
};

DEFINE_TYPED_PTR(RPCSearchShard);

/////////////////////////////////////////////////////

FX_NS_END

#endif
