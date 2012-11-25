#ifndef __BLENDER_SERVER_CONF_H
#define __BLENDER_SERVER_CONF_H

#include "firtex/config/Configurable.h"
#include "firtex/config/Configurator.h"
#include "../common/ServerConfBase.h"

FX_NS_DEF(app);

struct BlenderServerConf : public ServerConfBase
{
public:
    void configure(FX_NS(config)::Configurator& conf);
    /// Return the name of the server
    std::string getServerName() const { return "blender";}

public:
    struct sCluster : public FX_NS(config)::Configurable
    {
        std::string name;    /// Identifier of this cluster
        std::string server;  /// IP address of this cluster

        void configure(FX_NS(config)::Configurator& conf)
        {
            conf.configure("name", name, "");
            conf.configure("server", server, "");
        }
    };

    typedef std::list<sCluster> ClusterList;
    struct sResource : public FX_NS(config)::Configurable
    {
        ClusterList clusters;

        void configure(FX_NS(config)::Configurator& conf);
    };

public:
    sResource resourceConf;
};

DEFINE_TYPED_PTR(BlenderServerConf);

FX_NS_END

#endif //__BLENDER_SERVER_CONF_H
