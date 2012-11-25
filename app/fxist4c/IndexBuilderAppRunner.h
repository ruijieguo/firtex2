#ifndef __FXINDEXBUILDERAPPRUNNER_H
#define __FXINDEXBUILDERAPPRUNNER_H

#include "firtex/common/StdHeader.h"
#include "firtex/extension/util/AppRunner.h"

FX_NS_DEF(util);
class Application;
FX_NS_END

FX_NS_DEF(app);

class IndexBuilderAppRunner : public FX_NS(util)::AppRunner
{
public:
    IndexBuilderAppRunner();
    Status run();
    void registerOption(FX_NS(util)::Application* pApp);

private:
    void optionCallback(const FX_NS(util)::Option& option, const tstring& sValue);
    void buildIndex();
private:
    bool m_bBuild;
    tstring m_sConfFile;
};

FX_NS_END

#endif
