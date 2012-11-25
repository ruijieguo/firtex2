#include "DocumentOverviewAppRunner.h"
#include "firtex/extension/FirteX-Ext.h"
#include "firtex/utility/TimeProbe.h"
#include "firtex/utility/Number.h"
#include <ostream>
#include <istream>
#include "firtex/extension/util/AppRunner.h"
#include "firtex/extension/util/Application.h"
#include "firtex/extension/util/SharedOptionCallback.h"

using namespace std;

FX_ROOTNS_USE();
FX_NS_USE(util);
FX_NS_USE(index);
FX_NS_USE(utility);
FX_NS_USE(analyzer);
FX_NS_USE(document);
FX_NS_USE(collection);

FX_NS_DEF(app);

DocumentOverviewAppRunner::DocumentOverviewAppRunner()
{
}

AppRunner::Status DocumentOverviewAppRunner::run()
{
    if (m_sIndexPath.empty())
        return S_NONE;
    printDocument();
    return S_EXIT;
}
 
void DocumentOverviewAppRunner::registerOption(Application* pApp)
{
    pApp->addDescription(_T("Print document: "));
    pApp->addDescription(tstring(_T("  ")) + pApp->getProgName() + 
                         _T(" -r [INDEX_PATH] -N [DOCID]"));

    pApp->addOption(Option(_T("print-document"), _T("r"), _T("print document in index"))
                    .required(true)
                    .repeatable(false)
                    .argument("[INDEX_PATH]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option(_T("docid"), _T("N"), _T("document id"))
                    .required(true)
                    .repeatable(false)
                    .argument("[DOCID]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 
}

void DocumentOverviewAppRunner::optionCallback(const Option& option, const tstring& sValue)
{
    if (option.matchFull(_T("print-document")) || option.matchShort(_T("r")))
    {
        if (sValue.empty())
        {
            FIRTEX_THROW(MissingArgumentException, "--print-document/-r");
        }
        m_sIndexPath = sValue;
    }
    else if (option.matchFull(_T("docid")) || option.matchShort(_T("N")))
    {

        if (sValue.empty() || !NumberParser::tryParseInt32(sValue, m_nDocId))
        {
            throw MissingArgumentException("--docid/-N: "
                    "please specify document ID (numeric)");
        }
    }
}

void DocumentOverviewAppRunner::printDocument()
{
    try
    {
        Index index;
        index.open(m_sIndexPath, Index::READ, NULL);
        IndexReaderPtr pReader = index.acquireReader();	
        StoredFieldsReaderPtr pDocReader = pReader->createStoredFieldsReader();
        if (m_nDocId >= pReader->getMaxDocs())
        {
            cout << _T("There are only ") << pReader->getMaxDocs() 
                 << _T(" documents") << endl;
        }
        else if(pReader->isDeleted(m_nDocId))
        {
            cout << _T("Document: [") << m_nDocId << _T(" is deleted.") << endl;
        }
        else 
        {
            FieldSelector selector(pReader->getDocSchema(), true, false);
            ResultDoc resultDoc(m_nDocId);
            pDocReader->getDocument(selector, resultDoc);
            displayDoc(resultDoc);
        }        
    }
    catch(const FirteXException& fe)
    {		
        cout << _T("Print document FAILED: [") << fe.what() << _T("].") << endl;
    }
}

void DocumentOverviewAppRunner::displayDoc(const ResultDoc& resDoc)
{
    cout << "============================================="<<endl;
    cout << "doc id: "<< resDoc.getDocId() << endl;
    cout << "score:" << resDoc.getScore() << endl;

    ResultDoc::Iterator iter = resDoc.iterator();
    while (iter.hasNext())
    {
        const ResultDoc::Field& field = iter.next();
        cout << field.first << "\t->\t" << field.second << endl;
    }				
}

FX_NS_END
