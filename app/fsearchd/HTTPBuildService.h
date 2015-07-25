#ifndef __FX_HTTPBUILDSERVICE_H
#define __FX_HTTPBUILDSERVICE_H

#include "SearchResource.h"
#include "firtex/extension/network/EvHttpService.h"
#include "firtex/extension/network/EvHttpRequestContext.h"

FX_NS_DEF(app);

class HTTPBuildService : public FX_NS(network)::EvHttpService
{
public:
    HTTPBuildService(const SearchResource& searchRes);

public:
    /**
     * Return requsets this service can handle
     * @return the requests string
     */
     std::string requestCanHandle() const;

    /**
     * Handle a http request. This method must be thread-safe
     * @param pCtx request context
     */
    virtual void handleRequest(FX_NS(network)::EvHttpRequestContext* pCtx);

private:
    void sendResponse(const std::string& sMsg,
                      FX_NS(network)::EvHttpRequestContext* pCtx) const;

    void sendErrorMessage(const std::string& sErrorMsg,
                          FX_NS(network)::EvHttpRequestContext* pCtx) const;

private:
    const SearchResource& m_searchRes;

private:
    DECLARE_STREAM_LOGGER();
};

//////////////////////////////////////////
//

inline void HTTPBuildService::sendResponse(const std::string& sMsg,
        FX_NS(network)::EvHttpRequestContext* pCtx) const
{
    pCtx->getOutBuffer()->addData(sMsg.c_str(), sMsg.length());
}

FX_NS_END

#endif //__FX_SEARCH_SERVICE_H
