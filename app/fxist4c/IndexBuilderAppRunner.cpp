#include "IndexBuilderAppRunner.h"
#include "firtex/extension/FirteX-Ext.h"
#include "firtex/utility/TimeProbe.h"
#include "firtex/extension/util/SharedOptionCallback.h"
#include "firtex/extension/util/Application.h"
#include "firtex/extension/collection/ConsoleProgressObserver.h"

using namespace std;
FX_NS_USE(util);
FX_NS_USE(index);
FX_NS_USE(utility);
FX_NS_USE(collection);

FX_NS_DEF(app);

IndexBuilderAppRunner::IndexBuilderAppRunner()
{
}

AppRunner::Status IndexBuilderAppRunner::run()
{
    if (m_sConfFile.empty())
        return S_NONE;
    buildIndex();
    return S_EXIT;
}
 
void IndexBuilderAppRunner::registerOption(Application* pApp)
{
    pApp->addDescription(_T("Build index from configure file: "));
    pApp->addDescription(tstring(_T("  ")) + pApp->getProgName() + 
                         _T(" -b [CONF_FILE]"));

    pApp->addOption(Option(_T("build-index"), _T("b"), _T("build index database"))
                    .required(true)
                    .repeatable(false)
                    .argument("[CONF_FILE]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 
}

void IndexBuilderAppRunner::optionCallback(const Option& option, const tstring& sValue)
{
    if (option.matchFull(_T("build-index")) || option.matchShort(_T("b")))
    {
        if (sValue.empty())
        {
            FIRTEX_THROW(MissingArgumentException, "--build-index/-b");
        }
        m_sConfFile = sValue;
    }
}

void IndexBuilderAppRunner::buildIndex()
{
    try
    {
        cout << _T("Begin build index, config file: ") << m_sConfFile << endl;
        TimeProbe p;
        p.start();
        ProgressObserverPtr pOb(new ConsoleProgressObserver(1000));
        FX_NS(collection)::Collection::buildIndex(m_sConfFile, pOb);
        p.stop();
        cout << _T("Build index success! finished in ") << p.elapsedSeconds() 
             << _T(" s.") << endl;
    }
    catch(const FirteXException& fe)
    {
        cout << _T("Build index FAILED: ") << fe.what() << endl;
    }
}

FX_NS_END
