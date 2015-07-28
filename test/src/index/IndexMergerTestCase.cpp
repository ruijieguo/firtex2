#include "index/IndexMergerTestCase.h"
#include "firtex/index/IndexMerger.h"
#include "firtex/index/Index.h"
#include "firtex/index/OptimizeMergePolicy.h"
#include "firtex/index/NoMergePolicy.h"
#include "firtex/index/forwardindex/PrimitiveTypeForwardIndexIterator.h"
#include "firtex/index/SyncCommitScheduler.h"
#include "firtex/store/FileSystem.h"
#include "firtex/store/FileSystemFactory.h"
#include "IndexBuildHelper.h"
#include "TestHelper.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/utility/Hash.h"
#include "firtex/analyzer/StandardAnalyzer.h"
#include "firtex/analyzer/KeywordAnalyzer.h"

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(store);
FX_NS_USE(document);
FX_NS_USE(analyzer);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, IndexMergerTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(IndexMergerTestCase);

IndexMergerTestCase::IndexMergerTestCase() 
{
}

IndexMergerTestCase::~IndexMergerTestCase() 
{
}

void IndexMergerTestCase::setUp()
{
    TestHelper::removeTestFile("test_index_merger", true);

    m_pDocSchema.assign(new DocumentSchema());

    m_pDocSchema->addField("DOCID", "PRIMARY_KEY", true);
    m_pDocSchema->addField("TITLE", "KEYWORD", true);
    m_pDocSchema->addTextField("BODY");
    m_pDocSchema->addField("MODIFIED", "INT32", true);
}

void IndexMergerTestCase::tearDown()
{
    TestHelper::removeTestFile("test_index_merger", true);
}

string makeData(const DocumentSchema& schema,
                size_t nDataSize, size_t startPk)
{
    string testWords[] = {"one", "two", "three", "four"};
    stringstream ss;
    for (size_t i = 0; i < nDataSize; ++i, ++startPk)
    {
        DocumentSchema::Iterator it = schema.iterator();
        while (it.hasNext())
        {
            const FieldSchema* pFieldSchema = it.next();
            if (pFieldSchema->getTypeName() == "PRIMARY_KEY")
            {
                ss << "pk_" << startPk << ", ";
            }
            else if (pFieldSchema->getTypeName() == "TEXT")
            {
                ss <<  testWords[(i + (size_t)pFieldSchema->getId()) % 4] 
                   << " hot, ";
            }
            else if (pFieldSchema->getTypeName() == "KEYWORD")
            {
                ss <<  testWords[(i + (size_t)pFieldSchema->getId()) % 4] 
                   << ", ";
            }
            else if (pFieldSchema->getTypeName() == "INT32")
            {
                ss << startPk << " ";
            }
        }
        ss << ";";
    }
    return ss.str();
}

void IndexMergerTestCase::mergeIndex()
{
    string str = getTestPath();
    Index index;
    index.open(str, Index::APPEND, NULL);
    IndexWriterPtr pIndexWriter = index.acquireWriter();

    MergePolicyPtr pMergePolicy(new OptimizeMergePolicy);
    pIndexWriter->setMergePolicy(pMergePolicy);
    pIndexWriter->mergeIndex();
}

void IndexMergerTestCase::buildIndex(
        const vector<size_t>& docCountOfBarrels,
        MockIndex& answer,
        const BitVectorPtr& pDocFilter)
{
    string str = getTestPath();

    Index index;
    index.open(str, Index::WRITE, m_pDocSchema.get());
    IndexWriterPtr pIndexWriter = index.acquireWriter();
        
    MergePolicyPtr pMergePolicy(new NoMergePolicy);
    pIndexWriter->setMergePolicy(pMergePolicy);
    
    CommitSchedulerPtr pScheduler(new SyncCommitScheduler());
    pIndexWriter->setCommitScheduler(pScheduler);

    string sResult;
    size_t builtDocCount = 0;
    for (size_t i = 0; i < docCountOfBarrels.size(); ++i)
    {
        if (i > 0)
        {
            pIndexWriter->commit();
        }
        size_t docCount = docCountOfBarrels[i];
        string data = makeData(*m_pDocSchema, docCount, builtDocCount);

        IndexBuildHelper::buildIndex(pIndexWriter, data);

        sResult += data;
        builtDocCount += docCount;
    }
    pIndexWriter->commit();

    if (pDocFilter)
    {
        size_t count = (builtDocCount > pDocFilter->length()) 
                       ? pDocFilter->length() : builtDocCount;
        for (size_t i = 0; i < count; ++i)
        {
            if (pDocFilter->test(i))
            {
                stringstream ss;
                ss << "pk_" << i;
                pIndexWriter->deleteDocument(ss.str());
            }
        }
    }

    index.close();
    
    makeAnswer(sResult, *m_pDocSchema, pDocFilter, answer);
}

