#ifndef __FXLEXICONBUILDERAPPRUNNER_H
#define __FXLEXICONBUILDERAPPRUNNER_H

#include "firtex/common/StdHeader.h"
#include "firtex/extension/util/AppRunner.h"

FX_NS_DEF(util);
class Application;
FX_NS_END


FX_NS_DEF(app);

class LexiconBuilderAppRunner : public FX_NS(util)::AppRunner
{
public:
    LexiconBuilderAppRunner();
    Status run();
    void registerOption(FX_NS(util)::Application* pApp);

private:
    void optionCallback(const FX_NS(util)::Option& option, const tstring& sValue);
    void buildLexicon();

private:
    tstring m_sSourceFile;
    tstring m_sTargetFile;
};

FX_NS_END

#endif
