#include "firtex/extension/util/HelpDisplayAppRunner.h"
#include "firtex/extension/util/Application.h"
#include "firtex/extension/util/SharedOptionCallback.h"

using namespace std;

FX_NS_DEF(util);

HelpDisplayAppRunner::HelpDisplayAppRunner()
{
    m_bShowHelp = false;
}

AppRunner::Status HelpDisplayAppRunner::run()
{
    if (m_bShowHelp && m_pOptionParser)
    {
        m_pOptionParser->displayHelp();
        return S_EXIT;
    }
    return S_NONE;
}

void HelpDisplayAppRunner::registerOption(Application* pApp)
{
    pApp->addOption(Option(_T("help"), _T("h"), _T("display help"))
                    .required(false)
                    .repeatable(false)
                    .callback(SharedOptionCallback(
                                  this, &AppRunner::optionCallback))); 
    m_pOptionParser = pApp->getOptionParser();

}

void HelpDisplayAppRunner::optionCallback(const FX_NS(util)::Option& option,
                                          const tstring& sValue)
{
    m_bShowHelp = true;
}

FX_NS_END