void IndexMergerTestCase::testMerge()
{
    const size_t NUM_DOCS = 1000;
    vector<size_t> docCountOfBarrels;
    docCountOfBarrels.push_back(NUM_DOCS);
    docCountOfBarrels.push_back(NUM_DOCS);

    BitVectorPtr pDocFilter;
    MockIndex answer;
    try 
    {
        GLOBAL_CONF().Build.buildThreadCount = 1;
        buildIndex(docCountOfBarrels, answer, pDocFilter);

        mergeIndex();
    }
    catch (const FirteXException& e)
    {
        cout << "ERROR: " << e.what() << endl;
        FIRTEX_ASSERT2(false);
    }
    checkResult(answer, *m_pDocSchema);
}

void IndexMergerTestCase::testMergeWithDeletion()
{
    try
    {
        const size_t NUM_DOCS = 1000;
        vector<size_t> docCountOfBarrels;
        docCountOfBarrels.push_back(NUM_DOCS);
        docCountOfBarrels.push_back(NUM_DOCS);

        GLOBAL_CONF().Build.buildThreadCount = 1;

        BitVectorPtr pDocFilter = createDocFilter(NUM_DOCS * 2);
        MockIndex answer;
        buildIndex(docCountOfBarrels, answer, pDocFilter);

        mergeIndex();
        checkResult(answer, *m_pDocSchema);

    }
    catch(const FirteXException& e)
    {
        cout << "====" << e.what() << endl;
        CPPUNIT_ASSERT(false);
    }
}

void IndexMergerTestCase::testMergeExceedMaxAllowedOpenFiles()
{
    const size_t NUM_DOCS = 100;
    vector<size_t> docCountOfBarrels;
    docCountOfBarrels.push_back(NUM_DOCS);
    docCountOfBarrels.push_back(NUM_DOCS);
    docCountOfBarrels.push_back(NUM_DOCS);
    docCountOfBarrels.push_back(NUM_DOCS);

    int32_t buildThreadCount = GLOBAL_CONF().Build.buildThreadCount;
    GLOBAL_CONF().Build.buildThreadCount = 1;
    uint32_t nMaxAllowedOpenFiles = GLOBAL_CONF().Merge.maxAllowedOpenFiles;
    GLOBAL_CONF().Merge.maxAllowedOpenFiles = 16;

    BitVectorPtr pDocFilter;
    MockIndex answer;
    buildIndex(docCountOfBarrels, answer, pDocFilter);

    mergeIndex();
    checkResult(answer, *m_pDocSchema);
    GLOBAL_CONF().Merge.maxAllowedOpenFiles = nMaxAllowedOpenFiles;
    GLOBAL_CONF().Build.buildThreadCount = buildThreadCount;
}

