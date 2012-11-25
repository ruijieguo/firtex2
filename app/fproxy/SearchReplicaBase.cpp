#include "SearchReplicaBase.h"
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

SETUP_LOGGER(app, SearchReplicaBase);

SearchReplicaBase::SearchReplicaBase()
    : m_replicaId(-1)
    , m_nWeight(1)
{
}

SearchReplicaBase::~SearchReplicaBase()
{
}

bool SearchReplicaBase::init(const ProxyServerConf::sReplica& repConf)
{
    FX_LOG(INFO, "Init replica: [%s]", repConf.server.c_str());

    m_nWeight = repConf.weight;

    return SearchServiceClient::init(repConf.server);
}

FX_NS_END
