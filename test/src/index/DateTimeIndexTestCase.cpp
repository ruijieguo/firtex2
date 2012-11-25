#include "DateTimeIndexTestCase.h"
#include "firtex/index/Index.h"
#include "firtex/index/TermReader.h"
#include "firtex/index/TermIterator.h"
#include "firtex/search/IndexSearcher.h"
#include "TestHelper.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/index/TypedTerm.h"
#include "firtex/analyzer/DateTimeAnalyzer.h"
#include "firtex/queryparser/QueryParser.h"
#include <sstream>

using namespace std;
using namespace firtex;

FX_NS_USE(index);
FX_NS_USE(search);
FX_NS_USE(document);
FX_NS_USE(utility);
FX_NS_USE(analyzer);
FX_NS_USE(queryparser);

FX_NS_DEF(index);

CPPUNIT_TEST_SUITE_REGISTRATION(DateTimeIndexTestCase);

DateTimeIndexTestCase::DateTimeIndexTestCase(void)
{
}

DateTimeIndexTestCase::~DateTimeIndexTestCase(void)
{
}

void DateTimeIndexTestCase::testCTF()
{
    const size_t NUM_DOCS = 100;
    stringstream ss;
    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        ss << 2009 << "-" << i % 12 + 1 << "-" << i % 27 + 1 << " " 
           << i % 24 << ":" << i % 60 << ":" << i % 60 << ";";
    }

    buildDateTimeIndex(ss.str());

    Index index;
    index.open(getIndexPath(), Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();
    CPPUNIT_ASSERT(pReader != NULL);
    TermReaderPtr pTermReader = pReader->termReader();
    CPPUNIT_ASSERT(pTermReader);
    TermIteratorPtr pTermIterator = pTermReader->termIterator("DateTime1");
    CPPUNIT_ASSERT(pTermIterator != NULL);

    df_t ttf = 0;
    while (pTermIterator->hasNext())
    {
        TermIterator::TermEntry entry = pTermIterator->next();
        const TermMeta& termMeta = entry.postingIterator->getTermMeta();
        CPPUNIT_ASSERT_EQUAL((df_t)1, termMeta.getDocFreq());
        CPPUNIT_ASSERT_EQUAL((ctf_t)1, termMeta.getCTF());
        ttf +=  termMeta.getDocFreq();
    }

    CPPUNIT_ASSERT_EQUAL((df_t)100, ttf);
}

struct TimeAndDoc
{
    int64_t nTime;
    docid_t docId;
};

bool timeLess(const TimeAndDoc& h1, const TimeAndDoc& h2)
{
    return h1.nTime < h2.nTime;
}

void DateTimeIndexTestCase::testPosting()
{
    const size_t NUM_DOCS = 100;
    vector<TimeAndDoc> timeVec;

    stringstream ss;
    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        int y = 2011;
        int m = (i % 12) + 1;
        int d = (i % 27) + 1;
        int h = i % 24;
        int min = i % 60;
        int s = i % 60;
        ss << y << "-" << m << "-" << d << " " 
           << h << ":" << min << ":" << s << ";";
        TimeAndDoc td;
        td.nTime = DateTimeAnalyzer::makeTime(y, m, d, h, min, s);
        td.docId = (docid_t)i;
        timeVec .push_back(td);
    }
    sort(timeVec.begin(), timeVec.end(), timeLess);

    GLOBAL_CONF().Build.buildThreadCount = 1;
    buildDateTimeIndex(ss.str());

    Index index;
    index.open(getIndexPath(), Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();
    TermReaderPtr pTermReader = pReader->termReader();
    TermIteratorPtr pIterator = pTermReader->termIterator("DateTime1");
    CPPUNIT_ASSERT(!pIterator.isNull());

    size_t j = 0;
    while (pIterator->hasNext())
    {
        TermIterator::TermEntry entry = pIterator->next();
        const Term* pTerm = entry.term;
        const Int64Term* pTermX = pTerm->cast<int64_t>();
        int64_t nCurTerm = pTermX->getValue();
        int64_t nExpTerm = timeVec[j].nTime;
        CPPUNIT_ASSERT_EQUAL(nExpTerm, nCurTerm);

        TermPostingIteratorPtr pPostingIter = entry.postingIterator;
        CPPUNIT_ASSERT(pPostingIter != NULL);
        docid_t docId = pPostingIter->skipTo(0);
        CPPUNIT_ASSERT_EQUAL(timeVec[j].docId, docId);
        docId = pPostingIter->skipTo(docId + 1);
        CPPUNIT_ASSERT_EQUAL((docid_t)INVALID_DOCID, docId);
        
        j++;
    }
}

