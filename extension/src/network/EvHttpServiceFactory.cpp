#include "firtex/extension/network/EvHttpServiceFactory.h"
#include "firtex/extension/network/EchoService.h"

using namespace std;

FX_NS_USE(util);

FX_NS_DEF(network);

std::string EvHttpServiceFactory::NOT_FOUND_SERVICE_NAME = "NotFoundService";
std::string EvHttpServiceFactory::BAD_REQUEST_SERVICE_NAME = "BadRequestService";

SETUP_STREAM_LOGGER(network, EvHttpServiceFactory);

EvHttpServiceFactory::EvHttpServiceFactory() 
{
    registerService(new EchoService());
}

EvHttpServiceFactory::~EvHttpServiceFactory() 
{
    m_services.clear();
}

void EvHttpServiceFactory::registerService(EvHttpService* pService)
{
    EvHttpServicePtr pServicePtr(pService);

    string sRequest = pService->requestCanHandle();
    RouterPtr pRouter(new Router());
    if (pRouter->init(sRequest))
    {
        FX_LOG(INFO, "Register service for [%s] SUCCESS.", sRequest.c_str());
        m_services.push_back(make_pair(pRouter, pServicePtr));
    }
    else
    {
        FX_LOG(ERROR, "Register service for [%s] FAILED.", sRequest.c_str());
    }
}

EvHttpService* EvHttpServiceFactory::findService(
        const EvHttpRequestContext* pCtx) const
{
    string sRequest = pCtx->getResource();
    for (ServiceVector::const_iterator it = m_services.begin();
         it != m_services.end(); ++it)
    {
        if ((*it).first->match(pCtx->getMethodType(), sRequest))
        {
            return const_cast<EvHttpService*>((*it).second.get());
        }
    }
    return NULL;
}

FX_NS_END
