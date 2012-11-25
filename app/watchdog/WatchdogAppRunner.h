#ifndef __DAEMON_APP_RUNNER_H
#define __DAEMON_APP_RUNNER_H

#include "firtex/common/Logger.h"
#include "firtex/extension/util/AppRunner.h"
#include "../common/ServerBase.h"

FX_NS_DEF(util);
class Application;
FX_NS_END

FX_NS_DEF(app);

class WatchdogAppRunner : public FX_NS(util)::AppRunner
{
public:
    WatchdogAppRunner();
    ~WatchdogAppRunner();

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
    std::string m_sServiceType;
    std::string m_sWorkDir;
    int32_t m_nListenPort;

private:
    DECLARE_LOGGER();
};


FX_NS_END

#endif
