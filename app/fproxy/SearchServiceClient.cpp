#include "SearchServiceClient.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/extension/network/NetworkException.h"
#include "firtex/search/XMLResultFormatter.h"

#ifdef WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#else
#include <arpa/inet.h>
#endif

#ifdef HAVE_THRIFT
#include <thrift/protocol/TBinaryProtocol.h>
#endif

using namespace std;

FX_NS_USE(utility);
FX_NS_USE(thread);
FX_NS_USE(search);

FX_NS_DEF(app);

SETUP_LOGGER(app, SearchServiceClient);

SearchServiceClient::SearchServiceClient()
{
}

SearchServiceClient::~SearchServiceClient()
{
}

bool SearchServiceClient::init(const std::string& sHostAndPort)
{
    StringTokenizer st(sHostAndPort, ":", StringTokenizer::TOKEN_IGNORE_EMPTY
                       | StringTokenizer::TOKEN_TRIM);
    if (st.getNumTokens() != 2)
    {
        FX_LOG(ERROR, "Invalid host configure: [%s]",
               sHostAndPort.c_str());
        return false;
    }

    std::string sAddr = st[0];
    std::string sPort = st[1];
    int32_t nPort = 0;
    try 
    {
        if (!NumberParser::tryParseInt32(sPort, nPort))
        {
            FX_LOG(ERROR, "Invalid port number: [%s]", sPort.c_str());
            return false;
        }
        
        FX_DEBUG("Setup connection to [%s : %d].", sAddr.c_str(), nPort);
        initClient(sAddr, nPort);
    }
    catch(const NetworkException& e)
    {
        FX_LOG(ERROR, "Connect to [%s:%u] FAILED, reason: [%s].", 
               sAddr.c_str(), nPort, e.what().c_str());
        return false;
    }

    return true;
}

//static 
void SearchServiceClient::formatError(string& sResult, const string& sErrorMsg)
{
    stringstream ss;
    XMLResultFormatter formatter;
    QueryResult qResult;
    qResult.setError(sErrorMsg);
    formatter.format(qResult, ss);
    sResult = ss.str();
}

FX_NS_END
