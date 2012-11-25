#include "firtex/extension/network/EchoService.h"
#include "firtex/extension/network/EvHttpRequestContext.h"
#include "firtex/utility/StringUtils.h"

FX_NS_USE(utility);
FX_NS_DEF(network);

EchoService::EchoService() 
{
}

EchoService::~EchoService() 
{
}

std::string EchoService::requestCanHandle() const
{
    return "/echo";
}

void EchoService::handleRequest(EvHttpRequestContext* pCtx)
{
    EvDataBuffer* pOutBuf = pCtx->getOutBuffer();
    const std::string q = pCtx->getQuery();
    pOutBuf->addData(q.c_str(), q.length());
}

FX_NS_END
