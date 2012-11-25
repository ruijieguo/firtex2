#ifndef __FX_PROXYSERVICEBASE_H
#define __FX_PROXYSERVICEBASE_H

#include "ProxyServerConf.h"
#include "firtex/queryparser/QueryParser.h"
#include "firtex/utility/IconvWrapper.h"
#include "firtex/utility/URLDecoder.h"
#include "firtex/utility/BoundedConcurrentQueue.h"
#include "SearchShardsBase.h"

namespace firtex 
{
class QueryCommand;
}

FX_NS_DEF(app);

class ProxyServiceBase
{
public:
    const static int32_t DEFAULT_SEARCH_TIMEOUT = 10 * 1000; // 10 seconds;

public:
    ProxyServiceBase();
    virtual ~ProxyServiceBase();

public:
    /// Initialize
    bool init(const ProxyServerConfPtr& pConf);

protected:
    /// Create search shards implement
    virtual SearchShardsBase* newShards(int32_t nTimeout) = 0;

public:
    void syntaxSearch(std::string& sResult, const std::string& sUri);
#ifdef HAVE_THRIFT
    void search(std::string& sResult, const QueryCommand& cmd);
#endif

private:
    typedef FX_NS(utility)::BoundedConcurrentQueue<SearchShardsBasePtr> ClusterQueue;
    DEFINE_TYPED_PTR(ClusterQueue);

    ClusterQueuePtr m_pClusterQueue;

    int32_t m_nTimeout;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(ProxyServiceBase);

//////////////////////////////////////////
//

FX_NS_END

#endif //__FX_PRCPROXYSERVICE_H
