#ifndef __SERVER_CONF_BASE_H
#define __SERVER_CONF_BASE_H

#include "firtex/config/Configurable.h"
#include "firtex/config/Configurator.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/utility/Path.h"

FX_NS_DEF(app);

struct ServerConfBase : public FX_NS(config)::Configurable
{
public:
    void configure(FX_NS(config)::Configurator& conf)
    {
        if (conf.getMode() == FX_NS(config)::Configurator::TO_CONF)
        {
            FX_NS(config)::Configurator rootConf;
            FX_NS(config)::Configurator srvConfigurator;
            serverConf.configure(srvConfigurator);
            rootConf.configure("server", srvConfigurator.getMap());

            conf.configure(getServerName(), rootConf.getMap());
        }
        else 
        {
            GLOBAL_CONF().configure(conf);

            FX_NS(config)::Configurator::Iterator it0 = conf.findConf(getServerName());
            if (it0.hasNext())
            {
                FX_NS(config)::Configurator::ConfMap confMap =
                    FX_NS(common)::AnyCast<FX_NS(config)::Configurator::ConfMap>(it0.next().second);
                FX_NS(config)::Configurator c(confMap);

                FX_NS(config)::Configurator::Iterator it1 = c.findConf("server");
                if (it1.hasNext())
                {
                    FX_NS(config)::Configurator::ConfMap srvConfMap 
                        = FX_NS(common)::AnyCast<FX_NS(config)::Configurator::ConfMap>(it1.next().second);
                    FX_NS(config)::Configurator srvConf(srvConfMap);            
                    serverConf.configure(srvConf);
                }
            }
        }
    }

    virtual void makePathAbsolute(const std::string& sBasePath)
    {
        FX_NS(utility)::Path logPath(serverConf.logger_conf);
        if (!serverConf.logger_conf.empty() && logPath.isRelative())
        {
            FX_NS(utility)::Path basePath(sBasePath);
            logPath.makeAbsolute(basePath);
            serverConf.logger_conf = logPath.toString();
        }

        GLOBAL_CONF().makePathsAbsolute(sBasePath);
    }

    
    /// Return the name of the server
    virtual std::string getServerName() const = 0;

public:
    struct sServer : public FX_NS(config)::Configurable
    {
        std::string host;
        std::string logger_conf;
        int32_t listen_port;
        int32_t thread_pool;
        void configure(FX_NS(config)::Configurator& conf)
        {
            conf.configure("logger_conf", logger_conf, "");
            conf.configure("thread_pool", thread_pool, 8);

            if (conf.getMode() == FX_NS(config)::Configurator::TO_CONF)
            {
                if (host != "AUTO")
                {
                    std::string str = host + ":";
                    FX_NS(utility)::NumberFormatter::append(str, listen_port);
                    conf.configure("host", str);
                }
                else
                {
                    conf.configure("host", host);
                }
            }
            else
            {
                std::string str;
                conf.configure("host", str, "");

                FX_NS(utility)::StringTokenizer st(str, ":", FX_NS(utility)::StringTokenizer::TOKEN_TRIM
                        | FX_NS(utility)::StringTokenizer::TOKEN_IGNORE_EMPTY);
                if (st.getNumTokens() == 2)
                {
                    //format: ip:port or host:port
                    host = st[0];
                    if (!FX_NS(utility)::NumberParser::tryParseInt32(st[1], listen_port))
                    {
                        FIRTEX_THROW(InvalidConfigException, "Invalid host name format");
                    }
                }
                else if (st.getNumTokens() == 1 && st[0] != "AUTO")
                {
                    //format: port
                    host = "localhost";
                    if (!FX_NS(utility)::NumberParser::tryParseInt32(st[0], listen_port))
                    {
                        FIRTEX_THROW(InvalidConfigException, "Invalid host name format");
                    }
                }
                else 
                {
                    // empty or "AUTO"
                    conf.configure("host", host, "");
                    listen_port = -1;
                }
            }
        }
    };

public:
    sServer serverConf;
};

DEFINE_TYPED_PTR(ServerConfBase);

FX_NS_END

#endif //__SERVER_CONF_BASE_H
