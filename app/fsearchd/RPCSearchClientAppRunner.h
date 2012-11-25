#ifndef __FX_RPCSEARCHCILENT_APP_RUNNER_H
#define __FX_RPCSEARCHCILENT_APP_RUNNER_H

#include "firtex/common/Logger.h"
#include "firtex/extension/util/AppRunner.h"

FX_NS_DEF(util);
class Application;
FX_NS_END

FX_NS_DEF(app);

class RPCSearchClientAppRunner : public FX_NS(util)::AppRunner
{
public:
    RPCSearchClientAppRunner();
    ~RPCSearchClientAppRunner();

public:
    Status run();
    void registerOption(FX_NS(util)::Application* pApp);
    void optionCallback(const FX_NS(util)::Option& option, const tstring& sValue);

    void stop();
    void waitStop();

private:
    void startShell();
    void doSearch();
    void displayShellHelp();

private:
    std::string m_sHost;
    std::string m_sQueryString;
    std::string m_sQueryFile;
    int m_nPort;
    bool m_bStartShell;

private:
    DECLARE_LOGGER();
};


FX_NS_END

#endif
