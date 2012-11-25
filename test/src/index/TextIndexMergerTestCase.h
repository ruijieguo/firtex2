#ifndef __FX_TEXTINDEXMERGERTESTCASE_H
#define __FX_TEXTINDEXMERGERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/index/text/TextIndexer.h"
#include "firtex/index/text/TextTermReader.h"
#include "firtex/index/text/TextIndexMerger.h"

FX_NS_DEF(index);

class TextIndexMergerTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TextIndexMergerTestCase);
    CPPUNIT_TEST(testMerge);
    CPPUNIT_TEST(testMergeWithDeletions);
    CPPUNIT_TEST_SUITE_END();
public:
    TextIndexMergerTestCase();
    ~TextIndexMergerTestCase();

    void setUp();
    void tearDown();

    typedef std::pair<docid_t, std::vector<loc_t> > DocPair;
    typedef std::vector<DocPair> DocVector;
    typedef std::map<uint64_t, DocVector> TermMap;

protected:
    void testMerge();
    void testMergeWithDeletions();    

protected:
    void makeData();
    void makeMergedAnswer(TermMap& mergedAnswer);
    void checkMergedResult(TermIteratorPtr& pTermIt, TermMap& answer);
    void printPositng(TermIteratorPtr& pTermIt);
    void printPositngAnswer(const TermMap& answer);

protected:
    struct MergeEntry
    {
        MergeEntry();
        ~MergeEntry();

        TextIndexerPtr pIndexer;
        TextTermReaderPtr pTermReader;
        FX_NS(document)::FieldSchemaPtr pFieldSchema;
        FX_NS(index)::BarrelInfoPtr pBarrelInfo;
        PostingPoolPtr pPool;
        DocIdRecyclingPtr pDocIdRemapper;        
        FX_NS(utility)::BitVectorPtr pDocFilter;

        TermMap answer;
        std::string sBarrel;
        size_t nDataSize;
        docid_t newBaseDocId;
        
        FX_NS(store)::FileSystemPtr pFileSystem;
        FX_NS(store)::InputStreamPoolPtr pStreamPool;

        void makeData();
    };

    DEFINE_TYPED_PTR(MergeEntry);

    typedef std::vector<MergeEntryPtr> MergeEntryVect;

    MergeEntryVect m_mergeEntries;
    size_t m_nNumEntries;
    bool m_bHasDeletions;    
    FX_NS(store)::FileSystemPtr m_pFileSystem;
    FX_NS(store)::InputStreamPoolPtr m_pStreamPool;
    TextIndexMergerPtr m_pTextMerger;
    FX_NS(document)::FieldSchema m_fieldSchema;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_TEXTINDEXMERGERTESTCASE_H