void IndexMergerTestCase::makeAnswer(const string& sResult, 
                                     const DocumentSchema& docSchema,
                                     const BitVectorPtr& pDocFilter, 
                                     MockIndex& answer)
{
    StringTokenizer st(sResult, ";", StringTokenizer::TOKEN_TRIM | 
                           StringTokenizer::TOKEN_IGNORE_EMPTY);
    size_t nNumDocs = st.getNumTokens();
    
    StoredFields& storedFields = answer.second;
    storedFields.reserve(nNumDocs);

    docid_t docId = 0;
    for (size_t i = 0; i < nNumDocs; ++i)
    {
        if (!pDocFilter || !pDocFilter->test(i))
        {
            StringTokenizer st2(st[i], ",", StringTokenizer::TOKEN_TRIM | 
                    StringTokenizer::TOKEN_IGNORE_EMPTY);
            for (size_t j = 0; j < st2.getNumTokens(); ++j)
            {
                if (docSchema[j]->isIndexed())
                {
                    FieldIndex& fieldIndex = answer.first[docSchema[j]->getId()];
                    fieldIndex.lengthNorm.resize(nNumDocs);
                
                    makeInvertInexAnswer(docId, st2[j], 
                            fieldIndex.invIndex, fieldIndex.lengthNorm);
                }//if (docSchema[j]->isIndexed())

                if (docSchema[j]->hasForwardIndex())
                {
                    FieldIndex& fieldIndex = answer.first[docSchema[j]->getId()];
                    if ((size_t)docId >= fieldIndex.forwIndex.size())
                    {
                        fieldIndex.forwIndex.resize((size_t)docId + 1);
                    }
                    
                    int32_t value = NumberParser::parseInt32(st2[j]);
                    fieldIndex.forwIndex[docId] = value;
                }

                if (docSchema[j]->isStored())
                {
                    if ((size_t)docId >= storedFields.size())
                    {
                        storedFields.resize((size_t)docId + 1);
                    }
                    storedFields[docId].insert(make_pair(
                                    docSchema[j]->getId(), st2[j]));
                }
            }
            ++docId;
        }//if (!pDocFilter || !pDocFilter->test(i))
    }
}

void IndexMergerTestCase::makeInvertInexAnswer(docid_t docId,
        const string& sFields, InvertedIndex& invIndex,
        LengthNorm& lengthNorm)
{
    StringTokenizer st(sFields, " ",  StringTokenizer::TOKEN_TRIM | 
                        StringTokenizer::TOKEN_IGNORE_EMPTY);
    lengthNorm[docId] = (int32_t)st.getNumTokens();
    for (size_t k = 0; k < st.getNumTokens(); ++k)
    {
        InvertedIndex::iterator aIt = invIndex.find(st[k]);
        if (aIt != invIndex.end())
        {
            DocVector& docVect = aIt->second;
            if (docVect.back().first != docId)
            {
                vector<loc_t> pos;
                pos.push_back(k);
                docVect.push_back(make_pair(docId, pos));
            }
            else
            {
                DocPair& docPair = docVect.back();
                docPair.second.push_back(k);
            }
        }
        else 
        {
            DocVector docVect;
            vector<loc_t> pos;
            pos.push_back(k);
            docVect.push_back(make_pair(docId, pos));
            invIndex.insert(make_pair(st[k], docVect));
        }
    }
}

void IndexMergerTestCase::checkResult(MockIndex& answer,
                                      DocumentSchema& schema)
{
    string str = getTestPath();
    Index index;
    index.open(str, Index::READ, NULL);
    IndexReaderPtr pIndexReader = index.acquireReader();
    CPPUNIT_ASSERT(pIndexReader != NULL);

    DocumentSchema::Iterator schemaIt = schema.iterator();
    while (schemaIt.hasNext())
    {
        const FieldSchema* pFieldSchema = schemaIt.next();
        if (pFieldSchema->isIndexed())
        {
            const FieldIndex& fieldIndex = answer.first[pFieldSchema->getId()];
            const InvertedIndex& invIndex = fieldIndex.invIndex;
            
            TermReaderPtr pTermReader = pIndexReader->termReader();
            CPPUNIT_ASSERT(pTermReader);    

            for (InvertedIndex::const_iterator aIt = invIndex.begin();
                 aIt != invIndex.end(); ++aIt)
            {
                TermPtr pTerm;
                if (pFieldSchema->getTypeName() == "TEXT")
                {
                    StandardAnalyzer sa;
                    sa.init();
                    TokenViewPtr pTokens = sa.tokenize(aIt->first);
                    TokenView::Iterator it = pTokens->iterator();
                    pTerm.assign(new Term(pFieldSchema->getName(),
                                    it.next().getTextValue()));
                }
                else if (pFieldSchema->getTypeName() == "KEYWORD")
                {
                    KeywordAnalyzer sa;
                    TokenViewPtr pTokens = sa.tokenize(aIt->first);
                    TokenView::Iterator it = pTokens->iterator();
                    pTerm.assign(new Term(pFieldSchema->getName(), 
                                    it.next().getTextValue()));
                }
                else 
                {
                    pTerm.assign(new Term(pFieldSchema->getName(), aIt->first));
                }
                
                TermPostingIteratorPtr pPost = pTermReader->seek(pTerm.get());
                assert(pPost);
                CPPUNIT_ASSERT(pPost);
                const TermMeta& termMeta = pPost->getTermMeta();
                CPPUNIT_ASSERT_EQUAL((df_t)aIt->second.size(), termMeta.getDocFreq());
            }

            if (pFieldSchema->hasLengthNorm())
            {
                LengthNormIteratorPtr lengthNormIt =
                    pIndexReader->lengthNorm(pFieldSchema->getName());
                CPPUNIT_ASSERT(lengthNormIt != NULL);

                checkLengthNorm(fieldIndex.lengthNorm, lengthNormIt);
            }
        }//if (pFieldSchema->isIndexed())

        if (pFieldSchema->hasForwardIndex())
        {
            ForwardIndexIteratorPtr pFdIter =
                pIndexReader->forwardIndexReader(pFieldSchema->getName());
            CPPUNIT_ASSERT(pFdIter != NULL);

            const FieldIndex& fieldIndex = answer.first[pFieldSchema->getId()];
            checkForwardIndex(fieldIndex.forwIndex, pFdIter);
        }

        if (pFieldSchema->isStored())
        {
            const StoredFields& storedFields = answer.second;
            checkStoredFields(storedFields, pIndexReader);
        }
    }
}

