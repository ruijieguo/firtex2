#include "HTTPSearchCluster.h"
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

SETUP_LOGGER(app, HTTPSearchCluster);

HTTPSearchCluster::HTTPSearchCluster(AsyncDeliver* pDeliver)
    : SearchClusterBase(pDeliver)
{
}

HTTPSearchCluster::~HTTPSearchCluster()
{
    if (m_pSearchClient.isNotNull())
    {
        m_pSearchClient->close();
    }
}

void HTTPSearchCluster::initClient(const std::string& sAddr, int32_t nPort)
{
    FX_DEBUG("Setup connection to [%s : %u].", sAddr.c_str(), nPort);

    m_pSearchClient = new EvHttpSyncClient();
    m_pSearchClient->open(sAddr, nPort);
}

void HTTPSearchCluster::run()
{
    FX_DEBUG("Cluster send request : [%s] to searcher: [%s : %u].",
             m_sRequest.c_str(), getHost().c_str(), getPort());

    m_pSearchClient->get("/search?" + m_sRequest);
    if (m_pSearchClient->good())
    {
        m_sResponse = m_pSearchClient->getResponse();
        FX_TRACE("Receive response success: [%s]", m_sResponse.c_str());
        m_status = EC_OK;
    }
    else if (m_pSearchClient->timeout())
    {
        FX_LOG(ERROR, "Receive response timeout: [%s], [%s].",
               m_pSearchClient->getHost().c_str(), 
               m_pSearchClient->getLastErrorMessage().c_str());

        formatError(m_sResponse, m_pSearchClient->getLastErrorMessage());
        m_status = EC_TIMEOUT;
    }
    else// if (m_pSearchClient->error())
    {
        FX_LOG(ERROR, "Receive response from: [%s] FAILED: [%s].",
               m_pSearchClient->getHost().c_str(), 
               m_pSearchClient->getLastErrorMessage().c_str());

        formatError(m_sResponse, m_pSearchClient->getLastErrorMessage());
        m_status = EC_ERROR;
    }
}

FX_NS_END
