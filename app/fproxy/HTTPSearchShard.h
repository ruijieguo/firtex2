#ifndef __FX_HTTPSEARCHSHARD_H
#define __FX_HTTPSEARCHSHARD_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "SearchShardBase.h"

FX_NS_DEF(app);

class HTTPSearchShard : public SearchShardBase
{
public:
    using SearchShardBase::AsyncDeliver;

public:
    HTTPSearchShard(AsyncDeliver* pDeliver);
    ~HTTPSearchShard();

public:
    /**
     * Create the replica
     */
    virtual SearchReplicaBase* newReplica();
};

DEFINE_TYPED_PTR(HTTPSearchShard);

/////////////////////////////////////////////////////

FX_NS_END

#endif
