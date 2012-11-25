#ifndef __FXINDEXPRINTERAPPRUNNER_H
#define __FXINDEXPRINTERAPPRUNNER_H

#include "firtex/common/StdHeader.h"
#include "firtex/extension/util/AppRunner.h"

FX_NS_DEF(util);
class Application;
FX_NS_END

FX_NS_DEF(app);

class IndexPrinterAppRunner : public FX_NS(util)::AppRunner
{
public:
    IndexPrinterAppRunner();
    Status run();
    void registerOption(FX_NS(util)::Application* pApp);
private:
    void optionCallback(const FX_NS(util)::Option& option, const tstring& sValue);
    void printIndex();
private:
    tstring m_sIndexPath;
    tstring m_sTerm;
    bool m_bPrintPosition;
};

FX_NS_END

#endif
