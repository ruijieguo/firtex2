#ifndef __FX_SERVICECONF_H
#define __FX_SERVICECONF_H

#include "firtex/config/Configurable.h"
#include "firtex/config/Configurator.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/utility/StringTokenizer.h"
#include <vector>

FX_NS_DEF(app);

struct ServiceConf : public FX_NS(config)::Configurable
{
public:
    ServiceConf();
    ~ServiceConf();

public:
    struct sMaster : public FX_NS(config)::Configurable
    {
        std::string zk_root;
        std::string mesos_master;
        std::string host;
        int32_t port;
    public:
        void configure(FX_NS(config)::Configurator& conf)
        {            
            conf.configure("zk_root", zk_root, "");
            conf.configure("mesos_master", mesos_master, "");
            conf.configure("host", host, "");
            conf.configure("port", port, -1);
        }
    };
    sMaster Master;

    struct sCluster : public FX_NS(config)::Configurable
    {
        std::string name;
        std::string remote_data_root;
        int32_t number_of_shards;
        int32_t number_of_replicas;

        int32_t number_of_proxy;
        int32_t number_of_blender;

    public:
        void configure(FX_NS(config)::Configurator& conf)
        {
            conf.configure("name", name, ""); 
            conf.configure("remote_data_root", remote_data_root, "");

            conf.configure("number_of_shards", number_of_shards, 
                           number_of_shards);
            conf.configure("number_of_replicas", number_of_shards,
                           number_of_replicas);

            conf.configure("number_of_proxy", number_of_proxy, 
                           number_of_proxy);
            conf.configure("number_of_blender", number_of_blender, 
                           number_of_blender);
        }
    };

    typedef std::vector<sCluster> sClusters;
    sClusters Clusters;

    struct sPath : public FX_NS(config)::Configurable
    {
        std::string work_dir;  // Path to working directory
        std::string conf_dir;  // Path to directory containing configuration
        std::string data_dir;  // Path to directory where to store index data
        std::string dict_dir;  // Path to directory where to store dictionaries
        std::string plugin_dir;  // Path to directory where to store plugins

    public:
        void configure(FX_NS(config)::Configurator& conf)
        {
            conf.configure("work_dir", work_dir, "");
            conf.configure("conf_dir", conf_dir, work_dir.empty()? "" : work_dir + "/conf");
            conf.configure("data_dir", data_dir, work_dir.empty()? "" : work_dir + "/data");
            conf.configure("dict_dir", dict_dir, work_dir.empty()? "" : work_dir + "/dict");
            conf.configure("plugin_dir", plugin_dir, work_dir.empty()? "" : work_dir + "/plugins");
        }
    };
    sPath Path;

    struct sServer : public FX_NS(config)::Configurable
    {
        int32_t port;
        int32_t thread_pool;

    public:
        void configure(FX_NS(config)::Configurator& conf)
        {
            conf.configure("port", port, port);

            conf.configure("thread_pool", thread_pool, thread_pool);
        }
    };

    sServer Blender;
    
    struct sProxy : public sServer
    {
        std::string load_ballance;

    public:
        void configure(FX_NS(config)::Configurator& conf)
        {
            sServer::configure(conf);
            conf.configure("load_ballance", load_ballance, "");
        }
    };
    sProxy Proxy;    

    sServer Searcher;

public:
    void configure(FX_NS(config)::Configurator& conf);
    void makePathAbsolute(const std::string& sBasePath);

private:
    void makePathAbsolute(std::string& sRelativePath,
                          const std::string& sBasePath);
    void updateDefaultPath();

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(ServiceConf);

FX_NS_END

#endif //__FX_SERVERCONFBASE_H
