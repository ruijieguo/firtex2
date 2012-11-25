#include "FileIndexerAppRunner.h"
#include "firtex/extension/FirteX-Ext.h"
#include "firtex/utility/TimeProbe.h"
#include "firtex/extension/util/Application.h"
#include "firtex/extension/util/SharedOptionCallback.h"
#include "firtex/extension/util/AppRunner.h"
#include <memory>

using namespace std;
FX_NS_USE(util);
FX_NS_USE(index);
FX_NS_USE(utility);
FX_NS_USE(document);
FX_NS_USE(collection);

FX_NS_DEF(app);

FileIndexerAppRunner::FileIndexerAppRunner()
{
}

AppRunner::Status FileIndexerAppRunner::run()
{
    if (m_sFilePath.empty() || m_sIndexPath.empty())
        return S_NONE;
    indexFile();
    return S_EXIT;
}
 
void FileIndexerAppRunner::registerOption(Application* pApp)
{
    pApp->addDescription(_T("Build index from files: "));
    pApp->addDescription(tstring(_T("  ")) + pApp->getProgName() + 
                         _T(" -f [FILE_PATH] -i [INDEX_PATH] -t [plain/html/pdf]"));

    pApp->addOption(Option(_T("index-file"), _T("f"), _T("source file path to index"))
                    .required(true)
                    .repeatable(false)
                    .argument("[FILE_PATH]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option(_T("index-path"), _T("i"), _T("index path"))
                    .required(true)
                    .repeatable(false)
                    .argument("[INDEX_PATH]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option(_T("file-type"), _T("t"), _T("source file type(plain, html, pdf)"))
                    .required(true)
                    .repeatable(false)
                    .argument("[plain/html/pdf]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

}

void FileIndexerAppRunner::optionCallback(const Option& option, const tstring& sValue)
{
    if (option.matchFull(_T("index-file")) || option.matchShort(_T("f")))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--file-path or -f");
        }
        m_sFilePath = sValue;
    }
    else if (option.matchFull(_T("index-path")) || option.matchShort(_T("i")))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--index-path or -i");
        }
        m_sIndexPath = sValue;
    }
    else if (option.matchFull(_T("file-type")) || option.matchShort(_T("t")))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--file-type or -t");
        } 
        else if (!sValue.compare(_T("plain")) 
                 || !sValue.compare(_T("html"))
                 || !sValue.compare(_T("pdf")))
        {
            m_sFileType = sValue;
        }
        else 
        {   
            throw UnknownOptionException("--file-type=[plain|html|pdf]");
        }
    }
}

void FileIndexerAppRunner::indexFile()
{
    CollectionConfigurePtr pConfig(new CollectionConfigure);
    pConfig->Collection.source = m_sFilePath;
    pConfig->Collection.target = m_sIndexPath;
    pConfig->Collection.buildMode = _T("batch");

    pConfig->Collection.identifier = _T("collection.directory");
    pConfig->Collection.docTypes = _T("txt,html,pdf");
    
    DocumentSchema* pDocSchema = new DocumentSchema();
    auto_ptr<DocumentSchema> schemaPtr(pDocSchema);
    
    pDocSchema->addUnIndexedField(_T("PATH"));
    if (m_sFileType == _T("html") || m_sFileType == _T("pdf"))
    {
        pDocSchema->addTextField(_T("TITLE"), true);
    }
    pDocSchema->addTextField(_T("TEXT"));

    try
    {
        cout << "Begin build index..." << endl;
        pConfig->setDocSchema(pDocSchema);

        TimeProbe probe;
        probe.start();
        FX_NS(collection)::Collection::buildIndex(pConfig);
        probe.stop();

        cout << "Build index success! finished in " << probe.elapsedSeconds() 
             << " s." << endl;
    }
    catch (const FirteXException& fe) 
    {
        cout << "Build index FAILED: " << fe.what() << endl;
    }
}

FX_NS_END
