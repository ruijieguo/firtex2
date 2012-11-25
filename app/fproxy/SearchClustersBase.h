#ifndef __FX_SEARCHCLUSTERSBASE_H
#define __FX_SEARCHCLUSTERSBASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/FastMutex.h"
#include "firtex/thread/Condition.h"
#include "firtex/thread/AtomicCount.h"
#include "firtex/search/Statement.h"
#include "SearchClusterBase.h"
#include "BlenderServerConf.h"
#include "ResultCombiner.h"
#include "RequestAsyncDeliver.h"
#include <vector>
#include <map>
#include <sstream>

FX_NS_DEF(app);

class SearchClustersBase
{
public:
    const static size_t MAX_RETRY_COUNT = 3;

    typedef RequestAsyncDeliver<SearchClusterBase> AsyncDeliver;

public:
    SearchClustersBase(int32_t nTimeOut);
    virtual ~SearchClustersBase();

public:
    /// Initialize search cluster
    bool init(const BlenderServerConf::sResource& resConf);

    /**
     * Handle search request by sending request to searcher shards
     */
    void search(std::string& sResult, const std::string& sUri);

protected:
    /// Create search cluster
    virtual SearchClusterBase* newCluster(AsyncDeliver* pDeliver) = 0;

private:
    typedef std::vector<SearchClusterBasePtr> ClusterVector;
    typedef std::map<std::string, SearchClusterBasePtr> ClusterMap;

private:
    void collectResult();
    void sendResult(const std::string& sResult);
    void waitComplete(std::string& sResult);
    
private:
    /// Search clusters
    ClusterVector m_clusters;
    
    /// Current selected clusters
    ClusterVector m_selectedClusters;

    /// Mapping name to clusters
    ClusterMap m_name2Clusters;

    /// Search result combiner
    ResultCombinerPtr m_pResultCombiner;

    /// Statement of current request
    FX_NS(search)::Statement m_stat;

    /// Store combined search result
    std::string m_sResult;

    /// The request deliver
    AsyncDeliver m_deliver;

    /// Search timeout, in milliseconds
    int32_t m_nTimeout;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(SearchClustersBase);

/////////////////////////////////////////////////////
/// Inline functions

FX_NS_END

#endif
