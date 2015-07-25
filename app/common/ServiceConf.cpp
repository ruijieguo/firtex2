#include "ServiceConf.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/config/XMLConfigurator.h"

using namespace std;

FX_NS_USE(utility);
FX_NS_USE(common);
FX_NS_USE(config);

FX_NS_DEF(app);

SETUP_LOGGER(app, ServiceConf);

ServiceConf::ServiceConf() 
{
    Master.port = 19801;

    Blender.port = 19802;
    Blender.thread_pool = 10;
    
    Proxy.port = 19803;
    Proxy.thread_pool = 10;

    Searcher.port = 19804;
    Searcher.thread_pool = 10;
}

ServiceConf::~ServiceConf() 
{
}

void ServiceConf::configure(Configurator& conf)
{
    if (conf.getMode() == Configurator::TO_CONF)
    {
        Configurator rootConf;

        Configurator masterConf;
        Master.configure(masterConf);
        rootConf.configure("master", masterConf.getMap());

        Configurator clustersConf;
        for (sClusters::iterator it = Clusters.begin();
             it != Clusters.end(); ++it)
        {
            Configurator clusterConf;
            (*it).configure(clusterConf);
            clustersConf.configure("clusters",  clusterConf.getMap());
        }
        rootConf.configure("clusters", clustersConf.getMap());
     

        Configurator pathConf;
        Path.configure(pathConf);
        rootConf.configure("path", pathConf.getMap());

        Configurator blenderConf;
        Blender.configure(blenderConf);
        rootConf.configure("blender", blenderConf.getMap());

        Configurator proxyConf;
        Proxy.configure(proxyConf);
        rootConf.configure("proxy", proxyConf.getMap());

        Configurator searcherConf;
        Searcher.configure(searcherConf);
        rootConf.configure("searcher", searcherConf.getMap());
    }
    else 
    {
        GLOBAL_CONF().configure(conf);
        
        Configurator::Iterator masterIt = conf.findConf("master");
        if (masterIt.hasNext())
        {
            Configurator masterConf(AnyCast<Configurator::ConfMap>(masterIt.next().second));
            Master.configure(masterConf);
        }
            
        Configurator::Iterator clustersIt = conf.findConf("clusters");
        if (clustersIt.hasNext())
        {
            Configurator clustersConf(AnyCast<Configurator::ConfMap>(
                            clustersIt.next().second));
                
            Configurator::Iterator clusterIt = clustersConf.iterator();
            while (clusterIt.hasNext())
            {
                Configurator::KeyValuePair kv = clusterIt.next();
                if (kv.first == "cluster")
                {
                    sCluster cluster;
                    Configurator clusterConf(AnyCast<Configurator::ConfMap>(kv.second));
                    cluster.configure(clusterConf);
                    Clusters.push_back(cluster);
                }//endif
            }//end while
        }//end if

        Configurator::Iterator pathIt = conf.findConf("path");
        if (pathIt.hasNext())
        {
            Configurator pathConf(AnyCast<Configurator::ConfMap>(pathIt.next().second));
            Path.configure(pathConf);
            updateDefaultPath();
        }

        Configurator::Iterator blenderIt = conf.findConf("blender");
        if (blenderIt.hasNext())
        {
            Configurator blenderConf(AnyCast<Configurator::ConfMap>(blenderIt.next().second));
            Blender.configure(blenderConf);
        }

        Configurator::Iterator proxyIt = conf.findConf("proxy");
        if (proxyIt.hasNext())
        {
            Configurator proxyConf(AnyCast<Configurator::ConfMap>(proxyIt.next().second));
            Proxy.configure(proxyConf);
        }

        Configurator::Iterator searcherIt = conf.findConf("searcher");
        if (searcherIt.hasNext())
        {
            Configurator searcherConf(AnyCast<Configurator::ConfMap>(searcherIt.next().second));
            Searcher.configure(searcherConf);
        }
    }
}

void ServiceConf::updateDefaultPath()
{
    if (Path.conf_dir.empty())
    {
        Path.conf_dir = Path.work_dir + "/conf";
    }
    if (Path.data_dir.empty())
    {
        Path.data_dir = Path.work_dir + "/data";
    }
    if (Path.dict_dir.empty())
    {
        Path.dict_dir = Path.work_dir + "/dict";
    }
    if (Path.plugin_dir.empty())
    {
        Path.plugin_dir = Path.work_dir + "/plugins";
    }
    GLOBAL_CONF().General.dictionaryPath = Path.dict_dir;
    GLOBAL_CONF().General.pluginPath = Path.plugin_dir;
}

void ServiceConf::makePathAbsolute(const std::string& sBasePath)
{
    makePathAbsolute(Path.work_dir, sBasePath);
    makePathAbsolute(Path.conf_dir, sBasePath);
    makePathAbsolute(Path.data_dir, sBasePath);
    makePathAbsolute(Path.dict_dir, sBasePath);
    makePathAbsolute(Path.plugin_dir, sBasePath);

    GLOBAL_CONF().makePathsAbsolute(sBasePath);
}

void ServiceConf::makePathAbsolute(string& sRelativePath, const string& sBasePath)
{
    if (sRelativePath.empty())
        return;

    FX_NS(utility)::Path path(sRelativePath);
    if (path.isRelative())
    {
        FX_NS(utility)::Path basePath(sBasePath);
        path.makeAbsolute(basePath);
        sRelativePath = path.toString();
    }
}

FX_NS_END

