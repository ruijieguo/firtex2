#include "RoundRobinPolicy.h"

using namespace std;
FX_NS_USE(thread);
FX_NS_DEF(app);

SETUP_LOGGER(app, RoundRobinPolicy);

RoundRobinPolicy::RoundRobinPolicy() 
    : m_nActive((size_t)-1)
{
}

RoundRobinPolicy::~RoundRobinPolicy() 
{
}

void RoundRobinPolicy::init(const ReplicaVector& replicas)
{
    m_replicas.reserve(replicas.size());
    for (ReplicaVector::const_iterator it = replicas.begin();
         it != replicas.end(); ++it)
    {
        m_replicas.push_back(make_pair(*it, AVAILABLE));
    }
}

SearchReplicaBasePtr RoundRobinPolicy::select()
{
    ScopedRWLock g(m_rwlock, false);
    if (m_replicas.size() == 0)
    {
        return SearchReplicaBasePtr();
    }
    size_t nTryCount = 0;
    while (nTryCount < m_replicas.size())
    {
        m_nActive = ((m_nActive + 1) % m_replicas.size());
        if (m_replicas[m_nActive].second == AVAILABLE)
        {
            FX_DEBUG("Select the [%d] replica, this: %p", 
                     (int32_t)m_nActive, this);
            return m_replicas[m_nActive].first;
        }
        ++nTryCount;
    }
    return SearchReplicaBasePtr();
}

void RoundRobinPolicy::markAvailable(const SearchReplicaBasePtr& pReplica)
{
    markState(pReplica, AVAILABLE);
}

void RoundRobinPolicy::markUnavailable(const SearchReplicaBasePtr& pReplica)
{
    markState(pReplica, UNAVAILABLE);
}

void RoundRobinPolicy::markState(const SearchReplicaBasePtr& pReplica, State s)
{
    ScopedRWLock g(m_rwlock, true);
    for (RepPairVector::iterator it = m_replicas.begin();
         it != m_replicas.end(); ++it)
    {
        if (it->first == pReplica)
        {
            it->second = s;
            break;
        }
    }
}


FX_NS_END

