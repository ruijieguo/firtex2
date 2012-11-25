#ifndef __FX_BLENDERSERVICEBASE_H
#define __FX_BLENDERSERVICEBASE_H

#include "firtex/queryparser/QueryParser.h"
#include "firtex/utility/IconvWrapper.h"
#include "firtex/utility/URLDecoder.h"
#include "firtex/utility/BoundedConcurrentQueue.h"
#include "SearchClustersBase.h"
#include "BlenderServerConf.h"

namespace firtex 
{
class QueryCommand;
}


FX_NS_DEF(app);

class BlenderServiceBase
{
public:
    const static int32_t DEFAULT_SEARCH_TIMEOUT = 10 * 1000; // 10 seconds;

public:
    BlenderServiceBase();
    virtual ~BlenderServiceBase();

public:
    /// Initialize
    bool init(const BlenderServerConfPtr& pConf);

    /**
     * Handle a search request. This method must be thread-safe
     * @param sResult search result
     * @param sUri search request
     */
    void syntaxSearch(std::string& sResult, const std::string& sUri);

#ifdef HAVE_THRIFT
    /**
     * Handle a search request. This method must be thread-safe
     * @param sResult search result
     * @param cmd search request command
     */
    void search(std::string& sResult, const QueryCommand& cmd);
#endif

protected:
    /// Create search clusters implement
    virtual SearchClustersBase* newClusters(int32_t nTimeOut) = 0;

private:
    typedef FX_NS(utility)::BoundedConcurrentQueue<SearchClustersBasePtr> ClustersQueue;
    DEFINE_TYPED_PTR(ClustersQueue);

    ClustersQueuePtr m_pClustersQueue;

    int32_t m_nTimeout;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(BlenderServiceBase);

//////////////////////////////////////////
//

FX_NS_END

#endif //__FX_BLENDERSERVICEBASE_H
