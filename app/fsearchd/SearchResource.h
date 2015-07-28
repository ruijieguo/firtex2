#ifndef __FX_SEARCH_RESOURCE_H
#define __FX_SEARCH_RESOURCE_H

#include "firtex/index/Index.h"
#include "../common/ServiceConf.h"
#include "firtex/thread/RWLock.h"
#include <string>
#include <map>
#include <vector>

FX_NS_DEF(app);

class SearchResource
{
public:
    SearchResource();
    ~SearchResource();

protected:
    struct Type
    {
        std::string m_name;
        int32_t m_nGen;
        FX_NS(index)::IndexPtr m_pIndex;
        FX_NS(index)::IndexReaderPtr m_pIndexReader;
        
        void init(const std::string& sTypeName, int32_t nGeneration, 
                  const std::string& sDataRoot)
        {
            FX_NS(utility)::Path path(sDataRoot);
            path.makeDirectory();
            path.pushDirectory(sTypeName);
            std::string sGen;
            FX_NS(utility)::NumberFormatter::append(sGen, nGeneration);
            path.pushDirectory(sGen);

            m_name = sTypeName;
            m_nGen = nGeneration;
            m_pIndex.reset(new FX_NS(index)::Index());
            m_pIndex->open(path.toString(), FX_NS(index)::Index::READ, NULL);
            m_pIndexReader = m_pIndex->acquireReader();
        }

        void reopenIndex()
        {
            m_pIndexReader = m_pIndex->acquireReader(true);
        }
    };
    DEFINE_TYPED_PTR(Type);


    typedef std::map<std::string, TypePtr> TypeMap;

    struct Cluster
    {
        Cluster(const std::string& n) : m_name(n) {}

        std::string m_name;
        TypeMap m_types;
        
        void reopenIndex()
        {
            for (TypeMap::iterator it = m_types.begin(); it != m_types.end(); ++it)
            {
                it->second->reopenIndex();
            }
        }

        void reopenIndex(const std::string& sType)
        {
            TypeMap::iterator it = m_types.find(sType);
            if (it != m_types.end())
            {
                it->second->reopenIndex();
            }
        }
    };
    DEFINE_TYPED_PTR(Cluster);

    typedef std::map<std::string, ClusterPtr> ClusterMap;

public:
    /**
     * Scan data directory and load all clusters
     */
    void init(const ServiceConf& conf);

    /**
     * Refresh resource
     */
    void refresh();

    /**
     * Get index reader by cluster/type name
     * @param sCluster cluster name
     * @param sType type name
     * @return NULL if no specific cluster or type 
     */
    FX_NS(index)::IndexReaderPtr getIndexReader(const std::string& sCluster,
            const std::string& sType) const;

protected:
    void loadClusters(const std::string& sDataRoot);
    ClusterPtr loadCluster(const std::string& sClusterName, const std::string& sDataRoot);
    TypePtr loadType(const string& sTypeName, const string& sDataRoot);

    void reloadClusters(const std::string& sDataRoot);
    ClusterPtr reloadCluster(ClusterPtr pCluster, const std::string& sDataRoot);
    TypePtr reloadType(const TypePtr& pType, const std::string& sDataRoot);

    int32_t getLastGeneration(const std::vector<std::string>& dirs);

private:
    FX_NS(thread)::RWLock m_rwlock;

    ServiceConf m_conf;
    ClusterMap m_clusters;    

private:
    DECLARE_LOGGER();
};

/////////////////////////////////////
//


FX_NS_END

#endif//__FX_SEARCH_RESOURCE_H
