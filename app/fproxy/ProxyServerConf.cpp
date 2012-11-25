#include "ProxyServerConf.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/Path.h"

FX_NS_USE(config);
FX_NS_USE(utility);
FX_NS_USE(common);

FX_NS_DEF(app);

void ProxyServerConf::sShard::configure(FX_NS(config)::Configurator& conf)
{
    conf.configure("name", name, "");
    if (conf.getMode() == FX_NS(config)::Configurator::TO_CONF)
    {
        Configurator repsConf;
        for (std::list<sReplica>::iterator it = replicas.begin();
             it != replicas.end(); ++it)
        {
            Configurator repConf;
            (*it).configure(repConf);
            repsConf.configure("replica", repConf.getMap());
        }
        conf.configure("replicas", repsConf.getMap());
    }
    else
    {
        Configurator::Iterator repsIt = conf.findConf("replicas");
        if (!repsIt.hasNext())
        {
            FIRTEX_THROW(BadParameterException, "Missing configuration item: <replicas>");
        }
        Configurator::KeyValuePair kv = repsIt.next();
        Configurator::ConfMap repsConfMap = FX_NS(common)::AnyCast<Configurator::ConfMap>(kv.second);
        Configurator repsConf(repsConfMap);
                
        Configurator::Iterator it2 = repsConf.iterator();
        while (it2.hasNext())
        {
            Configurator::KeyValuePair kv2 = it2.next();
            if (!FX_NS(utility)::strCompareNoCase(kv2.first.c_str(), "replica"))
            {
                Configurator::ConfMap repConfMap 
                    = FX_NS(common)::AnyCast<Configurator::ConfMap>(kv2.second);
                Configurator repConf(repConfMap, Configurator::FROM_CONF);
                sReplica rep;
                rep.configure(repConf);
                replicas.push_back(rep);
            }
        }
    }
}

void ProxyServerConf::sResource::configure(FX_NS(config)::Configurator& conf)
{
    conf.configure("load_ballance", load_ballance);
    if (conf.getMode() == FX_NS(config)::Configurator::TO_CONF)
    {
        Configurator shardsConf;
        for (std::list<sShard>::iterator it = shards.begin();
             it != shards.end(); ++it)
        {
            Configurator shardConf;
            (*it).configure(shardConf);
            shardsConf.configure("shard", shardConf.getMap());
        }
        conf.configure("shards", shardsConf.getMap());
    }
    else
    {
        Configurator::Iterator shardsIt = conf.findConf("shards");
        if (!shardsIt.hasNext())
        {
            FIRTEX_THROW(BadParameterException, "Missing configuration item: <shards>");
        }
        Configurator::KeyValuePair kv = shardsIt.next();
        Configurator::ConfMap shardsConfMap = AnyCast<Configurator::ConfMap>(kv.second);
        Configurator shardsConf(shardsConfMap);
                
        Configurator::Iterator it2 = shardsConf.iterator();
        while (it2.hasNext())
        {
            Configurator::KeyValuePair kv2 = it2.next();
            if (!strCompareNoCase(kv2.first.c_str(), "shard"))
            {
                Configurator::ConfMap shardConfMap 
                    = AnyCast<Configurator::ConfMap>(kv2.second);
                Configurator shardConf(shardConfMap, Configurator::FROM_CONF);
                sShard shard;
                shard.configure(shardConf);
                shards.push_back(shard);
            }
        }
    }
}

void ProxyServerConf::configure(FX_NS(config)::Configurator& conf)
{
    if (conf.getMode() == Configurator::TO_CONF)
    {
        Configurator rootConf;
        Configurator srvConfigurator;
        serverConf.configure(srvConfigurator);
        rootConf.configure("server", srvConfigurator.getMap());

        Configurator resourceConfigurator;
        resourceConf.configure(resourceConfigurator);
        rootConf.configure("resource", resourceConfigurator.getMap());

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
                Configurator datConf(resourceConfMap);            
                resourceConf.configure(datConf);
            }
        }
    }
}

FX_NS_END
