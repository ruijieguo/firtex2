#ifndef __FX_PRCPROXYSERVICE_H
#define __FX_PRCPROXYSERVICE_H

#include "ProxyServerConf.h"
#include "firtex/queryparser/QueryParser.h"
#include "firtex/utility/IconvWrapper.h"
#include "firtex/utility/URLDecoder.h"
#include "firtex/extension/network/EvHttpService.h"
#include "ProxyServiceBase.h"

FX_NS_DEF(app);

class HTTPProxyService : public FX_NS(network)::EvHttpService, public ProxyServiceBase
{
public:
    HTTPProxyService();
    ~HTTPProxyService();
public:
    /**
     * Return requsets this service can handle
     * format: request1, request2....
     * @return the requests string
     */
    virtual std::string requestCanHandle() const;

    /**
     * Handle a http request. This method must be thread-safe
     * @param pCtx request context
     */
    virtual void handleRequest(FX_NS(network)::EvHttpRequestContext* pCtx);

protected:
    /// Create search shards implement
    virtual SearchShardsBase* newShards(int32_t nTimeout);
};

DEFINE_TYPED_PTR(HTTPProxyService);

//////////////////////////////////////////
//

FX_NS_END

#endif //__FX_PRCPROXYSERVICE_H
