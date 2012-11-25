#include "analyzer/StandardTokenizerTestCase.h"
#include "firtex/analyzer/StandardTokenizer.h"
#include "firtex/utility/BinaryFile.h"
#include "TestHelper.h"

using namespace std;

FX_NS_USE(utility);

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, StandardTokenizerTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(StandardTokenizerTestCase);

StandardTokenizerTestCase::StandardTokenizerTestCase() 
{
}

StandardTokenizerTestCase::~StandardTokenizerTestCase() 
{
}

void StandardTokenizerTestCase::setUp()
{
}

void StandardTokenizerTestCase::tearDown()
{
}

void StandardTokenizerTestCase::testTokenzieUTF8()
{
    UTF8StandardTokenizer st;
    TokenViewPtr pTokenView = new TokenView();
    string str = "my我的。，？￥１２ＡＢruijieguo@gmail.com ,."
                 "O'Reilly I.B.M. AT&T 1234 010-123456 192.168.1.1"
                 "\xb2\xe2\xca\xd4word\xa1\xa3";
    st.tokenize(pTokenView, str.c_str(), str.length());

    const static size_t TOKEN_COUNT = 13;
    string tokens[TOKEN_COUNT] = 
        {"my", "我", "的", "１２",
         "ＡＢ", "ruijieguo@gmail.com", "O'Reilly",
         "I.B.M.", "AT&T", "1234", "010-123456", 
         "192.168.1.1", "word"};
    int32_t posInc[TOKEN_COUNT] = {1, 1, 1, 5, 1, 1, 3, 1, 1, 1, 1, 1, 5};
    int32_t startOff[TOKEN_COUNT] = {0, 2, 5, 20, 26, 32, 54, 63, 70, 75, 80, 91, 106};
    int32_t endOff[TOKEN_COUNT] = {2, 5, 8, 26, 32, 51, 62, 69, 74, 79, 90, 102, 110};

    TokenView::Iterator it = pTokenView->iterator();

    CPPUNIT_ASSERT_EQUAL(TOKEN_COUNT, it.size());

    size_t i = 0;
    while (it.hasNext())
    {
        const Token& token = it.next();
        CPPUNIT_ASSERT_EQUAL(tokens[i], string(token.getTextValue())); 
        CPPUNIT_ASSERT_EQUAL(posInc[i], token.getPosIncrement());
        CPPUNIT_ASSERT_EQUAL(startOff[i], token.getStartOffset());
        CPPUNIT_ASSERT_EQUAL(endOff[i], token.getEndOffset());

        // cout << token.getTextValue() << std::endl;
        // cout << token.getPosIncrement() << ", " << 
        //     token.getStartOffset() << ", " << token.getEndOffset() << std::endl;
        ++i;
    }

    CPPUNIT_ASSERT(!it.hasNext());
}

