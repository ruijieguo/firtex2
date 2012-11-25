#include "index/ForwardIndexTestCase.h"
#include "firtex/index/Index.h"
#include "firtex/document/FieldDefinition.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/index/TypedForwardIndexIterator.h"
#include "firtex/index/forwardindex/PrimitiveTypeForwardIndexReaderMMapImpl.h"
#include "IndexBuildHelper.h"
#include "TestHelper.h"
#include <sstream>

using namespace std;

FX_NS_USE(utility);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, ForwardIndexTestCase);

CPPUNIT_TEST_SUITE_REGISTRATION(ForwardIndexTestCase);

ForwardIndexTestCase::ForwardIndexTestCase() 
{
}

ForwardIndexTestCase::~ForwardIndexTestCase() 
{
}

void ForwardIndexTestCase::setUp()
{
    File f(getIndexPath());
    if (f.exists())
    {
        f.remove(true);
    }
}

void ForwardIndexTestCase::tearDown()
{
    File f(getIndexPath());
    if (f.exists())
    {
        f.remove(true);
    }
}

void ForwardIndexTestCase::testWriteAndRead()
{
    const size_t NUM_DOCS = 100;
    vector<int32_t> v1;
    vector<int64_t> v2;
    vector<float> v3;

    float fVal = 0.5;
    stringstream ss;
    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        ss << i << "," << i * 9381 << "," << fVal * i << ";";
        v1.push_back(i);
        v2.push_back(i * 9381);
        v3.push_back(fVal * i);
    }

    GLOBAL_CONF().Build.buildThreadCount = 1;

    buildForwardIndex(ss.str());

    Index index;
    index.open(getIndexPath(), Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();
    
    ForwardIndexIteratorPtr fdIter = pReader->forwardIndexReader("Number1");
    CPPUNIT_ASSERT(fdIter != NULL);
    checkForwardIndex<int32_t>(fdIter, v1);

    fdIter = pReader->forwardIndexReader("Number2");
    CPPUNIT_ASSERT(fdIter != NULL);
    checkForwardIndex<int64_t>(fdIter, v2);

    fdIter = pReader->forwardIndexReader("Price");
    CPPUNIT_ASSERT(fdIter != NULL);
    checkForwardIndex<float>(fdIter, v3);
}

void ForwardIndexTestCase::testWriteAndReadWithMultiBarrels()
{
    const size_t NUM_BARRELS = 3;
    const size_t NUM_DOCS = 300;
    vector<int32_t> v1;
    vector<int64_t> v2;
    vector<float> v3;

    float fVal = 0.5;
    size_t idx = 0;
    for (size_t i = 0; i < NUM_BARRELS; ++i)
    {
        stringstream ss;
        for (size_t j = 0; j < NUM_DOCS; ++j)
        {
            ss << idx << "," << idx * 9381 << "," << fVal * idx << ";";
            v1.push_back(idx);
            v2.push_back(idx * 9381);
            v3.push_back(fVal * idx);
            ++idx;
        }
        buildForwardIndex(ss.str());
    }

    Index index;
    index.open(getIndexPath(), Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();
    
    ForwardIndexIteratorPtr fdIter = pReader->forwardIndexReader("Number1");
    CPPUNIT_ASSERT(fdIter != NULL);
    checkForwardIndex<int32_t>(fdIter, v1);

    fdIter = pReader->forwardIndexReader("Number2");
    CPPUNIT_ASSERT(fdIter != NULL);
    checkForwardIndex<int64_t>(fdIter, v2);

    fdIter = pReader->forwardIndexReader("Price");
    CPPUNIT_ASSERT(fdIter != NULL);
    checkForwardIndex<float>(fdIter, v3);
}

string ForwardIndexTestCase::getIndexPath()
{
    Path p(TestHelper::getTestDataPath());
    p.makeDirectory();
    p.pushDirectory(_T("/test_forwardindex"));
    return p.toString();
}

void ForwardIndexTestCase::buildForwardIndex(const std::string& sDocs)
{
    try
    {
        DocumentSchema schema;
        schema.addSortableField(_T("Number1"), FieldType::INT32, true);
        schema.addSortableField(_T("Number2"), FieldType::INT64, true);
        schema.addSortableField(_T("Price"), FieldType::FLOAT, true);

        IndexBuildHelper::buildIndex(getIndexPath(), schema, sDocs, true);
    }
    catch (const FirteXException& )
    {
        CPPUNIT_ASSERT(false);
    }
}

template<typename T>
void ForwardIndexTestCase::checkForwardIndex(
        const ForwardIndexIteratorPtr& pIter, const vector<T>& answer)
{
    FX_NS(common)::SharedPtr<TypedForwardIndexIterator<T> > pTypedIter 
        = pIter.cast<TypedForwardIndexIterator<T> >();
    CPPUNIT_ASSERT(!pTypedIter.isNull());
    
    CPPUNIT_ASSERT_EQUAL(answer.size(), pIter->size());
    for (size_t i = 0; i< answer.size(); ++i)
    {
        T val;
        CPPUNIT_ASSERT(pTypedIter->seek((docid_t)i, val) == true);
        CPPUNIT_ASSERT_EQUAL(answer[i], val);
    }
}

FX_NS_END

