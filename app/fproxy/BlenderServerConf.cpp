#include "BlenderServerConf.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/Path.h"

FX_NS_USE(config);
FX_NS_USE(utility);
FX_NS_USE(common);

FX_NS_DEF(app);

void BlenderServerConf::sResource::configure(FX_NS(config)::Configurator& conf)
{
    if (conf.getMode() == FX_NS(config)::Configurator::TO_CONF)
    {
        Configurator clustersConf;
        for (std::list<sCluster>::iterator it = clusters.begin();
             it != clusters.end(); ++it)
        {
            Configurator clusterConf;
            (*it).configure(clusterConf);
            clustersConf.configure("cluster", clusterConf.getMap());
        }
        conf.configure("clusters", clustersConf.getMap());
    }
    else
    {
        Configurator::Iterator clustersIt = conf.findConf("clusters");
        if (!clustersIt.hasNext())
        {
            FIRTEX_THROW(BadParameterException, "Missing configuration item: <clusters>");
        }
        Configurator::KeyValuePair kv = clustersIt.next();
        Configurator::ConfMap clustersConfMap = AnyCast<Configurator::ConfMap>(kv.second);
        Configurator clustersConf(clustersConfMap);
                
        Configurator::Iterator it2 = clustersConf.iterator();
        while (it2.hasNext())
        {
            Configurator::KeyValuePair kv2 = it2.next();
            if (!strCompareNoCase(kv2.first.c_str(), "cluster"))
            {
                Configurator::ConfMap clusterConfMap 
                    = AnyCast<Configurator::ConfMap>(kv2.second);
                Configurator clusterConf(clusterConfMap, Configurator::FROM_CONF);
                sCluster cluster;
                cluster.configure(clusterConf);
                clusters.push_back(cluster);
            }
        }
    }
}

void BlenderServerConf::configure(FX_NS(config)::Configurator& conf)
{
    if (conf.getMode() == Configurator::TO_CONF)
    {
        Configurator rootConf;
        Configurator srvConfigurator;
        serverConf.configure(srvConfigurator);
        rootConf.configure("server", srvConfigurator.getMap());

        Configurator resConfigurator;
        resourceConf.configure(resConfigurator);
        rootConf.configure("resource", resConfigurator.getMap());

        conf.configure(getServerName(), rootConf.getMap());
    }
    else 
    {
        ServerConfBase::configure(conf);

        Configurator::Iterator it0 = conf.findConf(getServerName());
        if (it0.hasNext())
        {
            Configurator::ConfMap confMap =
                AnyCast<Configurator::ConfMap>(it0.next().second);
            Configurator c(confMap);
            
            Configurator::Iterator it2 = c.findConf("resource");
            if (it2.hasNext())
            {
                Configurator::ConfMap resourceConfMap 
                    = AnyCast<Configurator::ConfMap>(it2.next().second);
                Configurator resConf(resourceConfMap);            
                resourceConf.configure(resConf);
            }
        }
    }
}

FX_NS_END
