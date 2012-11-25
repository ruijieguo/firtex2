#ifndef __FX_HTTPSEARCHCLUSTERS_H
#define __FX_HTTPSEARCHCLUSTERS_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "HTTPSearchCluster.h"
#include "BlenderServerConf.h"
#include "SearchClustersBase.h"

FX_NS_DEF(app);

class HTTPSearchClusters : public SearchClustersBase
{
public:
    using SearchClustersBase::AsyncDeliver;

public:
    HTTPSearchClusters(int32_t nTimeOut);
    ~HTTPSearchClusters();

protected:
    /// Create search cluster
    virtual SearchClusterBase* newCluster(AsyncDeliver* pDeliver);
};

DEFINE_TYPED_PTR(HTTPSearchClusters);

/////////////////////////////////////////////////////
/// Inline functions

FX_NS_END

#endif
