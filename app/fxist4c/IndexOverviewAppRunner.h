#ifndef __FXINDEXOVERVIEWERAPPRUNNER_H
#define __FXINDEXOVERVIEWERAPPRUNNER_H

#include "firtex/common/StdHeader.h"
#include "firtex/extension/util/AppRunner.h"

FX_NS_DEF(util);
class Application;
FX_NS_END

FX_NS_DEF(app);

class IndexOverviewerAppRunner : public FX_NS(util)::AppRunner
{
public:
    IndexOverviewerAppRunner();
    Status run();
    void registerOption(FX_NS(util)::Application* pApp);

private:
    void optionCallback(const FX_NS(util)::Option& option, const tstring& sValue);
    void overview();

private:
    tstring m_sIndexPath;
    tstring m_sLexiDir;
};

FX_NS_END

#endif
