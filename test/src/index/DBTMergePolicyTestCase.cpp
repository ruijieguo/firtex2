#include "index/DBTMergePolicyTestCase.h"
#include "firtex/utility/StringTokenizer.h"

using namespace std;
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, DBTMergePolicyTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(DBTMergePolicyTestCase);

DBTMergePolicyTestCase::DBTMergePolicyTestCase() 
{
}

DBTMergePolicyTestCase::~DBTMergePolicyTestCase() 
{
}

void DBTMergePolicyTestCase::setUp()
{
}

void DBTMergePolicyTestCase::tearDown()
{
}

void DBTMergePolicyTestCase::testCreateProposals()
{
    string sBarrel = "0, 10, 0; 1, 10, 0; 2, 10, 0";
    BarrelsInfoPtr pBarrelsInfo = createBarrelsInfo(sBarrel);
    DBTMergePolicy mergePolicy;
    MergeProposalPtr pProposal = mergePolicy.createProposals(*pBarrelsInfo);

    CPPUNIT_ASSERT(pProposal.isNotNull());

    MergeProposalPtr pExpProposal;
    makeAnswer(pExpProposal, sBarrel);

    checkProposals(pExpProposal, pProposal);
}

void DBTMergePolicyTestCase::testCreateProposalsWithMergePartial()
{
    string sBarrel = "0, 28, 0; 1, 10, 0; 2, 10, 0; 3, 10, 0";
    BarrelsInfoPtr pBarrelsInfo = createBarrelsInfo(sBarrel);
    DBTMergePolicy mergePolicy;
    MergeProposalPtr pProposal = mergePolicy.createProposals(*pBarrelsInfo);

    CPPUNIT_ASSERT(pProposal.isNotNull());

    string sExpMergeBarrel = "1, 10, 0; 2, 10, 0; 3, 10, 0";
    MergeProposalPtr pExpProposal;
    makeAnswer(pExpProposal, sExpMergeBarrel);

    checkProposals(pExpProposal, pProposal);
}

void DBTMergePolicyTestCase::testCreateProposalsWithCrossLevel()
{
    string sBarrel = "0, 28, 0; 1, 28, 0; 2, 10, 0; 3, 10, 0; 4, 10, 0";
    BarrelsInfoPtr pBarrelsInfo = createBarrelsInfo(sBarrel);
    DBTMergePolicy mergePolicy;
    MergeProposalPtr pProposal = mergePolicy.createProposals(*pBarrelsInfo);

    CPPUNIT_ASSERT(pProposal.isNotNull());

    MergeProposalPtr pExpProposal;
    makeAnswer(pExpProposal, sBarrel);

    checkProposals(pExpProposal, pProposal);
}

void DBTMergePolicyTestCase::testCreateProposalsWithTwoProposal()
{
    string sBarrel = "0, 82, 0; 1, 82, 0; 2, 82, 0; 3, 10, 0; 4, 10, 0; 5, 10, 0";
    BarrelsInfoPtr pBarrelsInfo = createBarrelsInfo(sBarrel);
    DBTMergePolicy mergePolicy;
    MergeProposalPtr pProposal = mergePolicy.createProposals(*pBarrelsInfo);

    CPPUNIT_ASSERT(pProposal.isNotNull());

    MergeProposalPtr pExpProposal;
    makeAnswer(pExpProposal, "3, 10, 0; 4, 10, 0; 5, 10, 0");
    makeAnswer(pExpProposal, "0, 82, 0; 1, 82, 0; 2, 82, 0");

    checkProposals(pExpProposal, pProposal);
}

BarrelsInfoPtr DBTMergePolicyTestCase::createBarrelsInfo(const string& sBarrel)
{
    BarrelsInfoPtr pBarrelsInfo = new BarrelsInfo();
    StringTokenizer st(sBarrel, ";", StringTokenizer::TOKEN_TRIM |
                       StringTokenizer::TOKEN_IGNORE_EMPTY);
    docid_t baseDocId = 0;
    for (size_t i = 0; i < st.getNumTokens(); ++i)
    {
        StringTokenizer st2(st[i], ",", StringTokenizer::TOKEN_TRIM |
                           StringTokenizer::TOKEN_IGNORE_EMPTY);
        int32_t commId = NumberParser::parseInt32(st2[0]);
        int32_t docCount = NumberParser::parseInt32(st2[1]);
        int32_t delDocCount = NumberParser::parseInt32(st2[2]);
        BarrelInfo barrelInfo(commId, baseDocId, docCount, delDocCount);
        pBarrelsInfo->addBarrel(barrelInfo);
        baseDocId += docCount;
    }
    return pBarrelsInfo;
}

void DBTMergePolicyTestCase::makeAnswer(MergeProposalPtr& pProposal, const string& sBarrel)
{
    if (pProposal.isNull())
    {
        pProposal = new MergeProposal();
    }

    BarrelsInfoPtr pBarrelsInfo = new BarrelsInfo();
    StringTokenizer st(sBarrel, ";", StringTokenizer::TOKEN_TRIM |
                       StringTokenizer::TOKEN_IGNORE_EMPTY);
    docid_t baseDocId = 0;
    for (size_t i = 0; i < st.getNumTokens(); ++i)
    {
        StringTokenizer st2(st[i], ",", StringTokenizer::TOKEN_TRIM |
                            StringTokenizer::TOKEN_IGNORE_EMPTY);
        int32_t commId = NumberParser::parseInt32(st2[0]);
        int32_t docCount = NumberParser::parseInt32(st2[1]);
        int32_t delDocCount = NumberParser::parseInt32(st2[2]);
        BarrelInfo barrelInfo(commId, baseDocId, docCount, delDocCount);
        pBarrelsInfo->addBarrel(barrelInfo);
        baseDocId += docCount;
    }
    pProposal->push_back(pBarrelsInfo);
}

void DBTMergePolicyTestCase::checkProposals(const MergeProposalPtr& pExpProposal,
        const MergeProposalPtr& pActProposal)
{
    CPPUNIT_ASSERT_EQUAL(pExpProposal->size(), pActProposal->size());
    for (size_t i = 0; i < pExpProposal->size(); ++i)
    {
        BarrelsInfoPtr pExpBarrelsInfo = (*pExpProposal)[i];
        BarrelsInfoPtr pActBarrelsInfo = (*pActProposal)[i];
        CPPUNIT_ASSERT_EQUAL(pExpBarrelsInfo->getBarrelCount(),
                             pActBarrelsInfo->getBarrelCount());
        BarrelsInfo::Iterator it1 = pExpBarrelsInfo->iterator();
        BarrelsInfo::Iterator it2 = pActBarrelsInfo->iterator();
        while (it1.hasNext())
        {
            const BarrelInfo& barrelInfo1 = it1.next();

            CPPUNIT_ASSERT(it2.hasNext());
            const BarrelInfo& barrelInfo2 = it2.next();

            CPPUNIT_ASSERT_EQUAL(barrelInfo1.getCommitId(), 
                    barrelInfo2.getCommitId());
        }
    }
}

FX_NS_END

