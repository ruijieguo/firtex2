#include "NumberIndexTestCase.h"
#include "firtex/index/Index.h"
#include "firtex/index/TermReader.h"
#include "firtex/index/TermIterator.h"
#include "firtex/search/IndexSearcher.h"
#include "TestHelper.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/File.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/index/TypedTerm.h"
#include <sstream>

using namespace std;
using namespace firtex;

FX_NS_USE(index);
FX_NS_USE(search);
FX_NS_USE(document);
FX_NS_USE(utility);
FX_NS_USE(utility);

FX_NS_DEF(index);

CPPUNIT_TEST_SUITE_REGISTRATION(NumberIndexTestCase);

NumberIndexTestCase::NumberIndexTestCase(void)
{
}

NumberIndexTestCase::~NumberIndexTestCase(void)
{
}

void NumberIndexTestCase::setUp()
{
}

void NumberIndexTestCase::tearDown()
{
    File f(getIndexPath());
    f.remove(true);
}

void NumberIndexTestCase::testCTF()
{
    buildNumberIndex("1,2;3,4;5,6;7,8;9,10;1,2;3,4;5,6;7,8;9,10");

	Index index(getIndexPath().c_str(), Index::READ, NULL);
	IndexReaderPtr pReader = index.acquireReader();
	CPPUNIT_ASSERT(pReader != NULL);
	TermReaderPtr pTermReader = pReader->termReader();
	CPPUNIT_ASSERT(pTermReader);
	TermIteratorPtr pTermIterator = pTermReader->termIterator(_T("Number1"));
	CPPUNIT_ASSERT(pTermIterator != NULL);
	TermPostingIterator* pTermDocFreqs;
	while(pTermIterator->next())
	{
		pTermDocFreqs = pTermIterator->getDocFreqs();
		CPPUNIT_ASSERT_EQUAL((df_t)2, pTermDocFreqs->getDocFreq());
	}
}

void NumberIndexTestCase::testPosting()
{
    const size_t NUM_DOCS = 100;
    std::set<int32_t> ans;
    stringstream ss;
    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        ss << i << "," << i % 10 << ";";
        ans.insert((int32_t)i);
    }
    buildNumberIndex(ss.str());

    Index index(getIndexPath(), Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();
    TermReaderPtr pTermReader = pReader->termReader();
    TermIteratorPtr pIterator = pTermReader->termIterator(_T("Number1"));
    CPPUNIT_ASSERT(pIterator != NULL);
        	    
    while(pIterator->next())
    {
        const Term* pTerm = pIterator->term();
        const TypedTerm<int32_t>* pTermX = pTerm->cast<int32_t>();
        int32_t nCurTerm = pTermX->getValue();
        std::set<int32_t>::const_iterator it = ans.find(nCurTerm);
        CPPUNIT_ASSERT(it != ans.end());

        TermPostingIterator* pPostingIter = pIterator->getDocFreqs();
        CPPUNIT_ASSERT(pPostingIter != NULL);
        docid_t docId = 0;
        while (pPostingIter->nextDoc())
        {
            CPPUNIT_ASSERT_EQUAL(docId, pPostingIter->doc());
        }
    }	
}

