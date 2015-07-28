#include "SearchClustersBase.h"
#include "firtex/search/XMLResultFormatter.h"
#include "firtex/search/IndexSearcher.h"
#include "firtex/search/XMLResultParser.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/URLDecoder.h"
#include "firtex/utility/StringTokenizer.h"
#include "ClusterClause.h"
#include "AppendResultCombiner.h"
#include <sstream>

using namespace std;

FX_NS_USE(search);
FX_NS_USE(thread);
FX_NS_USE(utility);
FX_NS_USE(index);

FX_NS_DEF(app);

SETUP_LOGGER(app, SearchClustersBase);

SearchClustersBase::SearchClustersBase(int32_t nTimeOut)
    : m_nTimeout(nTimeOut)
{
    m_stat.registerClause(ClusterClause::PREFIX, new ClusterClause::Creator);
}

SearchClustersBase::~SearchClustersBase()
{
    m_deliver.stop();
}

bool SearchClustersBase::init(const BlenderServerConf::sResource& resConf)
{
    m_clusters.reserve(resConf.clusters.size());

    for (BlenderServerConf::ClusterList::const_iterator it = resConf.clusters.begin();
         it != resConf.clusters.end(); ++it)
    {
        const BlenderServerConf::sCluster& cluster = *it;
        
        SearchClusterBasePtr pCluster(newCluster(&m_deliver));
        if (!pCluster->init(cluster))
        {
            FX_LOG(ERROR, "Init cluster: [%s] FAILED.", cluster.name.c_str());
            return false;
        }
        
        m_clusters.push_back(pCluster);

        if (m_name2Clusters.find(pCluster->getName()) != m_name2Clusters.end())
        {
            FX_LOG(WARN, "Duplicated cluster name: [%s].", pCluster->getName().c_str());
        }
        else
        {
            m_name2Clusters.insert(make_pair(pCluster->getName(), pCluster));
        }
    }

    m_pResultCombiner(new AppendResultCombiner());

    m_deliver.start();

    return true;
}

void SearchClustersBase::search(std::string& sResult, const std::string& sUri)
{
    FX_TRACE("Handle search request: [%s].", sUri.c_str());

    m_stat.clearClauses();
    m_stat.fromString(sUri);

    m_selectedClusters.clear();

    ClausePtr pClause = m_stat.getClause(ClusterClause::PREFIX);
    if (pClause)
    {
        ClusterClausePtr pClusterClause = pClause.cast<ClusterClause>();
        FIRTEX_ASSERT2(pClusterClause);

        ClusterClause::Iterator it = pClusterClause->iterator();
        while (it.hasNext())
        {
            ClusterClause::ClausePair p = it.next();
            ClusterMap::iterator cIt = m_name2Clusters.find(p->first);
            if (cIt != m_name2Clusters.end())
            {
                m_selectedClusters.push_back(cIt->second);
                if (p->second)
                {
                    stringstream ss;
                    p->second->toString(ss);
                    FX_DEBUG("Send request: [%s] to cluster: [%s]",
                            ss.str().c_str(), p->first.c_str());
                    cIt->second->asyncSearch(ss.str());
                }
                else 
                {
                    FX_DEBUG("Send request: [%s] to cluster: [%s]", 
                            sUri.c_str(), p->first.c_str());
                    cIt->second->asyncSearch(sUri);
                }
            }
            else
            {
                QueryResult result;
                string sErrorMsg = "Cluster: [" + p->first + "] not found";
                FX_LOG(ERROR, "Invalid request: %s", sErrorMsg.c_str());

                result.setError(sErrorMsg);
                XMLResultFormatter formatter;
                stringstream ss;
                formatter.format(result, ss);
                m_sResult = sResult = ss.str();
                return;
            }
        } // end while
        waitComplete(sResult);
    }
    else
    {
        for (ClusterVector::iterator it = m_clusters.begin();
             it != m_clusters.end(); ++it)
        {
            FX_DEBUG("Send request to cluster: [%s]", (*it)->getName().c_str());
            (*it)->asyncSearch(sUri);
        }
        waitComplete(sResult);
    }
}

void SearchClustersBase::waitComplete(std::string& sResult)
{
    m_deliver.waitFinish();

    collectResult();
    sResult = m_sResult;
}

void SearchClustersBase::collectResult()
{
    m_pResultCombiner->beginCombine();

    ClusterVector* pClusters = (m_selectedClusters.size() > 0 )
                               ? &m_selectedClusters: &m_clusters;
    for (size_t i = 0; i < pClusters->size(); ++i)
    {
        m_pResultCombiner->combine((*pClusters)[i]->getName(), 
                (*pClusters)[i]->getResponse());
    }

    m_sResult.clear();
    m_pResultCombiner->endCombine(m_sResult);
}

FX_NS_END
