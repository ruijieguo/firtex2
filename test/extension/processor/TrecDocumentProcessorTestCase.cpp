#include "TestHelper.h"
#include "TrecDocumentProcessorTestCase.h"
#include "firtex/utility/XMLDocumentWrapper.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"
#include "firtex/utility/StringTokenizer.h"

using namespace std;

FX_NS_USE(document);
FX_NS_USE(utility);

FX_NS_DEF(processor);

SETUP_STREAM_LOGGER(NONE, TrecDocumentProcessorTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(TrecDocumentProcessorTestCase);

static const char TEST_FILE1[] = 
    "<DOC>"
    "<field1>value1</field1>"
    "<field2>value2</field2>"
    "<field3>value3</field3>"
    "<field4>value4</field4>"
    "<field5>value5</field5>"
    "</DOC>";

static const char TEST_FILE2[] = 
    "<DOC>"
    "<field1>value11</field1>"
    "<field2>value12</field2>"
    "<field3>value13</field3>"
    "<field4>value14</field4>"
    "<field5>value15</field5>"
    "</DOC>"
    "<DOC>"
    "<field1>value21</field1>"
    "<field2>value22</field2>"
    "<field3>value23</field3>"
    "<field4>value24</field4>"
    "<field5>value25</field5>"
    "</DOC>";

static const char TEST_FILE3[] = 
    "<DOC>"
    "<field1>value1</field1>"
    "<field2>value2</field2>"
    "<DOCHDR>"
    "http://www.ram.org:80/ramblings/movies/jimmy_hollywood.html 208.194.41.61 19970101152051 text/html 2080\r\n"
    "HTTP/1.0 200 Document follows\r\n"
    "MIME-Version: 1.0\r\n"
    "Server: CERN/3.0\r\n"
    "Date: Wednesday, 01-Jan-97 15:20:23 GMT\r\n"
    "Content-type: text/html\r\n"
    "Content-length: 1873\x0d\n"
    "Last-modified: Thursday, 23-Nov-95 03:11:57 GMT\r\n"
    "</DOCHDR>"
    "<html>"
    "<head>"
    "<title> Jimmy Hollywood movie review </title>"
    "</head>"
    "<h1> Jimmy Hollywood </h1>"
    "</html>\n"
    "</DOC>";

static const char TEST_FILE_MISS_FIELD[] = 
    "<DOC>"
    "<field2>value2</field2>"
    "<field3>value3</field3>"
    "<field5>value5</field5>"
    "</DOC>";

std::string TrecDocumentProcessorTestCase::getTestPath()
{
    return TestHelper::getTestDataPath("processor", true);
}

std::string TrecDocumentProcessorTestCase::getTestOutputPath()
{
    return TestHelper::getTestDataPath("test_processor", true);
}

TrecDocumentProcessorTestCase::TrecDocumentProcessorTestCase() 
{
}

TrecDocumentProcessorTestCase::~TrecDocumentProcessorTestCase() 
{
}

void TrecDocumentProcessorTestCase::setUp()
{
    m_pDocSchema = new DocumentSchema();
    m_pDocSchema->addTextField("field1");
    m_pDocSchema->addTextField("field2");
    m_pDocSchema->addTextField("field3");
    m_pDocSchema->addTextField("field4");
    m_pDocSchema->addTextField("field5");

    m_pDocTemp = new DocumentTemplate();
    m_pDocTemp->setDocTag(_T("DOC"));
    m_pDocTemp->addTag("field1", "field1");
    m_pDocTemp->addTag("field2", "field2");
    m_pDocTemp->addTag("field3", "field3");
    m_pDocTemp->addTag("field4", "field4");
    m_pDocTemp->addTag("field5", "field5");

    m_pDocTemp->makeSure(m_pDocSchema.get());

    File f(getTestOutputPath());
    if (f.exists())
    {
        f.remove(true);
    }
    f.createDirectory();
}

void TrecDocumentProcessorTestCase::tearDown()
{
    File f(getTestOutputPath());
    if (f.exists())
    {
        f.remove(true);
    }
}

void TrecDocumentProcessorTestCase::makeAnswer(const std::string& str, Answer& ans)
{
    XMLDocumentWrapper xmlDoc;
    xmlDoc.parse(str);
    
    for (XMLNodeWrapperPtr pDocNode = xmlDoc.firstNode();
         pDocNode.isNotNull(); pDocNode = pDocNode->nextSibling())
    {
        for (XMLNodeWrapperPtr pChildNode = pDocNode->firstNode();
             pChildNode.isNotNull(); pChildNode = pChildNode->nextSibling())
        {
            ans.push_back(make_pair(pChildNode->getName(), pChildNode->getValue()));
        }
    }
}

void TrecDocumentProcessorTestCase::testProcess()
{
    string sPath = writeTestFile("trec_file1.txt", TEST_FILE1);

    TrecDocumentProcessor processor;
    processor.init(m_pDocSchema.get(), m_pDocTemp.get());
    
    DocumentSource docSource(m_pDocSchema.get());
    RawDocumentPtr pRawDoc = new RawDocument();
    pRawDoc->setPath(sPath);
    docSource.setRawDocument(pRawDoc);

    processor.process(docSource);

    DocumentPtr pDoc = docSource.stealLastDocument();
    CPPUNIT_ASSERT(pDoc.isNotNull());

    Answer ans;
    makeAnswer(TEST_FILE1, ans);

    Document::Iterator it = pDoc->iterator();
    CPPUNIT_ASSERT_EQUAL(ans.size(), it.size());
    size_t i = 0;
    while (it.hasNext())
    {
        const Field* pField = it.next();
//        cout << ans[i].first << " : " << ans[i].second << endl;
        CPPUNIT_ASSERT_EQUAL(ans[i].first, pField->getFieldSchema()->getName());
        CPPUNIT_ASSERT_EQUAL(ans[i].second, std::string(pField->getValue().c_str()));
        ++i;
    }
}

void TrecDocumentProcessorTestCase::testProcessMultiFile()
{
    string sPath = writeTestFile("trec_file2.txt", TEST_FILE2);

    TrecDocumentProcessor processor;
    processor.init(m_pDocSchema.get(), m_pDocTemp.get());
    
    DocumentSource docSource(m_pDocSchema.get());

    Answer ans;
    makeAnswer(TEST_FILE2, ans);
    RawDocumentPtr pRawDoc = new RawDocument();
    pRawDoc->setPath(sPath);
    docSource.setRawDocument(pRawDoc);

    size_t i = 0;
    do
    {
        processor.process(docSource);

        DocumentPtr pDoc = docSource.stealLastDocument();
        CPPUNIT_ASSERT(pDoc.isNotNull());

        Document::Iterator it = pDoc->iterator();
        while (it.hasNext())
        {
            const Field* pField = it.next();
            CPPUNIT_ASSERT_EQUAL(ans[i].first, pField->getFieldSchema()->getName());
            CPPUNIT_ASSERT_EQUAL(ans[i].second, std::string(pField->getValue().c_str()));
            ++i;
        }
    } while(docSource.toBeContinued());
    CPPUNIT_ASSERT_EQUAL(ans.size(), i);
}

void TrecDocumentProcessorTestCase::testProcessFileMisField()
{
    string sPath = writeTestFile("trec_file3.txt", TEST_FILE_MISS_FIELD);

    TrecDocumentProcessor processor;
    processor.init(m_pDocSchema.get(), m_pDocTemp.get());
    
    DocumentSource docSource(m_pDocSchema.get());
    RawDocumentPtr pRawDoc = new RawDocument();
    pRawDoc->setPath(sPath);
    docSource.setRawDocument(pRawDoc);

    processor.process(docSource);

    DocumentPtr pDoc = docSource.stealLastDocument();
    CPPUNIT_ASSERT(pDoc.isNotNull());

    Answer ans;
    makeAnswer(TEST_FILE_MISS_FIELD, ans);

    Document::Iterator it = pDoc->iterator();
    CPPUNIT_ASSERT_EQUAL(ans.size(), it.size());
    size_t i = 0;
    while (it.hasNext())
    {
        const Field* pField = it.next();
//        cout << ans[i].first << " : " << ans[i].second << endl;
        CPPUNIT_ASSERT_EQUAL(ans[i].first, pField->getFieldSchema()->getName());
        CPPUNIT_ASSERT_EQUAL(ans[i].second, std::string(pField->getValue().c_str()));
        ++i;
    }
}

void TrecDocumentProcessorTestCase::testProcessGZipFile()
{
    TrecDocumentProcessor processor;
    processor.init(m_pDocSchema.get(), m_pDocTemp.get());
    
    DocumentSource docSource(m_pDocSchema.get());
    RawDocumentPtr pRawDoc = new RawDocument();
    pRawDoc->setPath(getTestPath() + "/1.gz");
    docSource.setRawDocument(pRawDoc);

    Answer ans;
    makeAnswer(TEST_FILE2, ans);

    size_t i = 0;
    do
    {
        processor.process(docSource);

        DocumentPtr pDoc = docSource.stealLastDocument();
        CPPUNIT_ASSERT(pDoc.isNotNull());

        Document::Iterator it = pDoc->iterator();
        while (it.hasNext())
        {
            const Field* pField = it.next();
            CPPUNIT_ASSERT_EQUAL(ans[i].first, pField->getFieldSchema()->getName());
            CPPUNIT_ASSERT_EQUAL(ans[i].second, std::string(pField->getValue().c_str()));
            ++i;
        }
    } while(docSource.toBeContinued());
    CPPUNIT_ASSERT_EQUAL(ans.size(), i);
}

void TrecDocumentProcessorTestCase::testProcessTrecFileWithMeta()
{
    m_pDocSchema = new DocumentSchema();
    m_pDocSchema->addTextField("field1");
    m_pDocSchema->addTextField("field2");
    m_pDocSchema->addTextField("url");
    m_pDocSchema->addTextField("date");
    m_pDocSchema->addTextField("content_type");
    m_pDocSchema->addTextField("content_length");
    m_pDocSchema->addTextField("last_modified");
    m_pDocSchema->addTextField("title");
    m_pDocSchema->addTextField("body");

    m_pDocTemp = new DocumentTemplate();
    m_pDocTemp->setDocTag("DOC");
    m_pDocTemp->setEmbeddedField("body");

    m_pDocTemp->addMeta("url", "url");
    m_pDocTemp->addMeta("Date", "date");
    m_pDocTemp->addMeta("Content-Type", "content_type");
    m_pDocTemp->addMeta("Content-Length", "content_length");
    m_pDocTemp->addMeta("Last-Modified", "last_modified");
    m_pDocTemp->addMeta("title", "title");

    m_pDocTemp->addTag("field1", "field1");
    m_pDocTemp->addTag("field2", "field2");
    m_pDocTemp->addTag("DOCHDR");

    m_pDocTemp->makeSure(m_pDocSchema.get());

    string sPath = writeTestFile("trec_file3.txt", TEST_FILE3);

    TrecDocumentProcessor processor;
    processor.setContentType("html");
    processor.init(m_pDocSchema.get(), m_pDocTemp.get());
    
    DocumentSource docSource(m_pDocSchema.get());

    RawDocumentPtr pRawDoc = new RawDocument();
    pRawDoc->setPath(sPath);
    docSource.setRawDocument(pRawDoc);

    processor.process(docSource);

    DocumentPtr pDoc = docSource.stealLastDocument();
    CPPUNIT_ASSERT(pDoc.isNotNull());

    Document::Iterator it = pDoc->iterator();
    CPPUNIT_ASSERT(it.hasNext());
    const Field* pField = it.next();
    CPPUNIT_ASSERT_EQUAL(string("value1"), string(pField->getValue().c_str()));

    CPPUNIT_ASSERT(it.hasNext());
    pField = it.next();
    CPPUNIT_ASSERT_EQUAL(string("value2"), string(pField->getValue().c_str()));

    CPPUNIT_ASSERT(it.hasNext());
    pField = it.next();
    CPPUNIT_ASSERT_EQUAL(string("http://www.ram.org:80/ramblings/movies/jimmy_hollywood.html"),
                         string(pField->getValue().c_str()));
    CPPUNIT_ASSERT(it.hasNext());
    pField = it.next();
    CPPUNIT_ASSERT_EQUAL(string("Wednesday, 01-Jan-97 15:20:23 GMT"),
                         string(pField->getValue().c_str()));

    CPPUNIT_ASSERT(it.hasNext());
    pField = it.next();
    CPPUNIT_ASSERT_EQUAL(string("text/html"),
                         string(pField->getValue().c_str()));

    CPPUNIT_ASSERT(it.hasNext());
    pField = it.next();
    CPPUNIT_ASSERT_EQUAL(string("1873"),
                         string(pField->getValue().c_str()));

    CPPUNIT_ASSERT(it.hasNext());
    pField = it.next();
    CPPUNIT_ASSERT_EQUAL(string("Thursday, 23-Nov-95 03:11:57 GMT"),
                         string(pField->getValue().c_str()));

    CPPUNIT_ASSERT(it.hasNext());
    pField = it.next();
    CPPUNIT_ASSERT_EQUAL(string(" Jimmy Hollywood movie review "),
                         string(pField->getValue().c_str()));

    CPPUNIT_ASSERT(it.hasNext());
    pField = it.next();
    CPPUNIT_ASSERT_EQUAL(string(" Jimmy Hollywood "),
                         string(pField->getValue().c_str()));
}

string TrecDocumentProcessorTestCase::writeTestFile(const string& sFileName, 
                                                      const char* szFileContent)
{
    std::string sPath = getTestOutputPath();
    sPath += "/" + sFileName;
    
    BinaryFile bf;
    bf.open(sPath, BinaryFile::CREATE);
    bf.write(szFileContent, strlen(szFileContent));
    bf.close();

    return sPath;
}

FX_NS_END
