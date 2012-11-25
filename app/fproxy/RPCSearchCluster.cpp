#include "RPCSearchCluster.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/extension/network/NetworkException.h"
#include "firtex/search/XMLResultFormatter.h"
#include <thrift/protocol/TBinaryProtocol.h>

using namespace std;
using namespace apache;
using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

FX_NS_USE(utility);
FX_NS_USE(thread);
FX_NS_USE(search);

FX_NS_DEF(app);

SETUP_LOGGER(app, RPCSearchCluster);

RPCSearchCluster::RPCSearchCluster(AsyncDeliver* pDeliver)
    : SearchClusterBase(pDeliver)
{
}

RPCSearchCluster::~RPCSearchCluster()
{
    if (m_pSearchClient.isNotNull())
    {
        m_pSearchClient->close();
    }
}

void RPCSearchCluster::run()
{
    FX_DEBUG("Shard send request : [%s] to searcher: [%s : %u].",
             m_sRequest.c_str(), getHost().c_str(), getPort());

    try
    {
        m_pSearchClient->syntaxSearch(m_sResponse, m_sRequest);
    }
    catch(const TTransportException& e)
    {
        if (e.getType() == TTransportException::TIMED_OUT)
        {
            FX_LOG(ERROR, "Receive response timeout: [%s], [%s].",
                   m_pSearchClient->getHost().c_str(), e.what());            
            formatError(m_sResponse, e.what());
            m_status = EC_TIMEOUT;
        }

        FX_LOG(ERROR, "Receive response from: [%s] FAILED: [%s].",
               m_pSearchClient->getHost().c_str(), e.what());
        formatError(m_sResponse, e.what());
        m_status = EC_ERROR;
    }
    catch (const TException& e)
    {
        FX_LOG(ERROR, "Receive response from: [%s] FAILED: [%s].",
               m_pSearchClient->getHost().c_str(), e.what());
        formatError(m_sResponse, e.what());
    }

    FX_TRACE("Receive response success.");
    m_status = EC_OK;
}

void RPCSearchCluster::initClient(const std::string& sAddr, int32_t nPort)
{
    m_pSearchClient = new RPCClient();
    m_pSearchClient->open(sAddr, nPort);
}

FX_NS_END
