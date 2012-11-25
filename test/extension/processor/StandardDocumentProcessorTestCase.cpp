#include "TestHelper.h"
#include "StandardDocumentProcessorTestCase.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"
#include "firtex/utility/StringTokenizer.h"

using namespace std;

FX_NS_USE(document);
FX_NS_USE(utility);

FX_NS_DEF(processor);

SETUP_STREAM_LOGGER(NONE, StandardDocumentProcessorTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(StandardDocumentProcessorTestCase);

static const char TEST_FILE1[] = 
    "DOC=ADD\n"
    "field1=value1\n"
    "field2=value2\n"
    "field3=value3\n"
    "field4=value4\n"
    "field5=value5\n"
    "\n";

static const char TEST_FILE2[] = 
    "DOC=ADD\n"
    "field1=value11\n"
    "field2=value12\n"
    "field3=value13\n"
    "field4=value14\n"
    "field5=value15\n"
    "\n"
    "DOC=ADD\n"
    "field1=value21\n"
    "field2=value22\n"
    "field3=value23\n"
    "field4=value24\n"
    "field5=value25\n"
    "\n";

static const char TEST_FILE_MISS_FIELD[] = 
    "DOC=ADD\n"
    "field2=value2\n"
    "field3=value3\n"
    "field5=value5\n"
    "\n";

static const char TEST_FILE_WITH_EMPTY_FIELD[] = 
    "DOC=ADD\n"
    "field1=\n"
    "field2=value2\n"
    "field3=value3\n"
    "field4=\n"
    "field5=value5\n"
    "\n";

std::string getTestPath()
{
    tstring str = TestHelper::getTestDataPath();
    Path p(str);
    p.makeDirectory();
    p.pushDirectory("/test_processor");
    return p.toString();
}

StandardDocumentProcessorTestCase::StandardDocumentProcessorTestCase() 
{
}

StandardDocumentProcessorTestCase::~StandardDocumentProcessorTestCase() 
{
}

void StandardDocumentProcessorTestCase::setUp()
{
    m_pDocSchema = new DocumentSchema();
    m_pDocSchema->addTextField("field1");
    m_pDocSchema->addTextField("field2");
    m_pDocSchema->addTextField("field3");
    m_pDocSchema->addTextField("field4");
    m_pDocSchema->addTextField("field5");

    File f(getTestPath());
    if (f.exists())
    {
        f.remove(true);
    }
    f.createDirectory();
}

void StandardDocumentProcessorTestCase::tearDown()
{
    File f(getTestPath());
    if (f.exists())
    {
        f.remove(true);
    }
}

void StandardDocumentProcessorTestCase::makeAnswer(const std::string& str, Answer& ans)
{
    StringTokenizer st(str, "\n", StringTokenizer::TOKEN_IGNORE_EMPTY);
    for (size_t i = 0; i < st.getNumTokens(); ++i)
    {
        StringTokenizer st2(st[i], "\n", StringTokenizer::TOKEN_IGNORE_EMPTY);
        for (size_t j = 1; j < st2.getNumTokens(); ++j)
        {
            StringTokenizer st3(st2[j], "=", StringTokenizer::TOKEN_IGNORE_EMPTY);
            string sVal = st3[1].substr(0, st3[1].length() - 1);
            if (!sVal.empty())
            {
                ans.push_back(make_pair(st3[0], sVal));
            }
        }
    }             
}

void StandardDocumentProcessorTestCase::testProcess()
{
    string sPath = writeTestFile("file1.txt", TEST_FILE1);

    StandardDocumentProcessor processor;
    processor.init(m_pDocSchema.get());
    
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

void StandardDocumentProcessorTestCase::testProcessMultiFile()
{
    string sPath = writeTestFile("file2.txt", TEST_FILE2);

    StandardDocumentProcessor processor;
    processor.init(m_pDocSchema.get());
    
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

void StandardDocumentProcessorTestCase::testProcessFileMisField()
{
    string sPath = writeTestFile("file3.txt", TEST_FILE_MISS_FIELD);

    StandardDocumentProcessor processor;
    processor.init(m_pDocSchema.get());
    
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

void StandardDocumentProcessorTestCase::testProcessWithEmptyField()
{
    String sPath = writeTestFile("file_with_empty_field.txt", TEST_FILE_WITH_EMPTY_FIELD);

    StandardDocumentProcessor processor;
    processor.init(m_pDocSchema.get());
    
    DocumentSource docSource(m_pDocSchema.get());
    RawDocumentPtr pRawDoc = new RawDocument();
    pRawDoc->setPath(sPath);
    docSource.setRawDocument(pRawDoc);

    processor.process(docSource);

    DocumentPtr pDoc = docSource.stealLastDocument();
    CPPUNIT_ASSERT(pDoc.isNotNull());

    Answer ans;
    makeAnswer(TEST_FILE_WITH_EMPTY_FIELD, ans);

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

string StandardDocumentProcessorTestCase::writeTestFile(const string& sFileName, 
                                                      const char* szFileContent)
{
    std::string sPath = getTestPath();
    sPath += "/" + sFileName;
    
    BinaryFile bf;
    bf.open(sPath, BinaryFile::CREATE);
    bf.write(szFileContent, strlen(szFileContent));
    bf.close();

    return sPath;
}

FX_NS_END

