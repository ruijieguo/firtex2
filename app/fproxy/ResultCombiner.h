#ifndef __FX_RESULTCOMBINER_H
#define __FX_RESULTCOMBINER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(app);

class ResultCombiner
{
public:
    virtual ~ResultCombiner() {}

public:
    /// Begin result combination
    virtual void beginCombine() = 0;
    
    /**
     * Combine one result
     *
     * @param sShardId id of search shard 
     * @param sResult result of the search shard
     */
    virtual void combine(const std::string& sShardId, 
                         const std::string& sResult) = 0;

    /// extract combined result
    virtual void endCombine(std::string& sResult) = 0;
};

DEFINE_TYPED_PTR(ResultCombiner);

FX_NS_END

#endif
