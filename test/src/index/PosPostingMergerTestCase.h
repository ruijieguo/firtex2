#ifndef __FX_POSPOSTINGMERGERTESTCASE_H
#define __FX_POSPOSTINGMERGERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/index/FieldMerger.h"
#include "firtex/index/PostingPool.h"
#include "firtex/index/ByteSliceOutputStream.h"
#include <vector>

FX_NS_DEF(index);

struct PosPostingData;
DEFINE_TYPED_PTR(PosPostingData);

class PosPostingMergerTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(PosPostingMergerTestCase);
    CPPUNIT_TEST(testMerge);
    CPPUNIT_TEST(testMergeWithDeletion);
    CPPUNIT_TEST_SUITE_END();
public:
    PosPostingMergerTestCase();
    ~PosPostingMergerTestCase();

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

    std::vector<PosPostingDataPtr> m_postingHolder;

    PostingPoolPtr m_pPool;
    ByteSliceOutputStreamPtr m_pMergedDocStream;
    ByteSliceOutputStreamPtr m_pMergedPosStream;
    bool m_bHasDeletion;
private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_POSPOSTINGMERGERTESTCASE_H
