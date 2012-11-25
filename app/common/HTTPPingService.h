#ifndef __FX_HTTPPINGSERVICE_H
#define __FX_HTTPPINGSERVICE_H

#include "firtex/extension/network/EvHttpService.h"
#include "firtex/extension/network/EvHttpRequestContext.h"
#include "firtex/utility/StringUtils.h"

FX_NS_DEF(app);

class HTTPPingService : public FX_NS(network)::EvHttpService
{
public:
    HTTPPingService();

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
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_HTTPPINGSERVICE_H
