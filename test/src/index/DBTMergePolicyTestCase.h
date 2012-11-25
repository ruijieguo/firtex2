#ifndef __FX_DBTMERGEPOLICYTESTCASE_H
#define __FX_DBTMERGEPOLICYTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/DBTMergePolicy.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(index);

class DBTMergePolicyTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(DBTMergePolicyTestCase);
    CPPUNIT_TEST(testCreateProposals);
    CPPUNIT_TEST(testCreateProposalsWithMergePartial);
    CPPUNIT_TEST(testCreateProposalsWithCrossLevel);
    CPPUNIT_TEST(testCreateProposalsWithTwoProposal);
    CPPUNIT_TEST_SUITE_END();
public:
    DBTMergePolicyTestCase();
    ~DBTMergePolicyTestCase();

    void setUp();
    void tearDown();

private:
    BarrelsInfoPtr createBarrelsInfo(const std::string& sBarrel);
    void makeAnswer(MergeProposalPtr& pProposal, const std::string& sBarrel);
    void checkProposals(const MergeProposalPtr& pExpProposal,
                        const MergeProposalPtr& pActProposal);

protected:
    void testCreateProposals();
    void testCreateProposalsWithMergePartial();
    void testCreateProposalsWithCrossLevel();
    void testCreateProposalsWithTwoProposal();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DBTMERGEPOLICYTESTCASE_H
