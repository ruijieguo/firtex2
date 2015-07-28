#include "RPCSearchReplica.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/extension/network/NetworkException.h"
#include <thrift/protocol/TBinaryProtocol.h>

using namespace std;
using namespace apache;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

FX_NS_USE(network);
FX_NS_USE(utility);
FX_NS_USE(thread);

FX_NS_DEF(app);

SETUP_LOGGER(app, RPCSearchReplica);

RPCSearchReplica::RPCSearchReplica()
{
}

RPCSearchReplica::~RPCSearchReplica()
{
    if (m_pSearchClient)
    {
        m_pSearchClient->close();
    }
}

void RPCSearchReplica::initClient(const std::string& sAddr, int32_t nPort)
{
    FX_DEBUG("Setup connection to [%s : %u].", sAddr.c_str(), nPort);

    m_pSearchClient.reset(new RPCClient());
    m_pSearchClient->open(sAddr, nPort);
}

RPCSearchReplica::ErrorCode RPCSearchReplica::search(
        std::string& sResponse, const std::string& sRequest)
{
    FX_DEBUG("Replica send request : [%s] to searcher: [%s : %u].",
             sRequest.c_str(), m_pSearchClient->getHost().c_str(),
             m_pSearchClient->getPort());
    try
    {
        m_pSearchClient->syntaxSearch(sResponse, sRequest);
    }
    catch(const TTransportException& e)
    {
        if (e.getType() == TTransportException::TIMED_OUT)
        {
            FX_LOG(ERROR, "Receive response timeout: [%s], [%s].",
                   m_pSearchClient->getHost().c_str(), e.what());            
            formatError(sResponse, e.what());
            return EC_TIMEOUT;
        }

        FX_LOG(ERROR, "Receive response from: [%s] FAILED: [%s].",
               m_pSearchClient->getHost().c_str(), e.what());
        formatError(sResponse, e.what());
        return EC_ERROR;
    }
    catch (const TException& e)
    {
        FX_LOG(ERROR, "Receive response from: [%s] FAILED: [%s].",
               m_pSearchClient->getHost().c_str(), e.what());
        formatError(sResponse, e.what());
        return EC_ERROR;
    }

    FX_TRACE("Receive response success.");
    return EC_OK;
}

bool RPCSearchReplica::ping()
{
    try
    {
        return m_pSearchClient->ping();
    }
    catch(const TTransportException& e)
    {
        if (e.getType() == TTransportException::TIMED_OUT)
        {
            FX_LOG(INFO, "Ping the server timeout: [%s], [%s].",
                   m_pSearchClient->getHost().c_str(), e.what());            
            return false;
        }

        FX_LOG(INFO, "Receive ping response from: [%s] FAILED: [%s].",
               m_pSearchClient->getHost().c_str(), e.what());
        return false;
    }
    catch (const TException& e)
    {
        FX_LOG(INFO, "Receive ping response from: [%s] FAILED: [%s].",
               m_pSearchClient->getHost().c_str(), e.what());
        return false;
    }
    return false;
}

FX_NS_END
