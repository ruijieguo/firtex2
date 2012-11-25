#include "queryparser/QueryExprLexerTestCase.h"

using namespace std;

FX_NS_DEF(queryparser);

SETUP_STREAM_LOGGER(queryparser, QueryExprLexerTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(QueryExprLexerTestCase);

QueryExprLexerTestCase::QueryExprLexerTestCase() 
{
}

QueryExprLexerTestCase::~QueryExprLexerTestCase() 
{
}

void QueryExprLexerTestCase::setUp()
{
    _yylval.stringVal = NULL;
}

void QueryExprLexerTestCase::tearDown()
{
}

void QueryExprLexerTestCase::testSimpleTerm()
{
    TokenVect tokens;
    string str = "singleterm";
    lex(str, tokens);
    CPPUNIT_ASSERT_EQUAL(size_t(1), tokens.size());
    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_IDENT, tokens[0].first);
    CPPUNIT_ASSERT_EQUAL(str, *(tokens[0].second));
}

void QueryExprLexerTestCase::testAndOp()
{
    TokenVect tokens;
    string str = "q1 AND q2";
    lex(str, tokens);
    CPPUNIT_ASSERT_EQUAL(size_t(3), tokens.size());
    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_IDENT, tokens[0].first);
    CPPUNIT_ASSERT_EQUAL(string("q1"), *(tokens[0].second));
    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_AND, tokens[1].first);

    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_IDENT, tokens[2].first);
    CPPUNIT_ASSERT_EQUAL(string("q2"), *(tokens[2].second));

    tokens.clear();
    str = "q1 && q2";
    lex(str, tokens);
    CPPUNIT_ASSERT_EQUAL(size_t(3), tokens.size());
    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_IDENT, tokens[0].first);
    CPPUNIT_ASSERT_EQUAL(string("q1"), *(tokens[0].second));
    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_AND, tokens[1].first);

    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_IDENT, tokens[2].first);
    CPPUNIT_ASSERT_EQUAL(string("q2"), *(tokens[2].second));
}

void QueryExprLexerTestCase::testOrOp()
{
    TokenVect tokens;
    string str = "q1 OR q2";
    lex(str, tokens);
    CPPUNIT_ASSERT_EQUAL(size_t(3), tokens.size());
    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_IDENT, tokens[0].first);
    CPPUNIT_ASSERT_EQUAL(string("q1"), *(tokens[0].second));
    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_OR, tokens[1].first);

    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_IDENT, tokens[2].first);
    CPPUNIT_ASSERT_EQUAL(string("q2"), *(tokens[2].second));

    tokens.clear();
    str = "q1 || q2";
    lex(str, tokens);
    CPPUNIT_ASSERT_EQUAL(size_t(3), tokens.size());
    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_IDENT, tokens[0].first);
    CPPUNIT_ASSERT_EQUAL(string("q1"), *(tokens[0].second));
    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_OR, tokens[1].first);

    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_IDENT, tokens[2].first);
    CPPUNIT_ASSERT_EQUAL(string("q2"), *(tokens[2].second));
}

void QueryExprLexerTestCase::testWhitespace()
{
    TokenVect tokens;
    string str = " \t\n\r";
    lex(str, tokens);
    CPPUNIT_ASSERT_EQUAL(size_t(0), tokens.size());
}

void QueryExprLexerTestCase::testPlus()
{
    TokenVect tokens;
    string str = "+test";
    lex(str, tokens);
    CPPUNIT_ASSERT_EQUAL(size_t(2), tokens.size());
    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_MOD_REQ, tokens[0].first);
    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_IDENT, tokens[1].first);
    CPPUNIT_ASSERT_EQUAL(string("test"), *(tokens[1].second));
}

void QueryExprLexerTestCase::testMinus()
{
    TokenVect tokens;
    string str = "-test";
    lex(str, tokens);
    CPPUNIT_ASSERT_EQUAL(size_t(2), tokens.size());
    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_MOD_NOT, tokens[0].first);
    CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_IDENT, tokens[1].first);
    CPPUNIT_ASSERT_EQUAL(string("test"), *(tokens[1].second));
}

void QueryExprLexerTestCase::testBrace()
{
    TokenVect tokens;
    string str = "[323d2";
    lex(str, tokens);
    CPPUNIT_ASSERT_EQUAL(size_t(2), tokens.size());
    // CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_IDENT, tokens[0].first);
    // CPPUNIT_ASSERT_EQUAL(string("q1"), *(tokens[0].second));
    // CPPUNIT_ASSERT_EQUAL(QueryExprLexer::token::TOK_OR, tokens[1].first);
}

void QueryExprLexerTestCase::testEscapedChar()
{
    // TokenVect tokens;
    // string str = "\^";
    // lex(str, tokens);
    // CPPUNIT_ASSERT_EQUAL(size_t(1), tokens.size());
//    CPPUNIT_ASSERT_EQUAL(string("^"), *(tokens[0].second));
}

void QueryExprLexerTestCase::testIdent()
{
    {
        TokenVect tokens;
        string str = "fieldname";
        lex(str, tokens);
        CPPUNIT_ASSERT_EQUAL(size_t(1), tokens.size());
    }

    {
        TokenVect tokens;
        string str = "field.name";
        lex(str, tokens);
        CPPUNIT_ASSERT_EQUAL(size_t(1), tokens.size());
    }

    {
        TokenVect tokens;
        string str = "field_name";
        lex(str, tokens);
        CPPUNIT_ASSERT_EQUAL(size_t(1), tokens.size());
    }
}

void QueryExprLexerTestCase::lex(const string& exprStr, TokenVect& tokens)
{
    istringstream iss(exprStr);
    QueryExprLexer scanner(&iss, &cout);
    QueryExprLexer::token_type tokenType = scanner.lex(&_yylval, &_yylocation);

    while (tokenType != QueryExprLexer::token::END)
    {
        tokens.push_back(make_pair(tokenType, _yylval.stringVal));
        _yylval.stringVal = NULL; 
        tokenType = scanner.lex(&_yylval, &_yylocation);
    }
}

FX_NS_END

