#include "SearchResource.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"
#include "firtex/utility/NumberParser.h"
#include <algorithm>

FX_NS_USE(index);
FX_NS_USE(utility);
FX_NS_USE(thread);

FX_NS_DEF(app);

SETUP_LOGGER(app, SearchResource);

SearchResource::SearchResource()
{
}

SearchResource::~SearchResource()
{
}

void SearchResource::init(const ServiceConf& conf)
{
    m_conf = conf;
    loadClusters(conf.Path.data_dir);
}

void SearchResource::refresh()
{
    reloadClusters(m_conf.Path.data_dir);
}

IndexReaderPtr SearchResource::getIndexReader(const std::string& sCluster,
        const std::string& sType) const
{
    ClusterMap::const_iterator it = m_clusters.find(sCluster);
    if (it != m_clusters.end())
    {
        TypeMap::const_iterator it2 = it->second->m_types.find(sType);
        if (it2 != it->second->m_types.end())
        {
            return it2->second->m_pIndexReader;
        }
    }
    return IndexReaderPtr();
}

void SearchResource::loadClusters(const string& sDataRoot)
{
    FX_LOG(INFO, "Load clusters in dir: [%s]", sDataRoot.c_str());
    File file(sDataRoot);
    vector<string> dirs;
    file.list(dirs, false);
    
    for (vector<string>::const_iterator it = dirs.begin();
         it != dirs.end(); ++it)
    {
        SearchResource::ClusterPtr pCluster = loadCluster(*it, sDataRoot);
        if (pCluster != NULL)
        {
            FX_LOG(INFO, "Load cluster: [%s] in directory: [%s] SUCCESS.", 
                   (*it).c_str(), sDataRoot.c_str());
            m_clusters.insert(make_pair(*it, pCluster));
        }
        else
        {
            FX_LOG(INFO, "Load cluster: [%s] in directory: [%s] FAILED.", 
                   (*it).c_str(), sDataRoot.c_str());
        }
    }
}

SearchResource::ClusterPtr SearchResource::loadCluster(
        const std::string& sClusterName,
        const std::string& sDataRoot)
{
    FX_LOG(INFO, "Load cluster dir: [%s], cluster: [%s]", 
           sDataRoot.c_str(), sClusterName.c_str());

    Path path(sDataRoot);
    path.makeDirectory();
    path.pushDirectory(sClusterName);
    File file(path);
    if (!file.isDirectory() || !file.exists())
    {
        FX_LOG(ERROR, "[%s] is not a directory or not exists",
               path.toString().c_str());
        return NULL;
    }

    vector<string> dirs;
    file.list(dirs, false);
    if (dirs.empty())
    {
        return NULL;
    }

    SearchResource::ClusterPtr pCluster = new Cluster(sClusterName);
    for (vector<string>::const_iterator it = dirs.begin();
         it != dirs.end(); ++it)
    {
        SearchResource::TypePtr pType = loadType(*it, path.toString());
        if (pType.isNotNull())
        {
            pCluster->m_types.insert(make_pair(*it, pType));
        }
    }

    return pCluster;
}

bool dirCompare(int32_t v1, int32_t v2)
{
    return v1 < v2;
}

int32_t SearchResource::getLastGeneration(const vector<string>& dirs)
{
    vector<int32_t> sortedDirs;
    for (vector<string>::const_iterator it = dirs.begin();
         it != dirs.end(); ++it)
    {
        int32_t v = 0;
        if (NumberParser::tryParseInt32(*it, v))
        {
            sortedDirs.push_back(v);
        }
    }
    
    std::sort(sortedDirs.begin(), sortedDirs.end(), dirCompare);
    if (sortedDirs.size() > 0)
    {
        return *(sortedDirs.rbegin());
    }
    return -1;
}
    
SearchResource::TypePtr SearchResource::loadType(const string& sTypeName,
        const string& sDataRoot)
{
    vector<string> dirs;
    Path path(sDataRoot);
    path.makeDirectory();
    path.pushDirectory(sTypeName);
    File file(path);
    file.list(dirs, false);
    int32_t nLastGen = getLastGeneration(dirs);
    if (nLastGen == -1)
    {
        FX_LOG(WARN, "Load type: [%s], [%s] FAILED: no generation found", 
               sDataRoot.c_str(), sTypeName.c_str());

        return NULL;
    }

    string sIndexPath = path.toString();
    try
    {
        TypePtr pType = new Type();
        FX_LOG(INFO, "Begin load type: [%s], [%s], [%d]",
               sDataRoot.c_str(), sTypeName.c_str(), nLastGen);
        pType->init(sTypeName, nLastGen, sDataRoot);
        FX_LOG(INFO, "End load type");
        return pType;
    }
    catch (const FirteXException& e)
    {
        FX_LOG(WARN, "Load type: [%s], [%s], [%d] FAILED",
               sDataRoot.c_str(), sTypeName.c_str(), nLastGen);
        return NULL;
    }
    
    return NULL;
}

