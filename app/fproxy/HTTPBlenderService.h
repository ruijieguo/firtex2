#ifndef __FX_HTTPBLENDERSERVICE_H
#define __FX_HTTPBLENDERSERVICE_H

#include "HTTPSearchClusters.h"
#include "BlenderServerConf.h"
#include "BlenderServiceBase.h"
#include "firtex/extension/network/EvHttpService.h"

FX_NS_DEF(app);

class HTTPBlenderService : public FX_NS(network)::EvHttpService, public BlenderServiceBase
{
public:
    HTTPBlenderService();
    ~HTTPBlenderService();

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
    /// Create search clusters implement
    virtual SearchClustersBase* newClusters(int32_t nTimeout);
};

DEFINE_TYPED_PTR(HTTPBlenderService);

//////////////////////////////////////////
//

FX_NS_END

#endif //__FX_HTTPBLENDERSERVICE_H
