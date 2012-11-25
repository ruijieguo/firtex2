#ifndef __FX_CLUSTERCLAUSE_H
#define __FX_CLUSTERCLAUSE_H

#include "firtex/common/StdHeader.h"
#include "firtex/search/Clause.h"
#include "firtex/search/Statement.h"
#include <map>

FX_NS_DEF(app);

class ClusterClause : public FX_NS(search)::Clause
{
public:
    DECLARE_CLAUSE_CREATOR(ClusterClause);

    typedef std::map<std::string, FX_NS(search)::Statement*> ClusterMap;
    typedef ClusterMap::const_iterator ClausePair;

public:
    const static std::string PREFIX;

public:
    class Iterator
    {
    public:
        Iterator(const ClusterMap& clusters)
            : m_clusters(clusters)
            , m_current(m_clusters.begin())
        {
        }

        bool hasNext() const
        {
            return m_current != m_clusters.end();
        }
        
        ClausePair next()
        {
            return m_current++;
        }

        size_t size() const
        {
            return m_clusters.size();
        }

    private:
        const ClusterMap& m_clusters;
        ClausePair m_current;
    };

public:    
    ClusterClause(const std::string& sName,
                  const std::string& sClauseValue);
    ~ClusterClause() 
    {
        clear();
    }
    
public:
    /// Format parameter to string
    virtual void toString(std::stringstream& ss, const std::string& sEqSymbol) const;

    /// element iteration
    Iterator iterator() const;

    /// Clear content
    void clear();

private:
    /// format: cluster=cluster1:{!query=...},cluster2:{!...},...
    void fromString(const std::string& sClauseValue);

private:
    ClusterMap m_clusters;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(ClusterClause);

///////////////////////////////////////
inline ClusterClause::Iterator ClusterClause::iterator() const
{
    return Iterator(m_clusters);
}

FX_NS_END

#endif //__FX_CLUSTERCLAUSE_H
