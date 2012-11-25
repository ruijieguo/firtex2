#include "SearchServerConf.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/Path.h"

FX_NS_USE(config);
FX_NS_USE(utility);
FX_NS_USE(common);

FX_NS_DEF(app);

const std::string SearchServerConf::DEFAULT_LOGGER_CONF = "logger_conf.xml";

void SearchServerConf::configure(FX_NS(config)::Configurator& conf)
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

void SearchServerConf::makePathAbsolute(const std::string& sBasePath)
{
    Path srcPath(resourceConf.index_data);
    if (!resourceConf.index_data.empty() && srcPath.isRelative())
    {
        Path basePath(sBasePath);
        srcPath.makeAbsolute(basePath);
        resourceConf.index_data = srcPath.toString();
    }
    
    ServerConfBase::makePathAbsolute(sBasePath);
}

FX_NS_END
