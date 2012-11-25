#include "LexiconBuilderAppRunner.h"
#include "firtex/extension/FirteX-Ext.h"
#include "firtex/utility/TimeProbe.h"
#include "firtex/extension/util/SharedOptionCallback.h"
#include "firtex/extension/util/Application.h"
#include "firtex/analyzer/DoubleArrayTrie.h"

using namespace std;
FX_NS_USE(analyzer);
FX_NS_USE(utility);
FX_NS_USE(util);
FX_NS_USE(common);

FX_NS_DEF(app);

LexiconBuilderAppRunner::LexiconBuilderAppRunner()
{
}

AppRunner::Status LexiconBuilderAppRunner::run()
{
    if (m_sSourceFile.empty() || m_sTargetFile.empty())
        return S_NONE;
    buildLexicon();
    return S_EXIT;
}
 
void LexiconBuilderAppRunner::registerOption(Application* pApp)
{
    pApp->addDescription(_T("Build lexicon from raw lexicon file: "));
    pApp->addDescription(tstring(_T("  ")) + pApp->getProgName() + 
                         _T(" -s [SOURCE_FILE] -t [TARGET_FILE]"));

    pApp->addOption(Option(_T("source-file"), _T("s"),
                           _T("file path of source raw lexicon"))
                    .required(true)
                    .repeatable(false)
                    .argument("[SOURCE_FILE]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback)));

    pApp->addOption(Option(_T("target-file"), _T("t"),
                           _T("file path of source raw lexicon"))
                    .required(true)
                    .repeatable(false)
                    .argument("[TARGET_FILE]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 
}

void LexiconBuilderAppRunner::optionCallback(const Option& option, const tstring& sValue)
{
    if (option.matchFull(_T("source-file")) || option.matchShort(_T("s")))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--source-file/-s");
        }
        m_sSourceFile = sValue;
    }

    if (option.matchFull(_T("target-file")) || option.matchShort(_T("t")))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--target-file/-t");
        }
        m_sTargetFile = sValue;
    }
}

void LexiconBuilderAppRunner::buildLexicon()
{
    try
    {
        cout << "Begin build lexicon, source file: " << m_sSourceFile << endl
             << ", target file: " << m_sTargetFile << endl;

        TimeProbe p;
        DoubleArrayTrie trie(100000);
        trie.buildFromFile(m_sSourceFile);
        trie.save(m_sTargetFile);

        cout << _T("Build lexicon success! finished in ") << p.elapsedSeconds() 
             << _T(" s.") << endl;
    }
    catch(const FirteXException& fe)
    {
        cout << _T("Build lexicon FAILED: ") << fe.what() << endl;
    }
}

FX_NS_END
