#include "analyzer/SingleWordAnalyzerTestCase.h"
#include "firtex/analyzer/SingleWordAnalyzer.h"

using namespace std;

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, SingleWordAnalyzerTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(SingleWordAnalyzerTestCase);

SingleWordAnalyzerTestCase::SingleWordAnalyzerTestCase() 
{
}

SingleWordAnalyzerTestCase::~SingleWordAnalyzerTestCase() 
{
}

void SingleWordAnalyzerTestCase::setUp()
{
}

void SingleWordAnalyzerTestCase::tearDown()
{
}

void SingleWordAnalyzerTestCase::testTokenize()
{
    SingleWordAnalyzer sa("utf-8");
    sa.init();
    
    string str = "我爱北京天安门。firtex Open source激情ruijieguo@gmail.com";
    TokenViewPtr pTokenView = sa.tokenize(str.c_str());

    const static size_t NUM_TOKENS = 13;
    string answer[NUM_TOKENS] = {"我", "爱", "北", "京", "天", "安", "门",
                                 "firtex", "Open", "source", "激", "情",
                                 "ruijieguo@gmail.com"};
    int32_t posInc[NUM_TOKENS] = {1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1};
    int32_t startOff[NUM_TOKENS] = {0, 3, 6, 9, 12, 15, 18, 24, 31, 36, 42, 45, 48};
    int32_t endOff[NUM_TOKENS] = {3, 6, 9, 12, 15, 18, 21, 30, 35, 42, 45, 48, 67};

    TokenView::Iterator it = pTokenView->iterator();
    CPPUNIT_ASSERT_EQUAL(NUM_TOKENS, it.size());
    size_t i = 0;
    while (it.hasNext())
    {
        const Token& token = it.next();
        CPPUNIT_ASSERT_EQUAL(answer[i], string(token.getTextValue()));
        CPPUNIT_ASSERT_EQUAL(posInc[i], token.getPosIncrement());
        CPPUNIT_ASSERT_EQUAL(startOff[i], token.getStartOffset());
        CPPUNIT_ASSERT_EQUAL(endOff[i], token.getEndOffset());

        // cout << token.getTextValue() << std::endl;
        // cout << token.getPosIncrement() << ", " << 
        //     token.getStartOffset() << ", " << token.getEndOffset() << std::endl;
        ++i;
    }
}

FX_NS_END