void IndexMergerTestCase::checkLengthNorm(const LengthNorm& lengthNorm,
        LengthNormIteratorPtr& lengthNormIt)
{
    int32_t value = 0;
    for (size_t i = 0; i < lengthNorm.size(); ++i)
    {
        CPPUNIT_ASSERT(lengthNormIt->seek((docid_t)i, value));
        CPPUNIT_ASSERT_EQUAL((int32_t)lengthNorm[i], value);
    }
}

void IndexMergerTestCase::checkForwardIndex(const ForwardIndex& forwIndex,
        const ForwardIndexIteratorPtr& pFdIter)
{
    Int32ForwardIndexIteratorPtr pInt32It =
        pFdIter.cast<Int32ForwardIndexIterator>();

    int32_t value = 0;
    for (size_t i = 0; i < forwIndex.size(); ++i)
    {
        CPPUNIT_ASSERT(pInt32It->seek((docid_t)i, value));
        CPPUNIT_ASSERT_EQUAL((int32_t)forwIndex[i], value);
    }
}

void IndexMergerTestCase::checkStoredFields(const StoredFields& storedFields,
        const IndexReaderPtr& pIndexReader)
{
    StoredFieldsReaderPtr pStoredReader =
        pIndexReader->createStoredFieldsReader();
    CPPUNIT_ASSERT(pStoredReader != NULL);

    docid_t docId = 0;
    for (size_t i = 0; i < storedFields.size(); ++i, ++docId)
    {
        FieldSelector selector(pIndexReader->getDocSchema(), true, false);
        ResultDoc resultDoc(i);
        bool ret = pStoredReader->getDocument(selector, resultDoc);
        CPPUNIT_ASSERT(ret);
        if (resultDoc.size() == 0)
        {
            int a = resultDoc.size();
            cout << a << endl;
        }
        CPPUNIT_ASSERT(resultDoc.size() > 0);

        const FieldMap& fields = storedFields[i];

        ResultDoc::Iterator fieldIt = resultDoc.iterator();
        for (FieldMap::const_iterator it = fields.begin(); 
             it != fields.end(); ++it)
        {
            CPPUNIT_ASSERT(fieldIt.hasNext());
            const ResultDoc::Field& field = fieldIt.next();
            CPPUNIT_ASSERT_EQUAL(it->second, field.second);
        }
    }
}

string IndexMergerTestCase::getTestPath()
{
    return TestHelper::getTestDataPath("test_index_merger", true);
}

BitVectorPtr IndexMergerTestCase::createDocFilter(size_t nDocCount)
{
    BitVectorPtr pDocFilter(new BitVector());
    for (size_t i = 0; i < nDocCount; ++i)
    {
        if ((i % 32) == 0)
        {
            pDocFilter->set(i);
        }
        else if ((i % 121) < 5)
        {
            pDocFilter->set(i);
        }
    }
    return pDocFilter;
}

FX_NS_END

