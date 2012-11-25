#ifndef __FX_INDEXMERGERTESTCASE_H
#define __FX_INDEXMERGERTESTCASE_H

#include <map>
#include <vector>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/index/IndexReader.h"
#include "firtex/index/ForwardIndexReader.h"
#include "firtex/index/LengthNormIterator.h"
#include "firtex/utility/BitVector.h"

FX_NS_DEF(index);

class IndexMergerTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(IndexMergerTestCase);
    CPPUNIT_TEST(testMerge);
    CPPUNIT_TEST(testMergeWithDeletion);
    CPPUNIT_TEST(testMergeExceedMaxAllowedOpenFiles);
    CPPUNIT_TEST_SUITE_END();

    typedef std::pair<docid_t, std::vector<loc_t> > DocPair;
    typedef std::vector<DocPair> DocVector;
    typedef std::map<std::string, DocVector> InvertedIndex;

    typedef std::vector<int32_t> ForwardIndex;

    typedef std::vector<norm_t> LengthNorm;

    struct FieldIndex
    {
        InvertedIndex invIndex;
        ForwardIndex forwIndex;
        LengthNorm lengthNorm;
    };

    typedef std::map<fieldid_t, FieldIndex> IndexMap;

    typedef std::map<fieldid_t, std::string> FieldMap;
    typedef std::vector<FieldMap> StoredFields;

    typedef std::pair<IndexMap, StoredFields> MockIndex;

public:
    IndexMergerTestCase();
    ~IndexMergerTestCase();

    void setUp();
    void tearDown();

protected:
    void testMerge();
    void testMergeWithDeletion();
    void testMergeExceedMaxAllowedOpenFiles();

protected:
    std::string getTestPath();
    void buildIndex(const std::vector<size_t>& docCountOfBarrels,
                    MockIndex& answer,
                    const FX_NS(utility)::BitVectorPtr& pDocFilter);
    void mergeIndex();
    void makeAnswer(const std::string& sResult, 
                    const FX_NS(document)::DocumentSchema& docSchema,
                    const FX_NS(utility)::BitVectorPtr& pDocFilter,
                    MockIndex& answer);
    void makeInvertInexAnswer(docid_t docId,
                              const string& sFields,
                              InvertedIndex& mockIndex,
                              LengthNorm& lengthNorm);

    void checkResult(MockIndex& answer,
                     FX_NS(document)::DocumentSchema& schema);
    void checkLengthNorm(const LengthNorm& lengthNorm,
                         LengthNormIteratorPtr& lengthNormIt);
    void checkForwardIndex(const ForwardIndex& forwIndex,
                           const ForwardIndexIteratorPtr& pFdIter);
    void checkStoredFields(const StoredFields& storedFields,
                           const IndexReaderPtr& pIndexReader);

    FX_NS(utility)::BitVectorPtr createDocFilter(size_t nDocCount);

private:
    FX_NS(document)::DocumentSchemaPtr m_pDocSchema;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_INDEXMERGERTESTCASE_H