void DateTimeIndexTestCase::testQuery()
{
    const size_t NUM_DOCS = 12;
    stringstream ss;
    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        ss << 2009 << "-" << i % 12 + 1 << "-" << i % 27 + 1 << " " 
           << i % 24 << ":" << i % 60 << ":" << i % 60 << ";";
        if (i % 10 == 0)
        {
            ss << 2009 << "-" << i % 12 + 1 << "-" << i % 27 + 1 << " " 
               << i % 24 << ":" << i % 60 << ":" << i % 60 << ";";
        }
    }

    buildDateTimeIndex(ss.str());

    Index index;
    index.open(getIndexPath(), Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();
    IndexSearcher se(pReader);

    QueryParser queryParser(pReader->getAnalyzerMapper(), 
                    "DateTime1", QueryParser::OP_AND);
    QueryHitsPtr pHits = se.search("query=\'2009-2-2 1:1:1\'", queryParser);
    CPPUNIT_ASSERT(pHits.isNotNull());
    uint64_t uTotalHits =  pHits->getTotalHits();
    CPPUNIT_ASSERT_EQUAL((uint64_t)1, uTotalHits);

    pHits = se.search("query=\'2009-11-11 10:10:10\'", queryParser);
    CPPUNIT_ASSERT(pHits.isNotNull());
    uTotalHits =  pHits->getTotalHits();
    CPPUNIT_ASSERT_EQUAL((uint64_t)2, uTotalHits);
}

void DateTimeIndexTestCase::testRangeQuery()
{
    const size_t NUM_DOCS = 12;
    stringstream ss;
    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        ss << 2009 << "-" << i % 12 + 1 << "-" << i % 27 + 1 << " " 
           << i % 24 << "/" << i % 60 << "/" << i % 60 << ";";
        /*  if (i % 10 == 0)
            {
            ss << 2009 << "-" << i % 12 + 1 << "-" << i % 27 + 1 << " " 
            << i % 24 << "/" << i % 60 << "/" << i % 60 << ";";
            }*/
    }

    buildDateTimeIndex(ss.str());

    Index index;
    index.open(getIndexPath(), Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();

    // test case for [13 TO 18]
    {   
        IndexSearcher se(pReader);
        // QueryHits* pHits = se.search("[2009-11-11 10/10/10 TO 2009-12-12 11/11/11]",
        //         "DateTime1", 100);
        // CPPUNIT_ASSERT(pHits != NULL);
        // df_t numHits =  pHits->getHitNum();
        // CPPUNIT_ASSERT_EQUAL((df_t)2, numHits);

        // QueryHits::Iterator it = pHits->iterator();
    
        // df_t n = 0;
        // while (n < numHits)
        // {
        //     CPPUNIT_ASSERT(it.hasNext());
        //     QueryHits::HitDoc* hitDoc = it.next();
        //     CPPUNIT_ASSERT(hitDoc != NULL);
        //     CPPUNIT_ASSERT_EQUAL((docid_t)10 + n, hitDoc->getDocID());
        //     n++;
        // }

        // delete pHits;    
    }
}

string DateTimeIndexTestCase::getIndexPath()
{
    Path p(TestHelper::getTestDataPath());
    p.makeDirectory();
    p.pushDirectory("test_DateTimeindex");
    return p.toString();
}

void DateTimeIndexTestCase::buildDateTimeIndex(const string& sDocs)
{
    try
    {
        DocumentSchema schema;
        schema.addField("DateTime1", "DATETIME_I", true);

        Index index;
        index.open(getIndexPath(), Index::WRITE, &schema);
        IndexWriterPtr pIndexWriter = index.acquireWriter();
        
        StringTokenizer st(sDocs, ";", StringTokenizer::TOKEN_TRIM |
                           StringTokenizer::TOKEN_IGNORE_EMPTY);
        for (StringTokenizer::Iterator it = st.begin(); it != st.end(); ++it)
        {
            DocumentPtr pDoc = new Document(pIndexWriter->getDocSchema());
            pDoc->addField(0, (*it).c_str());
            pIndexWriter->addDocument(pDoc);
        }

        pIndexWriter->close();
    }
    catch (const FirteXException& )
    {
        CPPUNIT_ASSERT(false);
    }
}

FX_NS_END
