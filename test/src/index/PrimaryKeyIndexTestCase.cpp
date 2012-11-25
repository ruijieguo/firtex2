#include "PrimaryKeyIndexTestCase.h"
#include "firtex/index/Index.h"
#include "firtex/index/TermReader.h"
#include "firtex/index/TermIterator.h"
#include "firtex/search/IndexSearcher.h"
#include "TestHelper.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/HashMap.h"
#include "firtex/utility/Hash.h"
#include "firtex/utility/StringTokenizer.h"
#include "IndexBuildHelper.h"
#include "firtex/index/TypedTerm.h"
#include <sstream>
#include <limits>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;
using namespace firtex;

FX_NS_USE(index);
FX_NS_USE(search);
FX_NS_USE(document);
FX_NS_USE(utility);
FX_NS_USE(utility);

FX_NS_DEF(index);

CPPUNIT_TEST_SUITE_REGISTRATION(PrimaryKeyIndexTestCase);

PrimaryKeyIndexTestCase::PrimaryKeyIndexTestCase(void)
{
}

PrimaryKeyIndexTestCase::~PrimaryKeyIndexTestCase(void)
{
}

void PrimaryKeyIndexTestCase::tearDown()
{
    File f(getIndexPath());
    if (f.exists())
    {
        f.remove(true);
    }
}

void PrimaryKeyIndexTestCase::testBuildAndSearch()
{
    const size_t NUM_DOCS = 1000;

    map<docid_t, uint64_t> terms;

    stringstream ss;
    size_t i;
    for (i = 0; i < NUM_DOCS; ++i)
    {
        stringstream ss1;
        ss1 << "abc" << i;
        ss << ss1.str() << ", " << "abc" << i % 12 << ";";

        terms.insert(make_pair((docid_t)i, Hash::hashString64(ss1.str().c_str())));
    }
    GLOBAL_CONF().Build.buildThreadCount = 1;

    buildIndex(ss.str(), false);

    Index index;
    index.open(getIndexPath(), Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();
    TermReaderPtr pTermReader = pReader->termReader();
    TermIteratorPtr pIterator = pTermReader->termIterator(_T("PrimaryKey1"));
    CPPUNIT_ASSERT(pIterator != NULL);

    size_t j = 0;

    while (pIterator->hasNext())
    {
        TermIterator::TermEntry entry = pIterator->next();
        // const Term* pTerm = entry.term;
        // const UInt64Term* pTermX = pTerm->cast<uint64_t>();
        // uint64_t nCurTerm = pTermX->getValue();

        TermPostingIteratorPtr pPostingIter = entry.postingIterator;
        CPPUNIT_ASSERT(pPostingIter != NULL);
        docid_t docId = pPostingIter->skipTo(0);
        map<docid_t, uint64_t>::const_iterator it = terms.find(docId);
        CPPUNIT_ASSERT(it != terms.end());
//        CPPUNIT_ASSERT_EQUAL(it->second, nCurTerm);
        CPPUNIT_ASSERT_EQUAL((tf_t)1, pPostingIter->freq());

        j++;
    } 
    
    CPPUNIT_ASSERT_EQUAL(terms.size(), j);
}

void PrimaryKeyIndexTestCase::testSearchInMem()
{
    const size_t NUM_DOCS = 1000;

    stringstream ss;
    size_t i;
    for (i = 0; i < NUM_DOCS - 10; ++i)
    {
        stringstream ss1;
        ss1 << "abc" << i;
        ss << ss1.str() << ", " << "abc" << i % 12 << ";";
    }
    buildIndex(ss.str(), false);

    Index index;
    index.open(getIndexPath(), Index::APPEND, NULL);
    IndexWriterPtr pIndexWriter = index.acquireWriter();

    stringstream ss2;
    for (; i < NUM_DOCS; ++i)
    {
        stringstream ss1;
        ss1 << "abc" << i;
        ss2 << ss1.str() << ", " << "abc" << i % 12 << ";";
    }
    buildIndex(ss2.str(), pIndexWriter);

    pIndexWriter->commit();

    for (i = 0; i < NUM_DOCS; ++i)
    {
        stringstream ss1;
        ss1 << "abc" << i;
        CPPUNIT_ASSERT(pIndexWriter->hasDocument(ss1.str()));
    }

    stringstream ss1;
    ss1 << "abc" << i;
    CPPUNIT_ASSERT(!pIndexWriter->hasDocument(ss1.str()));
}

string PrimaryKeyIndexTestCase::getIndexPath()
{
    Path p(TestHelper::getTestDataPath());
    p.makeDirectory();
    p.pushDirectory(_T("/test_primarykey_index"));
    return p.toString();
}

void PrimaryKeyIndexTestCase::buildIndex(const string& sDocs, bool bInc)
{
    try
    {
        DocumentSchema schema;
        schema.addField(_T("PrimaryKey1"), "PRIMARY_KEY", true);
        schema.addField( _T("BODY"), "TEXT", true);

        IndexBuildHelper::buildIndex(getIndexPath(), schema, sDocs, bInc);
    }
    catch (const FirteXException& )
    {
        CPPUNIT_ASSERT(false);
    }
}

void PrimaryKeyIndexTestCase::buildIndex(const string& sDocs, IndexWriterPtr& pIndexWriter)
{
    try
    {
        StringTokenizer st(sDocs, _T(";"), StringTokenizer::TOKEN_TRIM |
                           StringTokenizer::TOKEN_IGNORE_EMPTY);
        for (StringTokenizer::Iterator it = st.begin(); it != st.end(); ++it)
        {
            DocumentPtr pDoc = new Document(pIndexWriter->getDocSchema());

            StringTokenizer st2(*it, _T(","), StringTokenizer::TOKEN_TRIM |
                                StringTokenizer::TOKEN_IGNORE_EMPTY);
            CPPUNIT_ASSERT_EQUAL((size_t)2, st2.getNumTokens());
            pDoc->addField(0, st2[0].c_str());
            pDoc->addField(1, st2[1].c_str());
        }
    }
    catch (const FirteXException& )
    {
        CPPUNIT_ASSERT(false);
    }
}

FX_NS_END
