#ifndef __PROXY_SERVER_CONF_H
#define __PROXY_SERVER_CONF_H

#include "firtex/config/Configurable.h"
#include "../common/ServerConfBase.h"

FX_NS_DEF(app);

struct ProxyServerConf : public ServerConfBase
{
public:
    void configure(FX_NS(config)::Configurator& conf);
    /// Return the name of the server
    std::string getServerName() const { return "proxy";}

public:
    struct sReplica : public FX_NS(config)::Configurable
    {
        std::string server;  /// format: ip:port
        int32_t weight;      /// weight of this server, used for load balance

        void configure(FX_NS(config)::Configurator& conf)
        {
            conf.configure("server", server, "");
            conf.configure("weight", weight, 0);
        }
    };

    typedef std::list<sReplica> ReplicaList;

    struct sShard : public FX_NS(config)::Configurable
    {
        std::string name;      /// Identifier of this shard
        ReplicaList replicas;  /// replicas of this shard
        
        void configure(FX_NS(config)::Configurator& conf);
    };
    
    typedef std::list<sShard> ShardList;

    struct sResource : public FX_NS(config)::Configurable
    {
        std::string load_ballance;
        ShardList shards;

        void configure(FX_NS(config)::Configurator& conf);
    };

public:
    sResource resourceConf;
};

DEFINE_TYPED_PTR(ProxyServerConf);

FX_NS_END

#endif //__PROXY_SERVER_CONF_H
