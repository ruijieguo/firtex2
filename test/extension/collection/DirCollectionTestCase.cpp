#include "DirCollectionTestCase.h"
#include "firtex/extension/collection/DirCollection.h"
#include "firtex/index/Index.h"
#include "TestHelper.h"

using namespace std;

FX_NS_USE(index);
FX_NS_USE(utility);

FX_NS_DEF(collection);

SETUP_STREAM_LOGGER(NONE, DirCollectionTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(DirCollectionTestCase);

DirCollectionTestCase::DirCollectionTestCase() 
{
}

DirCollectionTestCase::~DirCollectionTestCase() 
{
}

void DirCollectionTestCase::setUp()
{
}

void DirCollectionTestCase::tearDown()
{
}

void DirCollectionTestCase::testScan()
{
    string sConf = TestHelper::getTestDataPath();
	Path p(sConf);
	p.makeDirectory();
	p.pushDirectory("dir_collection");
	p.setFileName("config.xml");
    sConf = p.toString();
    try
    {
        CollectionPtr pCol = Collection::buildCollection(sConf);
        CPPUNIT_ASSERT(pCol);
        
        pCol->scan(true);
    }
    catch(const FirteXException& e)
    {
        cout << "===ERROR: " << e.what() << endl;
        CPPUNIT_ASSERT(false);
    }

    checkIndex();
}

string DirCollectionTestCase::getTestPath()
{
    return TestHelper::getTestDataPath("test_dir_collection", true);
}

void DirCollectionTestCase::checkIndex()
{
    string sIndex = TestHelper::getTestDataPath() + "/dir_collection/files_index";
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

