#include "HTTPPingService.h"

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(network);

FX_NS_DEF(app);

SETUP_LOGGER(app, HTTPPingService);

HTTPPingService::HTTPPingService()
{
}

std::string HTTPPingService::requestCanHandle() const
{
    return "/ping";
}

void HTTPPingService::handleRequest(EvHttpRequestContext* pCtx)
{
    static string OK = "OK";
    pCtx->setResponseCode(HTTPTypes::RESPONSE_CODE_OK);
    pCtx->getOutBuffer()->addData(OK.c_str(), OK.length());
}

FX_NS_END
