#include "IndexPrinterAppRunner.h"
#include "firtex/extension/FirteX-Ext.h"
#include "firtex/utility/TimeProbe.h"
#include "firtex/utility/StringTokenizer.h"
#include <ostream>
#include <istream>
#include "firtex/extension/util/AppRunner.h"
#include "firtex/extension/util/Application.h"
#include "firtex/extension/util/SharedOptionCallback.h"

using namespace std;

FX_ROOTNS_USE();
FX_NS_USE(util);
FX_NS_USE(utility);
FX_NS_USE(index);
FX_NS_USE(utility);
FX_NS_USE(analyzer);
FX_NS_USE(document);
FX_NS_USE(collection);

FX_NS_DEF(app);
IndexPrinterAppRunner::IndexPrinterAppRunner()
{
    m_bPrintPosition = false;
}

AppRunner::Status IndexPrinterAppRunner::run()
{
    if (m_sIndexPath.empty())
        return S_NONE;
    printIndex();
    return S_EXIT;
}
 
void IndexPrinterAppRunner::registerOption(Application* pApp)
{
    pApp->addDescription(_T("Print posting information of term : "));
    pApp->addDescription(tstring(_T("  ")) + pApp->getProgName() + 
                         _T(" -P [INDEX_PATH] -T [TERM] [-o] "));

    pApp->addOption(Option(_T("index-print"), _T("P"), _T("print posting list of term"))
                    .required(true)
                    .repeatable(false)
                    .argument("[INDEX_PATH]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option(_T("term"), _T("T"), _T("term to print"))
                    .required(true)
                    .repeatable(false)
                    .argument("[TERM]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option(_T("print-position"), _T("o"), _T("print position or not"))
                    .required(false)
                    .repeatable(false)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option(_T("lexicon-dir"), _T("l"), _T("lexicon directory"))
                    .required(false)
                    .repeatable(false)
                    .argument("[LEXICON_DIR]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback)));
}

void IndexPrinterAppRunner::optionCallback(const Option& option, const tstring& sValue)
{
    if (option.matchFull(_T("index-print")) || option.matchShort(_T("P")))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--index-print/-P");
        }
        m_sIndexPath = sValue;
    }
    else if (option.matchFull(_T("term")) || option.matchShort(_T("T")))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--term/-T");
        }
        m_sTerm = sValue;
    }
    else if (option.matchFull(_T("print-position")) || option.matchShort(_T("o")))
    {
        m_bPrintPosition = true;
    }
    else if (option.matchFull(_T("lexicon-dir")) || option.matchShort(_T("l")))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--def-field/-d");
        }
        GLOBAL_CONF().General.dictionaryPath = sValue;
    }
}

void IndexPrinterAppRunner::printIndex()
{
    Index index;
    index.open(m_sIndexPath, Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();	
    StringTokenizer st(m_sTerm.c_str(), _T(":"), StringTokenizer::TOKEN_TRIM
                       | StringTokenizer::TOKEN_IGNORE_EMPTY);
    if (st.getNumTokens() != 2)
    {
        cout << _T("Invalid query term: [") << m_sTerm << _T("]") 
             << _T("format: [field]:[term]") << endl;
        return ;
    }

    const Analyzer *pAnalyzer = index.getAnalyzerMapper()->getAnalyzer(st[0].c_str());
    if (!pAnalyzer)
    {
        cout << "No analyzer for field: [" << st[0] 
             << "], may be an invalid field" << endl;
        return ;
    }
    TokenViewPtr pTokens = pAnalyzer->tokenize(st[1]);
    if(pTokens->getNumTokens() <= 0)
    {
        cout << _T("The term is ignored by analyzer.") << endl;
        return ;
    }

    TokenView::Iterator iter = pTokens->iterator();
    while (iter.hasNext())
    {
        const Token& t = iter.next();
        TermPtr pTerm(new Term(st[0], t.getTextValue()));
        TermReaderPtr pTermReader = pReader->termReader();
        cout << _T("Term: ") << "<" << pTerm->toString() << ">" << endl;
        TermPostingIteratorPtr pPostIter = pTermReader->seek(pTerm.get());
        if (pPostIter)
        {
            if(!m_bPrintPosition)
            {
                cout << _T("  docFreq: ") << pPostIter->getTermMeta().getDocFreq() << endl;
                docid_t docId = INVALID_DOCID;
                while ((docId = pPostIter->skipTo(docId)) != INVALID_DOCID)
                {
                    cout << "   (" << docId;
                    cout << "," <<  pPostIter->freq() << ") ";
                    ++docId;
                }
                cout << endl;
            }
            else 
            {
                cout << _T("  docFreq: ") << pPostIter->getTermMeta().getDocFreq() << endl;
                docid_t docId = INVALID_DOCID;
                while ((docId = pPostIter->skipTo(docId)) != INVALID_DOCID)
                {
                    loc_t pos = INVALID_POS;
                    cout << _T("   (") << docId
                         << _T(",") << pPostIter->freq() << _T("<");
                    while ((pos = pPostIter->skipToPosition(++pos)) != INVALID_POS)
                    {
                        cout << pos << ",";
                    }
                    cout << "> )";
                    cout << endl;
                    ++docId;
                }
            }
        }
    }
}

FX_NS_END
