#include "utility/StringTokenizerTestCase.h"
#include "firtex/utility/StringTokenizer.h"
#include <stdio.h>
#include "TestHelper.h"

FX_NS_DEF(utility);

CPPUNIT_TEST_SUITE_REGISTRATION(StringTokenizerTestCase);

StringTokenizerTestCase::StringTokenizerTestCase(void)
{	
}

StringTokenizerTestCase::~StringTokenizerTestCase(void)
{	
}

void StringTokenizerTestCase::setUp()
{	
}

void StringTokenizerTestCase::tearDown()
{	
}

void StringTokenizerTestCase::testTokenizeNormal()
{
    StringTokenizer st(_T("中国;北京;奥运"), _T(";"));
    CPPUNIT_ASSERT_EQUAL((size_t)3, st.getNumTokens());
    CPPUNIT_ASSERT_EQUAL((tstring)_T("中国"), st[0]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("北京"), st[1]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("奥运"), st[2]);

    tstring str(_T("中国;北京;奥运"));
    StringTokenizer st2(str.c_str(), str.length(), _T(";"));
    CPPUNIT_ASSERT_EQUAL((size_t)3, st2.getNumTokens());
    CPPUNIT_ASSERT_EQUAL((tstring)_T("中国"), st2[0]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("北京"), st2[1]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("奥运"), st2[2]);


    StringTokenizer st3(_T("中国"), _T(";"));
    CPPUNIT_ASSERT_EQUAL((size_t)1, st3.getNumTokens());
    CPPUNIT_ASSERT_EQUAL((tstring)_T("中国"), st3[0]);
}

void StringTokenizerTestCase::testTokenizeSpaceSep()
{
    StringTokenizer st(_T("中国 北京 奥运"), _T(" "));
    CPPUNIT_ASSERT_EQUAL((size_t)3, st.getNumTokens());
    CPPUNIT_ASSERT_EQUAL((tstring)_T("中国"), st[0]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("北京"), st[1]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("奥运"), st[2]);
}

void StringTokenizerTestCase::testTokenizeWithSpace()
{
    StringTokenizer st;
    st.tokenize(_T(" 中国;北京 ;奥运 "), _T(";"));
    CPPUNIT_ASSERT_EQUAL((size_t)3, st.getNumTokens());
    CPPUNIT_ASSERT_EQUAL((tstring)_T(" 中国"), st[0]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("北京 "), st[1]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("奥运 "), st[2]);

    StringTokenizer st2;
    st2.tokenize(_T(" 中国;北京 ;奥运 "), _T(";"), StringTokenizer::TOKEN_TRIM);
    CPPUNIT_ASSERT_EQUAL((size_t)3, st2.getNumTokens());
    CPPUNIT_ASSERT_EQUAL((tstring)_T("中国"), st2[0]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("北京"), st2[1]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("奥运"), st2[2]);
}

void StringTokenizerTestCase::testTokenizeIgnoreEmpty()
{
    StringTokenizer st(_T("中国; ;;奥运"), _T(";"),
                       StringTokenizer::TOKEN_IGNORE_EMPTY);
    CPPUNIT_ASSERT_EQUAL((size_t)3, st.getNumTokens());
    CPPUNIT_ASSERT_EQUAL((tstring)_T("中国"), st[0]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T(" "), st[1]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("奥运"), st[2]);
}

void StringTokenizerTestCase::testTokenizeTrimIgnoreEmpty()
{
    StringTokenizer st(_T("中国; ;;奥运"), _T(";"),
                       StringTokenizer::TOKEN_TRIM
                       | StringTokenizer::TOKEN_IGNORE_EMPTY);
    CPPUNIT_ASSERT_EQUAL((size_t)2, st.getNumTokens());
    CPPUNIT_ASSERT_EQUAL((tstring)_T("中国"), st[0]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("奥运"), st[1]);
}

void StringTokenizerTestCase::testTokenizeLeaveAs()
{
    StringTokenizer st(_T("中国; ;;奥运"), _T(";"));
    CPPUNIT_ASSERT_EQUAL((size_t)4, st.getNumTokens());
    CPPUNIT_ASSERT_EQUAL((tstring)_T("中国"), st[0]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T(" "), st[1]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T(""), st[2]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("奥运"), st[3]);
}

void StringTokenizerTestCase::testTokenizeWithSpaceSep()
{
    StringTokenizer st(_T("中国 北京 奥运"), _T(" "));
    CPPUNIT_ASSERT_EQUAL((size_t)3, st.getNumTokens());
    CPPUNIT_ASSERT_EQUAL((tstring)_T("中国"), st[0]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("北京"), st[1]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("奥运"), st[2]);
}

void StringTokenizerTestCase::testTokenizeWithSpaceSepAndTrim()
{
    StringTokenizer st(_T(" 中国 北京  奥运 "), _T(" "),
                       StringTokenizer::TOKEN_TRIM);
    CPPUNIT_ASSERT_EQUAL((size_t)6, st.getNumTokens());
    CPPUNIT_ASSERT_EQUAL((tstring)_T(""), st[0]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("中国"), st[1]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("北京"), st[2]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T(""), st[3]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("奥运"), st[4]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T(""), st[5]);
}

void StringTokenizerTestCase::testTokenizeWithSpaceSepAndTrimIgnoreEmpty()
{
    StringTokenizer st(_T(" 中国 北京  奥运 "), _T(" "),
                       StringTokenizer::TOKEN_TRIM |
                       StringTokenizer::TOKEN_IGNORE_EMPTY);
    CPPUNIT_ASSERT_EQUAL((size_t)3, st.getNumTokens());
    CPPUNIT_ASSERT_EQUAL((tstring)_T("中国"), st[0]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("北京"), st[1]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("奥运"), st[2]);
}

void StringTokenizerTestCase::testTokenizeWithTwoCharSep()
{
    StringTokenizer st(_T("中国::北京::奥运"), _T("::"));
    CPPUNIT_ASSERT_EQUAL((size_t)3, st.getNumTokens());
    CPPUNIT_ASSERT_EQUAL((tstring)_T("中国"), st[0]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("北京"), st[1]);
    CPPUNIT_ASSERT_EQUAL((tstring)_T("奥运"), st[2]);
}

FX_NS_END
