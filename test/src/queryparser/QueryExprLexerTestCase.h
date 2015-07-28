#ifndef __FX_LEXERTESTCASE_H
#define __FX_LEXERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/queryparser/QueryExprLexer.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

FX_NS_DEF(queryparser);

class QueryExprLexerTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(QueryExprLexerTestCase);
    CPPUNIT_TEST(testSimpleTerm);
    CPPUNIT_TEST(testAndOp);
    CPPUNIT_TEST(testOrOp);
    CPPUNIT_TEST(testWhitespace);
    CPPUNIT_TEST(testPlus);
    CPPUNIT_TEST(testMinus);
    CPPUNIT_TEST(testBrace);
//    CPPUNIT_TEST(testEscapedChar);
    CPPUNIT_TEST(testIdent);
    CPPUNIT_TEST_SUITE_END();
public:
    QueryExprLexerTestCase();
    ~QueryExprLexerTestCase();

    void setUp();
    void tearDown();

protected:
    void testSimpleTerm();
    void testAndOp();
    void testOrOp();
    void testWhitespace();
    void testPlus();
    void testMinus();
    void testEscapedChar();
    void testBrace();
    void testIdent();
private:
    QueryExprLexer::semantic_type _yylval;
    QueryExprLexer::location_type _yylocation;
    
    DEFINE_TYPED_PTR(std::string);

    typedef std::pair<QueryExprLexer::token_type, StringPtr> TokenPair;
    typedef std::vector<TokenPair> TokenVect;

private:
    void lex(const std::string& exprStr, TokenVect& tokens);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_LEXERTESTCASE_H