void NumberIndexTestCase::testRangeQuery()
{
    const size_t NUM_DOCS = 100;
    size_t min = 8;
    stringstream ss;
    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        ss << min + i << "," << i * 10000 << ";";
    }
    buildNumberIndex(ss.str());

    Index index(getIndexPath().c_str(), Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();

    // test case for [13 TO 18]
    {   
        IndexSearcher se(pReader);
        QueryHits* pHits = se.search("[13 TO 18]", "Number1", 100);
        CPPUNIT_ASSERT(pHits != NULL);
        df_t numHits =  pHits->getHitNum();
        CPPUNIT_ASSERT_EQUAL((df_t)6, numHits);

        QueryHits::Iterator it = pHits->iterator();
    
        df_t n = 0;
        while (n < numHits)
        {
            CPPUNIT_ASSERT(it.hasNext());
            QueryHits::HitDoc* hitDoc = it.next();
            CPPUNIT_ASSERT(hitDoc != NULL);
            CPPUNIT_ASSERT_EQUAL((docid_t)5 + n, hitDoc->getDocID());
            n++;
        }

        delete pHits;    
    }

    // test case for [5 TO 20]
    {   
        IndexSearcher se(pReader);
        QueryHits* pHits = se.search("[5 TO 20]", "Number1", 100);
        CPPUNIT_ASSERT(pHits != NULL);
        df_t numHits =  pHits->getHitNum();
        CPPUNIT_ASSERT_EQUAL((df_t)13, numHits);

        QueryHits::Iterator it = pHits->iterator();
    
        df_t n = 0;
        while (n < numHits)
        {
            CPPUNIT_ASSERT(it.hasNext());
            QueryHits::HitDoc* hitDoc = it.next();
            CPPUNIT_ASSERT(hitDoc != NULL);
            CPPUNIT_ASSERT_EQUAL((docid_t)n, hitDoc->getDocID());
            n++;
        }

        delete pHits;    
    }

    // test case for (10 TO 15)
    {   
        IndexSearcher se(pReader);
        QueryHits* pHits = se.search("{10 TO 15}", "Number1", 100);
        CPPUNIT_ASSERT(pHits != NULL);
        df_t numHits =  pHits->getHitNum();
        CPPUNIT_ASSERT_EQUAL((df_t)4, numHits);

        QueryHits::Iterator it = pHits->iterator();
    
        df_t n = 0;
        while (n < numHits)
        {
            CPPUNIT_ASSERT(it.hasNext());
            QueryHits::HitDoc* hitDoc = it.next();
            CPPUNIT_ASSERT(hitDoc != NULL);
            CPPUNIT_ASSERT_EQUAL((docid_t)3 + n, hitDoc->getDocID());
            n++;
        }

        delete pHits;    
    }

    // test case for [ TO 15]
    {   
        IndexSearcher se(pReader);
        QueryHits* pHits = se.search("[TO 15]", "Number1", 100);
        CPPUNIT_ASSERT(pHits != NULL);
        df_t numHits =  pHits->getHitNum();
        CPPUNIT_ASSERT_EQUAL((df_t)8, numHits);

        QueryHits::Iterator it = pHits->iterator();
    
        df_t n = 0;
        while (n < numHits)
        {
            CPPUNIT_ASSERT(it.hasNext());
            QueryHits::HitDoc* hitDoc = it.next();
            CPPUNIT_ASSERT(hitDoc != NULL);
            CPPUNIT_ASSERT_EQUAL((docid_t)n, hitDoc->getDocID());
            n++;
        }

        delete pHits;    
    }

    // test case for [98 TO]
    {   
        IndexSearcher se(pReader);
        QueryHits* pHits = se.search("[98 TO]", "Number1", 100);
        CPPUNIT_ASSERT(pHits != NULL);
        df_t numHits =  pHits->getHitNum();
        CPPUNIT_ASSERT_EQUAL((df_t)10, numHits);

        QueryHits::Iterator it = pHits->iterator();
    
        df_t n = 0;
        while (n < numHits)
        {
            CPPUNIT_ASSERT(it.hasNext());
            QueryHits::HitDoc* hitDoc = it.next();
            CPPUNIT_ASSERT(hitDoc != NULL);
            CPPUNIT_ASSERT_EQUAL((docid_t)90 + n, hitDoc->getDocID());
            n++;
        }

        delete pHits;    
    }

    // test case for [20000 TO 50000] (int64)
    {   
        IndexSearcher se(pReader);
        QueryHits* pHits = se.search("[20000 TO 50000]", "Number2", 100);
        CPPUNIT_ASSERT(pHits != NULL);
        df_t numHits =  pHits->getHitNum();
        CPPUNIT_ASSERT_EQUAL((df_t)4, numHits);

        QueryHits::Iterator it = pHits->iterator();
    
        df_t n = 0;
        while (n < numHits)
        {
            CPPUNIT_ASSERT(it.hasNext());
            QueryHits::HitDoc* hitDoc = it.next();
            CPPUNIT_ASSERT(hitDoc != NULL);
            CPPUNIT_ASSERT_EQUAL((docid_t)2 + n, hitDoc->getDocID());
            n++;
        }

        delete pHits;    
    }
}

string NumberIndexTestCase::getIndexPath()
{
    Path p(TestHelper::getTestDataPath());
    p.makeDirectory();
    p.pushDirectory(_T("/test_numberindex"));
    return p.toString();
}

void NumberIndexTestCase::buildNumberIndex(const string& sDocs)
{
    try
    {
        DocumentSchema schema;
        schema.addField(_T("Number1"), "INT32", StoreType::YES);
        schema.addField( _T("Number2"), "INT64", StoreType::YES);

        Index index(getIndexPath().c_str(), Index::WRITE, &schema);
        IndexWriterPtr pIndexWriter = index.acquireWriter();

        StringTokenizer st(sDocs, _T(";"), StringTokenizer::TOKEN_TRIM |
                           StringTokenizer::TOKEN_IGNORE_EMPTY);

        DocumentPool docPool(st.getNumTokens(), pIndexWriter);

        for (StringTokenizer::Iterator it = st.begin(); it != st.end(); ++it)
        {
            Document* pDoc = docPool.nextDoc();
            StringTokenizer st2(*it, _T(","), StringTokenizer::TOKEN_TRIM |
                                StringTokenizer::TOKEN_IGNORE_EMPTY);
            pDoc->addField(0, st2[0]);
            pDoc->addField(1, st2[1]);
        }
        docPool.commit();
        pIndexWriter->close();
    }
    catch (const FirteXException& )
    {
        CPPUNIT_ASSERT(false);
    }
}

FX_NS_END
