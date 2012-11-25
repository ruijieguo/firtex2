#ifndef __FX_DOCPOSTINGMERGERTESTCASE_H
#define __FX_DOCPOSTINGMERGERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/index/FieldMerger.h"
#include "firtex/utility/Pool.h"
#include "firtex/index/ByteSliceOutputStream.h"
#include <vector>

FX_NS_DEF(index);

struct PostingData;
DEFINE_TYPED_PTR(PostingData);

class DocPostingMergerTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(DocPostingMergerTestCase);
    CPPUNIT_TEST(testMerge);
    CPPUNIT_TEST(testMergeWithDeletion);
    CPPUNIT_TEST_SUITE_END();
public:
    DocPostingMergerTestCase();
    ~DocPostingMergerTestCase();

    void setUp();
    void tearDown();

protected:
    void testMerge();
    void testMergeWithDeletion();

protected:
    void checkMergeResult(offset_t nStartOff);

protected:
    MergingTermVector m_mergingTerms;
    std::vector<MergingTermPtr> m_termsPtr;
    size_t m_nNumMergeTerms;

    BarrelsInfoPtr m_pBarrelsInfo;

    std::vector<PostingDataPtr> m_postingHolder;

    FX_NS(utility)::PoolPtr m_pPool;
    ByteSliceOutputStreamPtr m_pMergedStream;
    bool m_bHasDeletion;
private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DOCPOSTINGMERGERTESTCASE_H
