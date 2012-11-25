%{
#include <string>
#include "firtex/common/StdHeader.h"
#include "firtex/analyzer/GBKTokenLexer.h"

#ifdef FX_WINDOWS
#pragma warning(push)
#pragma warning(disable:4267)
#endif

/**
 * This disables inclusion of unistd.h, which is not available under Visual C++
 * on Win32. The C++ scanner uses STL streams instead. 
 */
#define YY_NO_UNISTD_H

#undef YY_DECL
#define YY_DECL firtex::analyzer::GBKTokenLexer::TokenType      \
        firtex::analyzer::GBKTokenLexer::lex(                   \
                const char*& pText, size_t& length)

%}

/** enable c++ scanner class generation */
%option c++
%option noyywrap

/** Change the name of the scanner class. results in "GBKTokenFlexLexer" */
%option prefix="GBKToken"
%option yyClass="GBKTokenLexer"

/* The manual says "somewhat more optimized" */
%option batch

/**
 * Enable scanner to generate debug output. disable this for release
 * versions. 
 */
%option debug

DIGIT        [0-9] 
WHITESPACE   [ \t\f\u3000]
LINE_BREAK   [\n\r]
ALPHA        [A-Za-z]
ALPHANUM     ({ALPHA}|{DIGIT})+

GBK_DBC_SYMBOL   (\xa1[\xa2-\xfe])
GBK_SBC_SYMBOL   (\xa3[\xa1-\xfe])

GBK          [\x81-\xfe][\x40-\xfe]

HAS_DIGIT    ({ALPHA}|{DIGIT})*{DIGIT}({ALPHA}|{DIGIT})*
PUNCT        ("_"|"-"|"/"|"."|",")
APOSTROPHE   {ALPHA}("'"|{ALPHA})+
ACRONYM      {ALPHA}"."({ALPHA}".")+
COMPANY      {ALPHA}+("&"|"@"){ALPHA}+
EMAIL        {ALPHANUM}(("."|"-"|"_"){ALPHANUM})*("@"{ALPHANUM}(("."|"-"){ALPHANUM})+)
HOST         {ALPHANUM}("."{ALPHANUM})+
NUM          ({ALPHANUM}{PUNCT}{HAS_DIGIT}|{HAS_DIGIT}{PUNCT}{ALPHANUM}|{ALPHANUM}({PUNCT}{HAS_DIGIT}{PUNCT}{ALPHANUM})+|{HAS_DIGIT}({PUNCT}{ALPHANUM}{PUNCT}{HAS_DIGIT})+|{ALPHANUM}{PUNCT}{HAS_DIGIT}({PUNCT}{ALPHANUM}{PUNCT}{HAS_DIGIT})+|{HAS_DIGIT}{PUNCT}{ALPHANUM}({PUNCT}{HAS_DIGIT}{PUNCT}{ALPHANUM})+)

%%

{ALPHANUM} {
    FX_DEBUG("ALPHANUM: [%s]", YYText());
    pText = YYText();
    length = (size_t)YYLeng();
    return Token::TT_STRING;
}

{APOSTROPHE} {
    FX_DEBUG("APOSTROPHE: [%s]", YYText());
    pText = YYText();
    length = (size_t)YYLeng();
    return Token::TT_STRING;
}

{ACRONYM} {
    FX_DEBUG("ACRONYM: [%s]", YYText());
    pText = YYText();
    length = (size_t)YYLeng();
    return Token::TT_STRING;
}

{COMPANY} {
    FX_DEBUG("COMPANY: [%s]", YYText());
    pText = YYText();
    length = (size_t)YYLeng();
    return Token::TT_STRING;
}

{EMAIL} {
    FX_DEBUG("EMAIL: [%s]", YYText());
    pText = YYText();
    length = (size_t)YYLeng();
    return Token::TT_STRING;
}

{HOST} {
    FX_DEBUG("HOST: [%s]", YYText());
    pText = YYText();
    length = (size_t)YYLeng();
    return Token::TT_STRING;
}

{NUM} {
    FX_DEBUG("NUM: [%s]", YYText());
    pText = YYText();
    length = (size_t)YYLeng();
    return Token::TT_STRING;
}

{LINE_BREAK} {
    pText = NULL;
    length = (size_t)YYLeng();
    return Token::TT_LINE_BREAK;
}

{WHITESPACE} { 
    pText = NULL;
    length = (size_t)YYLeng();
    return Token::TT_WHITESPACE;
}

{GBK_DBC_SYMBOL} {
    FX_DEBUG("GB_DBC_SYMBOL: [%s]", YYText());
    pText = NULL;
    length = (size_t)YYLeng();
    return Token::TT_PUNCT;
}

{GBK_SBC_SYMBOL} {
    FX_DEBUG("GB_SBC_SYMBOL: [%s]", YYText());
    pText = NULL;
    length = (size_t)YYLeng();
    return Token::TT_PUNCT;
}

{GBK} {
    FX_DEBUG("GBK: [%s]", YYText());
    pText = YYText();
    length = (size_t)YYLeng();
    return Token::TT_CWORD;
}


. {
    FX_DEBUG("Invalid char : [%s]", YYText());
    pText = NULL;
    length = (size_t)YYLeng();
    return Token::TT_UNKNOWN;
}

<<EOF>> {
    pText = NULL;
    length = 0;
    return Token::TT_MAX;
}

%%
/** Additional Code */

FX_NS_USE(common);

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, GBKTokenLexer);

GBKTokenLexer::GBKTokenLexer(std::istream *input, std::ostream *output) 
    : yyFlexLexer(input, output)
{
}

GBKTokenLexer::~GBKTokenLexer() 
{
}

void GBKTokenLexer::setDebug(bool debug) 
{
    yy_flex_debug = debug;
}

#ifdef FX_WINDOWS
#pragma warning(pop)
#endif

FX_NS_END
