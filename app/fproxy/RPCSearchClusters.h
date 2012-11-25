#ifndef __FX_RPCSEARCHCLUSTERS_H
#define __FX_RPCSEARCHCLUSTERS_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "SearchClustersBase.h"
#include "RPCSearchCluster.h"
#include "BlenderServerConf.h"
#include <sstream>

FX_NS_DEF(app);

class RPCSearchClusters : public SearchClustersBase
{
public:
    using SearchClustersBase::AsyncDeliver;

public:
    RPCSearchClusters(int32_t nTimeOut);
    ~RPCSearchClusters();

protected:
    /// Create search cluster
    virtual SearchClusterBase* newCluster(AsyncDeliver* pDeliver);

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(RPCSearchClusters);

/////////////////////////////////////////////////////
/// Inline functions

FX_NS_END

#endif
