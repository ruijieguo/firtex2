#include "../analyzer/DocumentScanner.h"
#include "IndexContentTestCase.h"
#include "TestHelper.h"
#include "firtex/utility/BinaryFile.h"
#include "firtex/index/Index.h"
#include "firtex/index/TypedTerm.h"
#include "firtex/document/DocumentSchema.h"
#include "firtex/utility/DirectoryIterator.h"
#include <memory>

using namespace std;

FX_NS_USE(utility);

FX_NS_USE(document);

FX_NS_DEF(index);

CPPUNIT_TEST_SUITE_REGISTRATION(IndexContentTestCase);

class DirecotryIndexer
{
public:
    DirecotryIndexer(const tstring& sIndexPath, IndexLevel il)
    {
        DocumentSchema schema;
        schema.addUnIndexedField(_T("PATH"));
        schema.addTextField(_T("BODY"));
        m_pIndex = new Index(sIndexPath, Index::WRITE, &schema);
    }

    ~DirecotryIndexer()
    {
        delete m_pIndex;
    }
public:
    void index(const tstring& sDir)
    {
        IndexWriterPtr pIndexWriter = m_pIndex->acquireWriter();

        DirectoryIterator di(sDir, false);
        while(di.hasNext())
        {
            const File& f = di.next();
            if(f.isFile())
            {
                BinaryFile bf;
                bf.open(f.getPath().c_str(), BinaryFile::READ);
                if(bf.isFileOpen())
                {
                    size_t nRead = (size_t)bf.getLength();
                    if (nRead > 0)
                    {
                        DocumentPtr pDoc = new Document(pIndexWriter->getDocSchema());
                        pDoc->addField(0, f.getPath().c_str());
                        char* buf = new char[nRead + 1];
                        bf.read(buf, nRead);
                        buf[nRead] = 0;
                        pDoc->addField(1, buf, nRead, false);
                        delete[] buf;
                        
                        pIndexWriter->addDocument(pDoc);
                    }
                }
            }
        }
        docPool.commit();
        pIndexWriter->close();
    }

protected:
    Index* m_pIndex;
};
    
IndexContentTestCase::IndexContentTestCase()
{
    m_testFilePath = TestHelper::getTestDataPath();
    m_testFilePath.makeDirectory();
    m_testFilePath.pushDirectory(_T("txt_files"));
    m_pDocScanner = new DocumentScanner(m_testFilePath.toString());
}

IndexContentTestCase::~IndexContentTestCase()
{
    delete m_pDocScanner;
}

void IndexContentTestCase::setUp()
{    
    Path indexPath = TestHelper::getTestDataPath();
    indexPath.makeDirectory();
    indexPath.pushDirectory(_T("test_wlindex"));
    DirecotryIndexer di(indexPath.toString(), WORD_LEVEL);
    di.index(m_testFilePath.toString());

    indexPath.popDirectory();
    indexPath.pushDirectory(_T("test_dlindex"));
    DirecotryIndexer di2(indexPath.toString(), DOCUMENT_LEVEL);
    di2.index(m_testFilePath.toString());

    indexPath.popDirectory();
    indexPath.pushDirectory(_T("test_dl0index"));
    DirecotryIndexer di3(indexPath.toString(), DOCUMENT_LEVEL_0);
    di3.index(m_testFilePath.toString());
}

void IndexContentTestCase::tearDown()
{
    Path indexPath = TestHelper::getTestDataPath();
    indexPath.makeDirectory();
    indexPath.pushDirectory(_T("test_wlindex"));    
    File indexFile(indexPath);
    indexFile.remove(true);

    indexPath.popDirectory();
    indexPath.pushDirectory(_T("test_dlindex"));    
    indexFile = indexPath;
    if (indexFile.exists())
    {
        indexFile.remove(true);
    }

    indexPath.popDirectory();
    indexPath.pushDirectory(_T("test_dl0index"));    
    indexFile = indexPath;
    if (indexFile.exists())
    {
        indexFile.remove(true);
    }
}

void IndexContentTestCase::testIndexContent_WL()
{
    Index* pIndex;
    IndexReaderPtr pReader;

    const Term* pTerm;
    TermIteratorPtr pTermIter;
    int	docCount = 0;
    int	termCount = 0;
    int	pos = -1;
    uint32_t indexTermId;
    string fileName;

    //Check posting list
    Path indexPath = TestHelper::getTestDataPath();
    indexPath.makeDirectory();
    indexPath.pushDirectory(_T("test_wlindex"));    
    pIndex = new Index(indexPath.toString().c_str(), Index::READ, NULL);
    auto_ptr<Index> indexPtr(pIndex);

    CPPUNIT_ASSERT(pIndex != NULL);

    pReader = pIndex->acquireReader();
    TermReaderPtr pTermReader = pReader->termReader();

    pTermIter = pTermReader->termIterator("BODY");

    StoredFieldsReaderPtr pDocReader = pReader->createStoredFieldsReader();
    //Iterator all terms
    while(pTermIter->next())
    {
        pTerm = pTermIter->term();

        CPPUNIT_ASSERT(pTermReader->seek(pTerm));
		
        indexTermId = (pTerm->cast<int32_t>())->getValue();
        TermPositionIteratorPtr pPositions = pTermReader->termPositions();
        docCount = 0;

        while(pPositions->nextDoc())
        {
            DocumentPtr pDoc = pDocReader->document(pPositions->doc());
            docCount++;

            fileName.assign(pDoc->getField("PATH")->getValue().c_str());

            TermList* pTermIdList = m_pDocScanner->getTermListOfFile(fileName);
            CPPUNIT_ASSERT(pTermIdList != NULL);

            pos = pPositions->nextPosition();
            termCount = 0;

            while(pos != -1)
            {
                termCount++;
                CPPUNIT_ASSERT(indexTermId == pTermIdList->getValue(pos));
                pos = pPositions->nextPosition();
            }
            CPPUNIT_ASSERT(termCount == pPositions->freq());
        }//end while nextDoc()
        CPPUNIT_ASSERT(docCount == pPositions->getDocFreq());
    }
    CPPUNIT_ASSERT_EQUAL((int64_t)m_pDocScanner->getTotalTermCount(), 
                         (int64_t)pReader->getNumTerms());
}

