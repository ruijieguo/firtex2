#ifndef __PROXY_SERVER_APP_RUNNER_H
#define __PROXY_SERVER_APP_RUNNER_H

#include "firtex/common/Logger.h"
#include "firtex/extension/util/AppRunner.h"
#include "ProxyServerConf.h"
#include "../common/ServerBase.h"

FX_NS_DEF(util);
class Application;
FX_NS_END

FX_NS_DEF(app);

class ProxyServerAppRunner : public FX_NS(util)::AppRunner
{
public:
    ProxyServerAppRunner();
    ~ProxyServerAppRunner();

public:
    Status run();
    void registerOption(FX_NS(util)::Application* pApp);
    void optionCallback(const FX_NS(util)::Option& option, 
                        const std::string& sValue);

    void stop();
    void waitStop();

private:
    bool initHttpServer();
    bool initRPCServer();

private:
    ServerBasePtr m_pServer;
    std::string m_sConfFile;
    std::string m_sRole;
    std::string m_sServiceType;

private:
    DECLARE_LOGGER();
};


FX_NS_END

#endif
