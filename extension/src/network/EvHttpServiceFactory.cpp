#include "firtex/extension/network/EvHttpServiceFactory.h"
#include "firtex/extension/network/EchoService.h"

using namespace std;

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
    for (ServiceMap::iterator it = m_services.begin();
         it != m_services.end(); ++it)
    {
        delete it->second;
    }
    m_services.clear();
}

void EvHttpServiceFactory::registerService(EvHttpService* pService)
{
    string sRequest = pService->requestCanHandle();
    ServiceMap::iterator it = m_services.find(sRequest);
    if (it != m_services.end())
    {
        FX_LOG(WARN, "The service for request: [%s] already exists, "
               "update to new service", sRequest.c_str());
        delete it->second;
        m_services.erase(it);
    }
    
    m_services.insert(make_pair(sRequest, pService));
}

EvHttpService* EvHttpServiceFactory::findService(
        const std::string& sRequest) const
{
    ServiceMap::const_iterator it = m_services.find(sRequest);
    if (it != m_services.end())
    {
        return it->second;
    }
    return NULL;
}

FX_NS_END
