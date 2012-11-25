#ifndef __FX_HTTPSEARCHSHARDS_H
#define __FX_HTTPSEARCHSHARDS_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "SearchShardsBase.h"
#include <vector>
#include <sstream>

FX_NS_DEF(app);

class HTTPSearchShards : public SearchShardsBase
{
public:
    using SearchShardsBase::AsyncDeliver;

public:
    HTTPSearchShards(int32_t nTimeOut);
    ~HTTPSearchShards();

public:
    /**
     * Create search shard implement
     *
     * @param pDeliver async request deliver
     */
    virtual SearchShardBase* newShard(AsyncDeliver* pDeliver);
};

DEFINE_TYPED_PTR(HTTPSearchShards);

/////////////////////////////////////////////////////
/// Inline functions
FX_NS_END

#endif
