#include "SearchClusterBase.h"
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

SETUP_LOGGER(app, SearchClusterBase);

SearchClusterBase::SearchClusterBase(AsyncDeliver* pDeliver)
    : m_status(SearchServiceClient::EC_ERROR)
    , m_pDeliver(pDeliver)
{
}

SearchClusterBase::~SearchClusterBase()
{
}

bool SearchClusterBase::init(const BlenderServerConf::sCluster& clusterConf)
{
    FX_LOG(INFO, "Init cluster: [%s]", clusterConf.name.c_str());

    m_sClusterName = clusterConf.name;

    return SearchServiceClient::init(clusterConf.server);
}

void SearchClusterBase::asyncSearch(const std::string& sRequest)
{
    m_sRequest = sRequest;
    m_pDeliver->deliver(this);
}

FX_NS_END
