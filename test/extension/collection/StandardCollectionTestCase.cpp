#include "StandardCollectionTestCase.h"
#include "firtex/extension/collection/StandardCollection.h"
#include "firtex/index/Index.h"
#include "TestHelper.h"

using namespace std;

FX_NS_USE(index);
FX_NS_USE(utility);
FX_NS_USE(thread);
FX_NS_USE(document);

FX_NS_DEF(collection);

SETUP_STREAM_LOGGER(NONE, StandardCollectionTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(StandardCollectionTestCase);

StandardCollectionTestCase::StandardCollectionTestCase() 
{
}

StandardCollectionTestCase::~StandardCollectionTestCase() 
{
}

void cleanFiles()
{
    string sListFile = TestHelper::getTestDataPath() +
                       "/standard_collection/files/INCREMENTAL_LIST";
    File f(sListFile);
    if (f.exists())
    {
        f.remove(true);
    }

    string sIndexFiles = TestHelper::getTestDataPath() +
                       "/standard_collection/files_index";
    File f2(sIndexFiles);
    if (f2.exists())
    {
        f2.remove(true);
    }
}
void StandardCollectionTestCase::setUp()
{
    m_pConf = new CollectionConfigure;
    m_pDocSchema = new DocumentSchema();
    m_pDocSchema->addField("field1", "PRIMARY_KEY", false);
    m_pDocSchema->addTextField("field2");
    m_pDocSchema->addTextField("field3");

    m_pConf->setDocSchema(m_pDocSchema);

    cleanFiles();
}

void StandardCollectionTestCase::tearDown()
{
}

string StandardCollectionTestCase::getFilePath(const string& sDirName, const string& sFileName)
{
	if (sFileName.empty())
	{
		return TestHelper::getTestDataPath(sDirName, true);
	}
	else
	{
		Path p(TestHelper::getTestDataPath(sDirName, true));
		p.makeDirectory();
		p.setFileName(sFileName);
		return p.toString();
	}
}

void StandardCollectionTestCase::testScan()
{    
    string sConf = getFilePath("standard_collection", "standard.xml");
    CollectionPtr pCol = Collection::buildCollection(sConf);
    CPPUNIT_ASSERT(pCol);

    pCol->scan(true);

    checkIndex();
}

void StandardCollectionTestCase::testScanAsync()
{    
    string sConf = getFilePath("standard_collection", "standard.xml");
    CollectionPtr pCol = Collection::buildCollection(sConf);
    CPPUNIT_ASSERT(pCol);

    pCol->scan(false);
    pCol->waitStop();

    checkIndex();
}

void StandardCollectionTestCase::testScanWithMonitor()
{    
    string sConf = getFilePath("standard_collection", "standard_with_monitor.xml");
    CollectionPtr pCol = Collection::buildCollection(sConf);
    CPPUNIT_ASSERT(pCol);

    pCol->scan(false);
    Thread::sleep(1000);
    pCol->waitStop();

    checkIndex();
}

void StandardCollectionTestCase::checkIndex()
{
    string sIndex = TestHelper::getTestDataPath() + "/standard_collection/files_index";
    Index index;
    try
    {
        index.open(sIndex, Index::READ, NULL);

        IndexReaderPtr pReader = index.acquireReader();
        CPPUNIT_ASSERT(pReader != NULL);        
    }
    catch(const FirteXException& e)
    {
        cout << "===ERROR: " << e.what() << endl;
        CPPUNIT_ASSERT(false);
    }
}

FX_NS_END

