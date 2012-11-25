#include "ClusterClause.h"

using namespace std;
FX_NS_USE(search);

FX_NS_DEF(app);

SETUP_LOGGER(app, ClusterClause);

const std::string ClusterClause::PREFIX = "cluster";

ClusterClause::ClusterClause(const string& sName,
                             const string& sClauseValue)
    : Clause(sName, sClauseValue)
{
    fromString(sClauseValue);
}

void ClusterClause::toString(stringstream& ss, const string& sEqSymbol) const
{
    ss << PREFIX << sEqSymbol;
    for (ClusterMap::const_iterator it = m_clusters.begin();
         it != m_clusters.end(); ++it)
    {
        if (it != m_clusters.begin())
        {
            ss << ",";
        }

        ss << it->first;
        if (it->second)
        {
            ss << ":";
            it->second->toString(ss);
        }
    }
}

void ClusterClause::fromString(const string& sClauseValue)
{
    size_t n = 0;
    while (n < sClauseValue.size())
    {
        string sKey, sValue, sSkipBlock;
        n = Clause::parseClauseValue(sKey, sValue, sSkipBlock, sClauseValue, n);
        Statement* pSubStat = NULL;
        if (!sSkipBlock.empty())
        {
            pSubStat = new Statement();
            if (!pSubStat->fromString(sSkipBlock))
            {
                FX_LOG(ERROR, "Parse sub query of cluster FAILED: [%s]", sSkipBlock.c_str());
                delete pSubStat;
                pSubStat = NULL;
            }
        }

        if (m_clusters.find(sKey) != m_clusters.end())
        {
            FX_LOG(ERROR, "Cluster is duplicated: [%s]", sKey.c_str());
        }
        m_clusters.insert(make_pair(sKey, pSubStat));
    }// end while
}

void ClusterClause::clear()
{
    for (ClusterMap::iterator it = m_clusters.begin();
         it != m_clusters.end(); ++it)
    {
        if (it->second)
        {
            delete it->second;
        }
    }

    m_clusters.clear();
}

FX_NS_END