void StandardTokenizerTestCase::testTokenzieGBK()
{
    GBKStandardTokenizer st;
    TokenViewPtr pTokenView = new TokenView();

    //gbk code of : “四放开”改革就是按照社会主义市场经济的要求，把企业推向市场。1234场
    string str = "\xa1\xb0\xcb\xc4\xb7\xc5\xbf\xaa\xa1\xb1\xb8\xc4"
                 "\xb8\xef\xbe\xcd\xca\xc7\xb0\xb4\xd5\xd5\xc9\xe7"
                 "\xbb\xe1\xd6\xf7\xd2\xe5\xca\xd0\xb3\xa1\xbe\xad"
                 "\xbc\xc3\xb5\xc4\xd2\xaa\xc7\xf3\xa3\xac\xb0\xd1"
                 "\xc6\xf3\xd2\xb5\xcd\xc6\xcf\xf2\xca\xd0\xb3\xa1\xa1\xa3"
                 "12345\xb3\xa1";

    st.tokenize(pTokenView, str.c_str(), str.length());

    const static size_t TOKEN_COUNT = 29;
    string tokens[TOKEN_COUNT] = 
        {"\xcb\xc4", "\xb7\xc5", "\xbf\xaa", "\xb8\xc4", 
         "\xb8\xef", "\xbe\xcd", "\xca\xc7", "\xb0\xb4", "\xd5\xd5", "\xc9\xe7", 
         "\xbb\xe1", "\xd6\xf7", "\xd2\xe5", "\xca\xd0", "\xb3\xa1", "\xbe\xad", 
         "\xbc\xc3", "\xb5\xc4", "\xd2\xaa", "\xc7\xf3", "\xb0\xd1", 
         "\xc6\xf3", "\xd2\xb5", "\xcd\xc6", "\xcf\xf2", "\xca\xd0", "\xb3\xa1", 
         "12345","\xb3\xa1",};

    int32_t posInc[TOKEN_COUNT] = {2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
                                   1, 1, 1, 1, 1, 1, 1, 2, 1, 1,
                                   1, 1, 1, 1, 2, 1};
    int32_t startOff[TOKEN_COUNT] = {2, 4, 6, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 
                                     32, 34, 36, 38, 40, 42, 46, 48, 50,
                                     52, 54, 56, 58, 62, 67};
    int32_t endOff[TOKEN_COUNT] = {4, 6, 8, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 
                                   34, 36, 38, 40, 42, 44, 48, 50, 52,
                                   54, 56, 58, 60, 67, 69};

    TokenView::Iterator it = pTokenView->iterator();

    CPPUNIT_ASSERT_EQUAL(TOKEN_COUNT, it.size());

    size_t i = 0;
    while (it.hasNext())
    {
        const Token& token = it.next();
        CPPUNIT_ASSERT_EQUAL(tokens[i], string(token.getTextValue())); 
        CPPUNIT_ASSERT_EQUAL(posInc[i], token.getPosIncrement());
        CPPUNIT_ASSERT_EQUAL(startOff[i], token.getStartOffset());
        CPPUNIT_ASSERT_EQUAL(endOff[i], token.getEndOffset());

        // cout << token.getTextValue() << std::endl;
        // cout << token.getPosIncrement() << ", " << 
        //     token.getStartOffset() << ", " << token.getEndOffset() << std::endl;
        ++i;
    }

    CPPUNIT_ASSERT(!it.hasNext());
}

void StandardTokenizerTestCase::testTokenizeGBKFile()
{
    string sPath = TestHelper::getTestDataPath("txt_files", true);
    sPath += "/0.txt";
    BinaryFile bf;
    bf.open(sPath, BinaryFile::READ);
    CPPUNIT_ASSERT(bf.isFileOpen());
    
    string data;
    data.resize((size_t)bf.getLength());
    bf.read((void*)data.c_str(), data.size());

    TokenViewPtr pTokenView = new TokenView();
    GBKStandardTokenizer st;
    st.tokenize(pTokenView, data.c_str(), data.length());

    // TokenView::Iterator it = pTokenView->iterator();
    // while (it.hasNext())
    // {
    //     const Token& token = it.next();
    //     cout << token.getTextValue() << endl;
    //     cout << token.getPosIncrement() << ", " << 
    //         token.getStartOffset() << ", " << token.getEndOffset() << std::endl;

    // }
}

void StandardTokenizerTestCase::testTokenizeUTF8File()
{
    string sPath = TestHelper::getTestDataPath("utf8_txt_files", true);
    sPath += "/0.txt";
    BinaryFile bf;
    bf.open(sPath, BinaryFile::READ);
    CPPUNIT_ASSERT(bf.isFileOpen());
    
    string data;
    data.resize((size_t)bf.getLength());
    bf.read((void*)data.c_str(), data.size());

    TokenViewPtr pTokenView = new TokenView();
    UTF8StandardTokenizer st;
    st.tokenize(pTokenView, data.c_str(), data.length());

    // TokenView::Iterator it = pTokenView->iterator();
    // while (it.hasNext())
    // {
    //     const Token& token = it.next();
    //     cout << token.getTextValue() << endl;
    //     cout << token.getPosIncrement() << ", " << 
    //         token.getStartOffset() << ", " << token.getEndOffset() << std::endl;

    // }
}

FX_NS_END

