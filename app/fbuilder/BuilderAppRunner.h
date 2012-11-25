#ifndef __FXBUILDERAPPRUNNER_H
#define __FXBUILDERAPPRUNNER_H

#include "firtex/common/StdHeader.h"
#include "firtex/extension/util/AppRunner.h"
#include "firtex/extension/collection/Collection.h"

FX_NS_DEF(util);
class Application;
class ServerApplication;
FX_NS_END

FX_NS_DEF(app);

class BuilderAppRunner : public FX_NS(util)::AppRunner
{
public:
    BuilderAppRunner();
    Status run();
    void registerOption(FX_NS(util)::Application* pApp);

    void stop();
    void waitStop();

private:
    void optionCallback(const FX_NS(util)::Option& option, const std::string& sValue);
    void buildIndex(bool bBlock);

private:
    std::string m_sConfFile;
    FX_NS(collection)::CollectionPtr m_pColletion;
    FX_NS(util)::ServerApplication* m_pServerApp;
    bool m_bMonitor;
};

FX_NS_END

#endif
