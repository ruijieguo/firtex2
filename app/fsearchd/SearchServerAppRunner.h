#ifndef __SEARCH_SERVER_APP_RUNNER_H
#define __SEARCH_SERVER_APP_RUNNER_H

#include "firtex/extension/util/AppRunner.h"
#include "SearchServerBase.h"

FX_NS_DEF(util);
class Application;
FX_NS_END

FX_NS_DEF(app);

class SearchServerAppRunner : public FX_NS(util)::AppRunner
{
public:
    SearchServerAppRunner();
    ~SearchServerAppRunner();

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
    SearchServerBasePtr m_pServer;
    std::string m_sConfFile;
    std::string m_sServiceType;

private:
    DECLARE_LOGGER();
};


FX_NS_END

#endif