void IndexContentTestCase::testIndexContent_DL()
{
    Index* pIndex;
    IndexReaderPtr pReader;

    const Term* pTerm;
    TermIteratorPtr pTermIter;
    int	docCount = 0;
    int	termCount = 0;
    uint32_t i;
    uint32_t indexTermId;
    string fileName;

    //Check posting list
    Path indexPath = TestHelper::getTestDataPath();
    indexPath.makeDirectory();
    indexPath.pushDirectory(_T("test_dlindex"));    
    pIndex = new Index(indexPath.toString().c_str(), Index::READ, NULL);
    auto_ptr<Index> indexPtr(pIndex);
    pReader = pIndex->acquireReader();
    TermReaderPtr pTermReader = pReader->termReader();

    pTermIter = pTermReader->termIterator("BODY");

    StoredFieldsReaderPtr pDocReader = pReader->createStoredFieldsReader();
    //Iterator all terms
    while(pTermIter->next())
    {
        pTerm = pTermIter->term();
		
        CPPUNIT_ASSERT(pTermReader->seek(pTerm));
				
        indexTermId = (pTerm->cast<int32_t>())->getValue();
        docCount = 0;
        TermPostingIteratorPtr pTermDocFreqs = pTermReader->termPostings();
        while(pTermDocFreqs->nextDoc())
        {
            DocumentPtr pDoc = pDocReader->document(pTermDocFreqs->doc());
            docCount++;
            // 获取文件路径
            fileName.assign(pDoc->getField("PATH")->getValue().c_str());

            TermList* pTermIdList = m_pDocScanner->getTermListOfFile(fileName);
            CPPUNIT_ASSERT(pTermIdList != NULL);

            for(i = 0, termCount = 0; i < pTermIdList->getSize(); i++)
            {
                if(indexTermId == pTermIdList->getValue(i))
                {
                    termCount++;
                }
            }
			
            CPPUNIT_ASSERT_EQUAL((tf_t)termCount, pTermDocFreqs->freq());

        }//end while nextDoc()
        CPPUNIT_ASSERT_EQUAL((df_t)docCount, pTermDocFreqs->getDocFreq());
    }
    CPPUNIT_ASSERT(m_pDocScanner->getTotalTermCount() == pReader->getNumTerms());
}

void IndexContentTestCase::testIndexContent_DL0()
{
    Index* pIndex;
    IndexReaderPtr pReader;

    const Term* pTerm;
    TermIteratorPtr pTermIter;
    int	docCount = 0;
    uint32_t i;
    uint32_t indexTermId;
    string fileName;

    //Check posting list
    Path indexPath = TestHelper::getTestDataPath();
    indexPath.makeDirectory();
    indexPath.pushDirectory(_T("test_dl0index"));    
    pIndex = new Index(indexPath.toString().c_str(), Index::READ, NULL);
    auto_ptr<Index> indexPtr(pIndex);
    pReader = pIndex->acquireReader();
    TermReaderPtr pTermReader = pReader->termReader();

    pTermIter = pTermReader->termIterator("BODY");

    StoredFieldsReaderPtr pDocReader = pReader->createStoredFieldsReader();
    //Iterator all terms
    while(pTermIter->next())
    {
        pTerm = pTermIter->term();

        CPPUNIT_ASSERT(pTermReader->seek(pTerm));
		
        indexTermId = (pTerm->cast<int32_t>())->getValue();
        TermPostingIteratorPtr pTermDocFreqs = pTermReader->termPostings();
        docCount = 0;

        while(pTermDocFreqs->nextDoc())
        {
            DocumentPtr pDoc = pDocReader->document(pTermDocFreqs->doc());
            docCount++;

            // 获取文件路径
            fileName.assign(pDoc->getField("PATH")->getValue().c_str());

            TermList* pTermIdList = m_pDocScanner->getTermListOfFile(fileName);
            CPPUNIT_ASSERT(pTermIdList != NULL);//存在该文件
            for(i = 0; i < pTermIdList->getSize(); i++)
            {
                if(indexTermId == pTermIdList->getValue(i))
                {
                    break;
                }
            }
            CPPUNIT_ASSERT(i < pTermIdList->getSize());//源文档中是否存在该词
        }//end while nextDoc()
        CPPUNIT_ASSERT_EQUAL((df_t)docCount, pTermDocFreqs->getDocFreq());//测DF
    }
    CPPUNIT_ASSERT_EQUAL((int64_t)m_pDocScanner->getTotalTermCount(), 
                         pReader->getNumTerms());//CTF
}

FX_NS_END
