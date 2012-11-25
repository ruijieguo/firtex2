#include "IndexOverviewAppRunner.h"
#include "firtex/extension/FirteX-Ext.h"
#include "firtex/utility/TimeProbe.h"
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

IndexOverviewerAppRunner::IndexOverviewerAppRunner()
{
}

AppRunner::Status IndexOverviewerAppRunner::run()
{
    if (m_sIndexPath.empty())
        return S_NONE;
    overview();
    return S_EXIT;
}
 
void IndexOverviewerAppRunner::registerOption(Application* pApp)
{
    pApp->addDescription(_T("Print index information : "));
    pApp->addDescription(tstring(_T("  ")) + pApp->getProgName() + 
                         _T(" -n [INDEX_PATH]"));

    pApp->addOption(Option(_T("look-index"), _T("n"), _T("print index infomation"))
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

void IndexOverviewerAppRunner::optionCallback(const Option& option, const tstring& sValue)
{
    if (option.matchFull(_T("look-index")) || option.matchShort(_T("n")))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--look-index/-n");
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

void IndexOverviewerAppRunner::overview()
{
    try
    {
        if (!m_sLexiDir.empty())
        {
            GLOBAL_CONF().General.dictionaryPath = m_sLexiDir;
        }

        Index index;
        index.open(m_sIndexPath, Index::READ, NULL);
        IndexReaderPtr pReader = index.acquireReader();	

        printf("Index path:\t\t\t%s\n", m_sIndexPath.c_str());
        printf("Version:\t\t\t%s\n",pReader->getIndexVersion().c_str());

        const BarrelsInfo* pBarrelsInfo = pReader->getBarrelsInfo();
        printf("Number of index barrels:\t%d\n", (int)pBarrelsInfo->getBarrelCount());

        printf("Number of documents:\t\t%d\n", pReader->getDocCount());
        printf("Number of deleted documents:\t%u\n", pReader->getDeletedDocCount());

        printf("Term information (per field):\n");

        const IndexMeta& indexMeta = pReader->getIndexMeta();
        for (size_t i = 0; i < indexMeta.size(); ++i)
        {
            const FieldMeta& fieldMeta = indexMeta[i];
            printf("Field: %s\ttotal term count: %llu\tdistinct term count: %llu\n",
                   fieldMeta.fieldName.c_str(), (long long unsigned int)fieldMeta.totalTermCount,
                   (long long unsigned int)fieldMeta.distinctTermCount);
        }

        DateTime dt = pReader->lastModified();
        tstring str;
        dt.format(str, DateTime::SORTABLE_FORMAT);
        printf("Last modified:\t\t\t%s\n", str.c_str());

        //barrels
        printf("\nIndex barrels:\n");
        printf("\tname\t\tstart docid\tnumDocs\n");
        BarrelsInfo::Iterator iter = pBarrelsInfo->iterator();
        while (iter.hasNext())
        {
            const BarrelInfo& baInfo = iter.next();
            printf("\t%s\t\t%u\t\t%u\n", baInfo.getSuffix().c_str(),
                   baInfo.getBaseDocId(), baInfo.getDocCount());
        }

        //fields
        printf("\nIndex fields:\n");

        const DocumentSchema* pDocSchema = pReader->getDocSchema();
        DocumentSchema::Iterator iter2 = pDocSchema->iterator();
        while (iter2.hasNext())
        {
            const FieldSchema* pFieldSchema = iter2.next();
            printf("\t%s: ", pFieldSchema->getName().c_str());

            if(pFieldSchema->isStored())
                printf("stored, ");
            if(pFieldSchema->isAnalyzed())
                printf("analyzed, ");
            if(pFieldSchema->isIndexed())
                printf("indexed");
            if (pFieldSchema->hasForwardIndex())
                printf("forwardindexed");
            printf("\n");
        }
        printf("\n");
    }
    catch(const FirteXException& fe)
    {
        cout << _T("Look index FAILED: ") << fe.what() << endl;
    }
}

FX_NS_END
