%{
#include "firtex/common/StdHeader.h"
#include <string>
#include "firtex/queryparser/QueryExprParser.h"
#include "firtex/queryparser/QueryExprLexer.h"

using namespace fx_bison;

typedef QueryExprBisonParser::token token;
typedef QueryExprBisonParser::token_type token_type;

/**
 * By default yylex returns int, we use token_type. Unfortunately yyterminate
 * by default returns 0, which is not of token_type. 
 */
#define yyterminate() return token::END

/**
 * This disables inclusion of unistd.h, which is not available under Visual C++
 * on Win32. The C++ scanner uses STL streams instead. 
 */
#define YY_NO_UNISTD_H

#undef YY_DECL
#define	YY_DECL	token_type firtex::queryparser::QueryExprLexer::lex(   \
            semantic_type* yylval, location_type* yylloc)
%}

/** enable c++ scanner class generation */
%option c++
%option noyywrap

 /** Change the name of the scanner class. results in "QueryExprFlexLexer" */
%option prefix="QueryExpr"
%option yyClass="QueryExprLexer"

/* The manual says "somewhat more optimized" */
%option batch

/**
 * Enable scanner to generate debug output. disable this for release
 * versions. 
 */
%option debug

%x SINGLE_QUOTED DOUBLE_QUOTED

DIGIT             [0-9] 
IDENT_START_CHAR  [_[:alpha:]] 
IDENT_CHAR        [_[:alnum:]._] 
WHITESPACE        [ \t\n\r\u3000]

SIGNED_CHAR  [+-]

CJK_1 (\xe2[\xba-\xbf][\x80-\xbf]) 
CJK_2 ([\xe3-\xe9][\x80-\xbf][\x80-\xbf])

SBC_1 (\xef[\xbc-\xbe][\x80-\xbf])
SBC_2 (\xef\xbf[\x80-\xaf])

SYMBOL_CHAR    [.\*/&!_`?$\\%|;#@]
%%

AND {
     return token::TOK_AND;
}

"&&" {
     return token::TOK_AND;
}

OR {
    return token::TOK_OR;
}

"||" {
    return token::TOK_OR;
}

"TO" { 
    return token::TOK_RANGE_TO;
}

"NOT" {
    return token::TOK_MOD_NOT;
}

"+" { 
    FX_TRACE("+: MOD_REQ");
    return token::TOK_MOD_REQ;
}

"-" {
    FX_TRACE("+: MOD_NOT");
    return token::TOK_MOD_NOT;
}

"^" {
    FX_TRACE("^: BOOST");
    return token::TOK_BOOST;
}

"~" {
    FX_TRACE("~: PHRASE_SLOP");
    return token::TOK_PHRASE_SLOP;
}

"*:*" {
    FX_TRACE("*:*: QUERY_ANY");
    return token::TOK_QUERY_ANY;
}

[:\]\[(),{}] {
    FX_TRACE("[%s]: SYMBOL]", YYText());
    return static_cast<token_type>(*YYText());
}

{SIGNED_CHAR}?{DIGIT}+ { 
    FX_TRACE("[%s: INT]", YYText());
    yylval->numberVal = new NumberNode;
    yylval->numberVal->m_sValue = YYText();
    yylval->numberVal->m_i64Val = STRTOLL(YYText(), NULL, 10);
    return token::TOK_INT;
 }

{SIGNED_CHAR}?{DIGIT}*\.{DIGIT}* {
    FX_TRACE("[%s: FLOAT]", YYText());
    yylval->numberVal = new NumberNode;
    yylval->numberVal->m_sValue = YYText();
    yylval->numberVal->m_fVal = (float)strtod(YYText(), NULL);
    return token::TOK_FLOAT;
}

{IDENT_START_CHAR}{IDENT_CHAR}* { 
    FX_TRACE("[%s: TOK_IDENT]", YYText());
    yylval->stringVal = new std::string(YYText(), YYLeng());
    return token::TOK_IDENT;
}

({CJK_1}|{CJK_2}|{SBC_1}|{SBC_2}|{IDENT_CHAR}|{SYMBOL_CHAR})+ {
    FX_TRACE("[%s: TOK_CJK]", YYText());
    yylval->stringVal = new std::string(YYText(), YYLeng());
    return token::TOK_CJK;
}

\" {
    BEGIN(DOUBLE_QUOTED);
    oss.str("");
}

<DOUBLE_QUOTED>{
    [^\\\"]+ {
        oss << YYText();
    }
    \\[\\\"] {
        oss << (const char *)(YYText() + 1);
    }
    \" {
        BEGIN(INITIAL);
        yylval->stringVal = new std::string(oss.str());

        return token::TOK_DOUBLE_QUOTED;
    }
}

\' {
    BEGIN(SINGLE_QUOTED);
    oss.str("");
}

<SINGLE_QUOTED>{
    [^\\\']+ {
        oss << YYText();
    }
    \\[\\\'] {
        oss << (YYText() + 1);
    }
    \' {
        yylval->stringVal = new std::string(oss.str());
        FX_TRACE("[%s : SINGLE QUOTED TOK_CJK]", yylval->stringVal->c_str());
        BEGIN(INITIAL);
        return token::TOK_SINGLE_QUOTED;
    }
}

"=" {
    return token::TOK_EQ;
}

">=" {
    return token::TOK_GE;
}

"<=" {
    return token::TOK_LE;
}

"<" {
    return token::TOK_LESS;
}

">" {
    return token::TOK_GREATER;
}


{WHITESPACE} { }

<SINGLE_QUOTED,DOUBLE_QUOTED><<EOF>> {
    return token::END;
}

%%
/** Additional Code */

FX_NS_USE(common);

FX_NS_DEF(queryparser);
SETUP_STREAM_LOGGER(queryparser, QueryExprLexer);

QueryExprLexer::QueryExprLexer(std::istream *input, std::ostream *output) 
    : yyFlexLexer(input, output)
{
}

QueryExprLexer::~QueryExprLexer() 
{
}

void QueryExprLexer::setDebug(bool debug) 
{
    yy_flex_debug = debug;
}

FX_NS_END
