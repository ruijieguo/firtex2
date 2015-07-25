#include "SearchResourceTestCase.h"
#include "index/IndexBuildHelper.h"
#include "TestHelper.h"
#include "ServiceConf.h"
#include "SearchResource.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"
#include "firtex/index/Index.h"


using namespace std;

FX_NS_USE(utility);
FX_NS_USE(index);
FX_NS_USE(document);

FX_NS_DEF(app);

SETUP_LOGGER(NONE, SearchResourceTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(SearchResourceTestCase);

SearchResourceTestCase::SearchResourceTestCase() 
{
}

SearchResourceTestCase::~SearchResourceTestCase() 
{
}

void SearchResourceTestCase::setUp()
{
    File f(getTestPath("", "", ""));
    if (f.exists())
    {
        f.remove(true);
    }
}

void SearchResourceTestCase::tearDown()
{

    File f(getTestPath("", "", ""));
    if (f.exists())
    {
        f.remove(true);
    }
}

void SearchResourceTestCase::testLoadClusters()
{
    string sCluster = "cluster1";
    string sType = "type1";
    string sGen = "0";
    buildIndex(sCluster, sType, sGen);

    ServiceConf conf;
    conf.Path.data_dir = getTestPath("", "", "");
    SearchResource res;
    try
    {
        res.init(conf);
    }
    catch(const FirteXException& e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
        CPPUNIT_ASSERT(false);
    }

    IndexReaderPtr pIndexReader = res.getIndexReader(sCluster, sType);
    CPPUNIT_ASSERT(pIndexReader != NULL);
}
 
void SearchResourceTestCase::testReloadTypes()
{
    string sCluster = "cluster1";
    string sType = "type1";
    string sGen = "0";
    buildIndex(sCluster, sType, sGen);

    ServiceConf conf;
    conf.Path.data_dir = getTestPath("", "", "");
    SearchResource res;
    res.init(conf);

    string sType2 = "type2";
    buildIndex(sCluster, sType2, sGen);
    res.refresh();

    IndexReaderPtr pIndexReader = res.getIndexReader(sCluster, sType2);
    CPPUNIT_ASSERT(pIndexReader != NULL);

    string sType1Path = getTestPath(sCluster, sType, "");
    File file(sType1Path);
    file.remove(true);

    res.refresh();
    IndexReaderPtr pIndexReader1 = res.getIndexReader(sCluster, sType);
    CPPUNIT_ASSERT(pIndexReader1 == NULL);
}

void SearchResourceTestCase::testReloadClusters()
{
    string sCluster = "cluster1";
    string sType = "type1";
    string sGen = "0";
    buildIndex(sCluster, sType, sGen);

    ServiceConf conf;
    conf.Path.data_dir = getTestPath("", "", "");
    SearchResource res;
    res.init(conf);

    string sCluster2 = "cluster2";
    buildIndex(sCluster2, sType, sGen);
    res.refresh();

    IndexReaderPtr pIndexReader = res.getIndexReader(sCluster2, sType);
    CPPUNIT_ASSERT(pIndexReader != NULL);

    string sType1Path = getTestPath(sCluster, sType, "");
    File file(sType1Path);
    file.remove(true);

    res.refresh();
    IndexReaderPtr pIndexReader1 = res.getIndexReader(sCluster, sType);
    CPPUNIT_ASSERT(pIndexReader1 == NULL);
}

void SearchResourceTestCase::testReloadGeneration()
{
    string sCluster = "cluster1";
    string sType = "type1";
    string sGen = "0";
    buildIndex(sCluster, sType, sGen);

    ServiceConf conf;
    conf.Path.data_dir = getTestPath("", "", "");
    SearchResource res;
    res.init(conf);

    string sGen2 = "1";
    buildIndex(sCluster, sType, sGen2, "docid3, generation", false);
    
    res.refresh();
    IndexReaderPtr pIndexReader1 = res.getIndexReader(sCluster, sType);
    CPPUNIT_ASSERT(pIndexReader1 != NULL);
    CPPUNIT_ASSERT_EQUAL((df_t)1, pIndexReader1->getDocCount());

    buildIndex(sCluster, sType, sGen2, "docid4, new doc", true);
    
    res.refresh();
    IndexReaderPtr pIndexReader2 = res.getIndexReader(sCluster, sType);
    CPPUNIT_ASSERT(pIndexReader2 != NULL);
    CPPUNIT_ASSERT_EQUAL((df_t)2, pIndexReader2->getDocCount());
}

string SearchResourceTestCase::getTestPath(const string& sCluster,
        const string& sType, const string& sGen)
{
    string str = TestHelper::getTestDataPath();
    Path p(str);
    p.makeDirectory();
    p.pushDirectory("test_searchresource");
    if (!sCluster.empty())
    {
        p.pushDirectory(sCluster);
    }
    if (!sType.empty())
    {
        p.pushDirectory(sType);
    }
    if (!sGen.empty())
    {
        p.pushDirectory(sGen);
    }
    return p.toString();
}

void SearchResourceTestCase::buildIndex(const string& sCluster,
                                        const string& sType,
                                        const string& sGen)
{
    buildIndex(sCluster, sType, sGen, 
               "docid1, hello world; docid2, this is a smoke test",
               false);
}

void SearchResourceTestCase::buildIndex(const string& sCluster,
                                        const string& sType,
                                        const string& sGen,
                                        const string& sDocs,
                                        bool bInc)
{
    try
    {
        GLOBAL_CONF().Build.buildThreadCount = 1;
        string indexDir = getTestPath(sCluster, sType, sGen);
        File file(indexDir);
        bool ret = file.createDirectories();
        CPPUNIT_ASSERT(ret);

        DocumentSchema schema;
        schema.addUnIndexedField("docid");
        schema.addTextField("BODY");
        IndexBuildHelper::buildIndex(indexDir, schema,
                sDocs, bInc);
    }
    catch(const FirteXException& e)
    {
        cout << "ERROR: " << e.what() << endl;
        CPPUNIT_ASSERT(false);
    }
}

FX_NS_END

