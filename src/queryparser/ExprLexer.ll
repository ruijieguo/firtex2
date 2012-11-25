%{
#include "firtex/common/StdHeader.h"
#include <string>
#include "firtex/queryparser/ExprParser.h"
#include "firtex/queryparser/ExprLexer.h"

using namespace fx_bison;

//FX_NS_USE(queryparser);

typedef ExprBisonParser::token token;
typedef ExprBisonParser::token_type token_type;

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
#define	YY_DECL	token_type firtex::queryparser::ExprLexer::lex(   \
            semantic_type* yylval, location_type* yylloc)

%}

/** enable c++ scanner class generation */
%option c++
%option noyywrap

/** Change the name of the scanner class. results in "ExprFlexLexer" */
%option prefix="Expr"
%option yyClass="ExprLexer"

/* The manual says "somewhat more optimized" */
%option batch

/**
 * Enable scanner to generate debug output. disable this for release
 * versions. 
 */
%option debug

%x SINGLE_QUOTED DOUBLE_QUOTED

DIGIT            [0-9] 
ID               [a-zA-Z_][a-zA-Z_0-9]*
WHITESPACE       [ \t\n\r\u3000]
SIGNED_CHAR      [+-]

%%

{SIGNED_CHAR}?{DIGIT}+ { 
    FX_TRACE("[%s]: INT", YYText());
    yylval->intVal = STRTOLL(YYText(), NULL, 10);
    return token::TOK_INT;
 }

{SIGNED_CHAR}?{DIGIT}*\.{DIGIT}* {
    FX_TRACE("[%s]: DOUBLE", YYText());
    yylval->doubleVal = strtod(YYText(), NULL);
    return token::TOK_DOUBLE;
}

"OR" { 
    FX_TRACE("OR");
    return token::TOK_OR; 
}
"AND" {
    FX_TRACE("AND");
    return token::TOK_AND;
}
"NOT" {
    FX_TRACE("NOT");
    return token::TOK_NOT;
}

"=" { 
    FX_TRACE("=");
    return token::TOK_EQ; 
}
">=" {
    FX_TRACE(">=");
    return token::TOK_GTE;
}
"<=" {
    FX_TRACE("<=");
    return token::TOK_LTE;
}
"!=" {
    FX_TRACE("!=");
    return token::TOK_NE;
}
"<>" {
    FX_TRACE("<>");
    return token::TOK_NE;
}

"DATE" {
    FX_TRACE("DATE");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC_DATE; 
}

"NOW" {
    FX_TRACE("NOW");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"ABS" {
    FX_TRACE("ABS");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"CEIL" {
    FX_TRACE("CEIL");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"FLOOR" {
    FX_TRACE("FLOOR");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"SIN" {
    FX_TRACE("SIN");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"COS" {
    FX_TRACE("COS");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"LN" {
    FX_TRACE("LN");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"LOG2" {
    FX_TRACE("LOG2");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"LOG10" {
    FX_TRACE("LOG10");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"EXP" {
    FX_TRACE("EXP");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"SQRT" {
    FX_TRACE("SQRT");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"INT64" {
    FX_TRACE("INT64()");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"INT32" {
    FX_TRACE("INT32()");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"DOUBLE" {
    FX_TRACE("DOUBLE Func");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"FLOAT" {
    FX_TRACE("FLOAT Func");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"MIN" {
    FX_TRACE("OP: MIN");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"MAX" {
    FX_TRACE("OP: MAX");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"POW" {
    FX_TRACE("OP: POW");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"IDIV" {
    FX_TRACE("IDIV");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"IF" {
    FX_TRACE("IF");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"MADD" {
    FX_TRACE("MADD");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"MUL3" {
    FX_TRACE("MUL3");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC; 
}

"IN" {
    FX_TRACE("IN");
    yylval->funcIdx = ExprParser::getFuncIdx(YYText());
    return token::TOK_FUNC_IN; 
}

{ID} {
    FX_TRACE("[%s]: TOK_IDENT", YYText());
    yylval->stringVal = new std::string(YYText(), YYLeng());
    return token::TOK_IDENT;
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

        return token::TOK_QUOTED_STRING;
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
        BEGIN(INITIAL);
        return token::TOK_QUOTED_STRING;
    }
}

{WHITESPACE} { }
. {
    FX_TRACE("[%s]", YYText());
    return (token_type)YYText()[0];
}

<SINGLE_QUOTED,DOUBLE_QUOTED><<EOF>> {return token::END;}


%%
/** Additional Code */

FX_NS_USE(common);

FX_NS_DEF(queryparser);

SETUP_STREAM_LOGGER(queryparser, ExprLexer);

ExprLexer::ExprLexer(std::istream *input, std::ostream *output) 
    : yyFlexLexer(input, output)
{
}

ExprLexer::~ExprLexer() 
{
}

void ExprLexer::setDebug(bool debug) 
{
    yy_flex_debug = debug;
}

FX_NS_END