SearchResource::TypePtr SearchResource::reloadType(const TypePtr& pType, 
                                                   const string& sDataRoot)
{
    vector<string> dirs;
    Path path(sDataRoot);
    path.makeDirectory();
    path.pushDirectory(pType->m_name);
    File file(path);
    file.list(dirs, false);
    int32_t nLastGen = getLastGeneration(dirs);
    if (nLastGen == -1)
    {
        FX_LOG(WARN, "Re-load type: [%s], [%s] FAILED: no generation found", 
               sDataRoot.c_str(), pType->m_name.c_str());
        return NULL;
    }

    try
    {
        if (nLastGen == pType->m_nGen)
        {
            TypePtr pNewType = pType;
            pNewType->reopenIndex();
            return pType;
        }
        else
        {
            TypePtr pNewType = new Type();
            FX_LOG(INFO, "Begin load type: [%s], [%s], [%d]",
                   sDataRoot.c_str(), pType->m_name.c_str(), nLastGen);
            pNewType->init(pType->m_name, nLastGen, sDataRoot);
            FX_LOG(INFO, "End load type");
            return pNewType;
        }
    }
    catch (const FirteXException& e)
    {
        FX_LOG(WARN, "Begin load type: [%s], [%s], [%d]",
               sDataRoot.c_str(), pType->m_name.c_str(), nLastGen);
        return NULL;
    }
    
    return NULL;
}


void SearchResource::reloadClusters(const std::string& sDataRoot)
{
    FX_LOG(INFO, "Re-load clusters in directory: [%s]", sDataRoot.c_str());
    ClusterMap oldClusters;
    {
        ScopedRWLock g(m_rwlock, false);
        oldClusters = m_clusters;
    }
    ClusterMap newClusters;
    
    File file(sDataRoot);
    vector<string> dirs;
    file.list(dirs, false);

    for (vector<string>::const_iterator it = dirs.begin();
         it != dirs.end(); ++it)
    {
        ClusterMap::const_iterator it2 = oldClusters.find(*it);
        if (it2 != oldClusters.end())
        {
            ClusterPtr pCluster = it2->second;
            pCluster = reloadCluster(pCluster, sDataRoot);
            if (pCluster.isNotNull())
            {
                newClusters.insert(make_pair(*it, pCluster));
            }
        }
        else 
        {
            SearchResource::ClusterPtr pCluster = loadCluster(*it, sDataRoot);
            if (pCluster != NULL)
            {
                FX_LOG(INFO, "Load cluster: [%s] in directory: [%s] SUCCESS.", 
                       (*it).c_str(), sDataRoot.c_str());
                newClusters.insert(make_pair(*it, pCluster));
            }
            else
            {
                FX_LOG(INFO, "Load cluster: [%s] in directory: [%s] FAILED.", 
                       (*it).c_str(), sDataRoot.c_str());
            }            
        }
    }

    ScopedRWLock g(m_rwlock, true);
    m_clusters.swap(newClusters);
}

SearchResource::ClusterPtr SearchResource::reloadCluster(
        ClusterPtr pCluster, const string& sDataRoot)
{
    FX_LOG(INFO, "Re-load cluster [%s] in directory: [%s]",
           pCluster->m_name.c_str(), sDataRoot.c_str());

    TypeMap oldTypes;
    {
        ScopedRWLock g(m_rwlock, false);
        oldTypes = pCluster->m_types;
    }

    SearchResource::ClusterPtr pNewCluster = new Cluster(pCluster->m_name);

    Path path(sDataRoot);
    path.makeDirectory();
    path.pushDirectory(pCluster->m_name);
    File file(path);
    vector<string> dirs;
    file.list(dirs, false);
    if (dirs.empty())
    {
        return NULL;
    }

    string sClusterPath = path.toString();
    for (vector<string>::const_iterator it = dirs.begin();
         it != dirs.end(); ++it)
    {
        TypeMap::const_iterator it2 = oldTypes.find(*it);
        if (it2 != oldTypes.end())
        {
            SearchResource::TypePtr pType = it2->second;
            pType = reloadType(pType, sClusterPath);
            if (pType != NULL)
            {
                pNewCluster->m_types.insert(make_pair(*it, pType));
            }
        }
        else 
        {
            SearchResource::TypePtr pType = loadType(*it, sClusterPath);
            if (pType != NULL)
            {
                pNewCluster->m_types.insert(make_pair(*it, pType));
            }
        }
    }
    return pNewCluster;
}

FX_NS_END
