#ifndef __FXINDEXMERGERAPPRUNNER_H
#define __FXINDEXMERGERAPPRUNNER_H

#include "firtex/common/StdHeader.h"
#include "firtex/extension/util/AppRunner.h"

FX_NS_DEF(util);
class Application;
FX_NS_END

FX_NS_DEF(app);

class IndexMergerAppRunner : public FX_NS(util)::AppRunner
{
public:
    IndexMergerAppRunner();
    Status run();
    void registerOption(FX_NS(util)::Application* pApp);
private:
    void optionCallback(const FX_NS(util)::Option& option, const tstring& sValue);
    void indexMerge();
private:
    int32_t m_nMergeMode;//1 for merge, 2 for optimize
    tstring m_sIndexPath;
    tstring m_sLexiDir;
};

FX_NS_END

#endif
