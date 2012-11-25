#ifndef __FX_RPCSEARCHSHARDS_H
#define __FX_RPCSEARCHSHARDS_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "SearchShardsBase.h"
#include <vector>
#include <sstream>

FX_NS_DEF(app);

class RPCSearchShards : public SearchShardsBase
{
public:
    using SearchShardsBase::AsyncDeliver;

public:
    RPCSearchShards(int32_t nTimeOut);
    ~RPCSearchShards();

public:
    /**
     * Create search shard implement
     *
     * @param pDeliver async request deliver
     */
    virtual SearchShardBase* newShard(AsyncDeliver* pDeliver);
};

DEFINE_TYPED_PTR(RPCSearchShards);

/////////////////////////////////////////////////////
/// Inline functions
FX_NS_END

#endif
