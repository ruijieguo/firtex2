#ifndef __SEARCH_SERVER_CONF_H
#define __SEARCH_SERVER_CONF_H

#include "firtex/config/Configurable.h"
#include "firtex/config/Configurator.h"
#include "../common/ServerConfBase.h"

FX_NS_DEF(app);

struct SearchServerConf : public ServerConfBase
{
public:
    const static std::string DEFAULT_LOGGER_CONF;

public:
    void configure(FX_NS(config)::Configurator& conf);
    void makePathAbsolute(const std::string& sBasePath);
    /// Return the name of the server
    std::string getServerName() const { return "searchd";}

public:
    struct sResource : public FX_NS(config)::Configurable
    {
        /// index data path
        std::string index_data;

        /// default field of query
        std::string default_field;

        /// refresh timer in seconds, <=0 means no refresh
        int32_t refresh_timer;

        void configure(FX_NS(config)::Configurator& conf)
        {
            conf.configure("index_data", index_data, "");
            conf.configure("default_field", default_field, "BODY");
            conf.configure("refresh_timer", refresh_timer, 0);
        }
    };

public:
    sResource resourceConf;
};

DEFINE_TYPED_PTR(SearchServerConf);

FX_NS_END

#endif //__SEARCH_SERVER_CONF_H
