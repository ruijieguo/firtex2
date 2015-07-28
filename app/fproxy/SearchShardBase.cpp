#include "SearchShardBase.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/extension/network/NetworkException.h"
#include "LoadBallancePolicyFactory.h"
#include <algorithm>

FX_NS_USE(network);
FX_NS_USE(utility);
FX_NS_USE(thread);

FX_NS_DEF(app);

SETUP_LOGGER(app, SearchShardBase);

SearchShardBase::SearchShardBase(AsyncDeliver* pDeliver)
    : m_shardId(INVALID_SHARDID)
    , m_status(SearchReplicaBase::EC_ERROR)
    , m_pDeliver(pDeliver)
{
}

SearchShardBase::~SearchShardBase()
{
}

bool SearchShardBase::init(const ProxyServerConf::sShard& shardConf, 
                           const std::string& sLoadBallancePolicy)
{
    m_sName = shardConf.name;

    int32_t repId = 0;
    for (ProxyServerConf::ReplicaList::const_iterator it = shardConf.replicas.begin();
         it != shardConf.replicas.end(); ++it, ++repId)
    {

        SearchReplicaBasePtr pReplica(newReplica());
        if (!pReplica->init(*it))
        {
            FX_LOG(ERROR, "Init replica: [%s] FAILED.", it->server.c_str());
            return false;
        }
        pReplica->setReplicaId(repId);
        m_replicas.push_back(pReplica);
    }

    std::string sPolicy = sLoadBallancePolicy;
    if (sPolicy.empty())
    {
        sPolicy = "load_ballance.round_robin";
    }
    m_pBallancePolicy = LoadBallancePolicyFactory::instance()->create(sPolicy);
    m_pBallancePolicy->init(m_replicas);
    return true;
}

void SearchShardBase::asyncSearch(const std::string& sRequest)
{
    m_sRequest = sRequest;
    m_pDeliver->deliver(this);
}

void SearchShardBase::run()
{
    do
    {
        SearchReplicaBasePtr pReplica = m_pBallancePolicy->select();
        if (!pReplica)
        {
            FX_LOG(ERROR, "No replica to select: shard: [%d]", m_shardId);

            m_pActiveReplica.reset();
            SearchReplicaBase::formatError(m_sResponse, "No replica to select");
            m_status = SearchReplicaBase::EC_ERROR;
            break;
        }
        else
        {
            m_pActiveReplica = pReplica;
            m_status = pReplica->search(m_sResponse, m_sRequest);
            if (m_status == SearchReplicaBase::EC_OK)
            {
                break;
            }
            else
            {
                FX_LOG(ERROR, "Replica search FAILED, shard: [%d], "
                       "replica: [%d]", m_shardId, pReplica->getReplicaId());
                {
                    FastMutex::Guard g(m_lock);
                    m_unavailableReplicas.push_back(pReplica);
                }
                m_pBallancePolicy->markUnavailable(pReplica);
            }
        }
    } while (true);
}

void SearchShardBase::checkUnavailableReplicas()
{
    FastMutex::Guard g(m_lock);

    ReplicaVector availReps;
    availReps.reserve(m_unavailableReplicas.size());

    for (ReplicaVector::iterator it = m_unavailableReplicas.begin();
         it != m_unavailableReplicas.end(); ++it)
    {
        if ((*it)->ping())
        {
            m_pBallancePolicy->markAvailable(*it);
            availReps.push_back(*it);            
        }
    }

    for (ReplicaVector::const_iterator it = availReps.begin();
         it != availReps.end(); ++it)
    {
        std::remove(m_unavailableReplicas.begin(), m_unavailableReplicas.end(), *it);
    }
}

FX_NS_END
