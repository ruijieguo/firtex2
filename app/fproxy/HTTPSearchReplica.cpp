#include "HTTPSearchReplica.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/extension/network/NetworkException.h"
#include "firtex/search/XMLResultFormatter.h"

using namespace std;
FX_NS_USE(network);
FX_NS_USE(utility);
FX_NS_USE(thread);
FX_NS_USE(search);

FX_NS_DEF(app);

SETUP_LOGGER(app, HTTPSearchReplica);

HTTPSearchReplica::HTTPSearchReplica()
{
}

HTTPSearchReplica::~HTTPSearchReplica()
{
    if (m_pSearchClient)
    {
        m_pSearchClient->close();
    }
}

void HTTPSearchReplica::initClient(const std::string& sAddr, int32_t nPort)
{
    FX_DEBUG("Setup connection to [%s : %u].", sAddr.c_str(), nPort);

    m_pSearchClient.reset(new EvHttpSyncClient());
    m_pSearchClient->open(sAddr, nPort);
}

HTTPSearchReplica::ErrorCode 
HTTPSearchReplica::search(std::string& sResponse, 
                          const std::string& sRequest)
{
    FX_DEBUG("Shard send request : [%s] to searcher: [%s : %u].",
             sRequest.c_str(), m_pSearchClient->getHost().c_str(),
             m_pSearchClient->getPort());
    m_pSearchClient->get("/search?" + sRequest);
    if (m_pSearchClient->good())
    {
        FX_TRACE("Receive response success.");
        sResponse = m_pSearchClient->getResponse();
        return EC_OK;
    }
    else if (m_pSearchClient->timeout())
    {
        FX_LOG(ERROR, "Receive response timeout: [%s], [%s].",
               m_pSearchClient->getHost().c_str(), 
               m_pSearchClient->getLastErrorMessage().c_str());

        formatError(sResponse, m_pSearchClient->getLastErrorMessage());
        return EC_TIMEOUT;
    }
    else if (m_pSearchClient->error())
    {
        FX_LOG(ERROR, "Receive response from: [%s] FAILED: [%s].",
               m_pSearchClient->getHost().c_str(), 
               m_pSearchClient->getLastErrorMessage().c_str());

        formatError(sResponse, m_pSearchClient->getLastErrorMessage());
        return EC_ERROR;
    }
    return EC_OK;
}

bool HTTPSearchReplica::ping()
{
    m_pSearchClient->get("/ping");
    if (m_pSearchClient->good())
    {
        FX_TRACE("Receive response success.");
        return true;
    }
    else if (m_pSearchClient->timeout())
    {
        FX_LOG(INFO, "Ping the server timeout: [%s], [%s].",
               m_pSearchClient->getHost().c_str(), 
               m_pSearchClient->getLastErrorMessage().c_str());
        return false;
    }
    else if (m_pSearchClient->error())
    {
        FX_LOG(INFO, "Receive ping response from: [%s] FAILED: [%s].",
               m_pSearchClient->getHost().c_str(), 
               m_pSearchClient->getLastErrorMessage().c_str());

        return false;
    }
    return false;
}

FX_NS_END
