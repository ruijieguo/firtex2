#include "IndexMergerAppRunner.h"
#include "firtex/extension/FirteX-Ext.h"
#include "firtex/utility/TimeProbe.h"
#include "firtex/extension/util/SharedOptionCallback.h"
#include "firtex/extension/util/AppRunner.h"
#include "firtex/extension/util/Application.h"

using namespace std;
FX_NS_USE(util);
FX_NS_USE(index);
FX_NS_USE(utility);
FX_NS_USE(collection);

FX_NS_DEF(app);

IndexMergerAppRunner::IndexMergerAppRunner()
{
    m_nMergeMode = 0;
}

AppRunner::Status IndexMergerAppRunner::run()
{
    if (!m_nMergeMode || m_sIndexPath.empty())
        return S_NONE;
    indexMerge();
    return S_EXIT;
}
 
void IndexMergerAppRunner::registerOption(Application* pApp)
{
    pApp->addDescription(_T("Merge index inplace: "));
    pApp->addDescription(tstring(_T("  ")) + pApp->getProgName() + 
                         _T(" -m [merge/optimize] -i [INDEX_PATH] [-l [LEXICON_DIR]]"));

    pApp->addOption(Option(_T("merge-index"), _T("m"), _T("merge index"))
                    .required(true)
                    .repeatable(false)
                    .argument("[merge/optimize]", false)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option(_T("index-path"), _T("i"), _T("index path"))
                    .required(true)
                    .repeatable(false)
                    .argument("[INDEX_PATH]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option(_T("lexicon-dir"), _T("l"), _T("lexicon directory"))
                    .required(false)
                    .repeatable(false)
                    .argument("[LEXICON_DIR]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback)));
}

void IndexMergerAppRunner::optionCallback(const Option& option, const tstring& sValue)
{
    if (option.matchFull(_T("merge-index")) || option.matchShort(_T("m")))
    {
        if (sValue.empty() || (!sValue.compare(_T("merge"))))
        {
            m_nMergeMode = 1;
        }
        else if (!sValue.compare(_T("optimize")))
        {
            m_nMergeMode = 2;
        }
        else 
        {
            throw UnknownOptionException(sValue);
        }
    }
    else if (option.matchFull(_T("index-path")) || option.matchShort(_T("i")))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--index-path/-i");
        }
        m_sIndexPath = sValue;
    }
    else if (option.matchFull(_T("lexicon-dir")) || option.matchShort(_T("l")))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--def-field/-d");
        }
        m_sLexiDir = sValue;
    }
}

void IndexMergerAppRunner::indexMerge()
{
    try
    {
        if (!m_sLexiDir.empty())
        {
            GLOBAL_CONF().General.dictionaryPath = m_sLexiDir;
        }

        std::cout << "Open index: [ " << m_sIndexPath << "]" << std::endl;

        IndexPtr pIndex(new Index());
        pIndex->open(m_sIndexPath, Index::APPEND, NULL);
        IndexWriterPtr pWriter = pIndex->acquireWriter();

        if (m_nMergeMode == 2)
        {
            std::cout << "Begin optimize index: [ " << m_sIndexPath << "]" << std::endl;
            pWriter->optimizeIndex();
        }
        else 
        {
            std::cout << "Begin merge index: [ " << m_sIndexPath << "]" << std::endl;
            pWriter->mergeIndex();
        }
    }
    catch(const FirteXException& fe)
    {
        cout << "Merge index FAILED: " << fe.what() << endl;
    }
}

FX_NS_END
