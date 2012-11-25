#ifndef __HELPDISPLAYAPPRUNNER_H
#define __HELPDISPLAYAPPRUNNER_H

#include "firtex/extension/util/AppRunner.h"
#include "firtex/extension/util/Option.h"
#include "firtex/extension/util/OptionParser.h"

FX_NS_DEF(util);
class Application;

class HelpDisplayAppRunner : public AppRunner
{
public:
    HelpDisplayAppRunner();
    Status run();
    void registerOption(Application* pApp);
    void optionCallback(const FX_NS(util)::Option& option, const tstring& sValue);

private:
    FX_NS(util)::OptionParser* m_pOptionParser;    
    bool m_bShowHelp;
};

FX_NS_END

#endif
