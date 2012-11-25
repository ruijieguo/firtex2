%{
#include <string>
#include "firtex/common/StdHeader.h"
#include "firtex/analyzer/UTF8TokenLexer.h"

/**
 * This disables inclusion of unistd.h, which is not available under Visual C++
 * on Win32. The C++ scanner uses STL streams instead. 
 */
#define YY_NO_UNISTD_H

#undef YY_DECL
#define YY_DECL firtex::analyzer::UTF8TokenLexer::TokenType     \
        firtex::analyzer::UTF8TokenLexer::lex(                  \
                const char*& pText, size_t& length)

%}

/** enable c++ scanner class generation */
%option c++
%option noyywrap

/** Change the name of the scanner class. results in "UTF8TokenFlexLexer" */
%option prefix="UTF8Token"
%option yyClass="UTF8TokenLexer"

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

CJK_1        (\xe2[\xba-\xbf][\x80-\xbf]) 
CJK_2        ([\xe3-\xe9][\x80-\xbf][\x80-\xbf])

UTF8_SBC_DIGIT    (\xef\xbc[\x90-\x99])
UTF8_SBC_ALPHA    (\xef\xbc[\xa1-\xba])|(\xef\xbd[\x81-\x9a])
UTF8_SBC_1        (\xef[\xbc-\xbe][\x80-\xbf])
UTF8_SBC_2        (\xef\xbf[\x80-\xaf])

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

{UTF8_SBC_DIGIT}+ {
    FX_DEBUG("UTF8_SBC_DIGIT: [%s]", YYText());
    pText = YYText();
    length = (size_t)YYLeng();
    return Token::TT_NUMBER;
}

{UTF8_SBC_ALPHA}+ {
    FX_DEBUG("UTF8_SBC_ALPHA: [%s]", YYText());
    pText = YYText();
    length = (size_t)YYLeng();
    return Token::TT_ALPHA;
}

("。")|{UTF8_SBC_1}|{UTF8_SBC_2} {
    FX_DEBUG("UTF8_SBC: [%s]", YYText());
    pText = NULL;
    length = (size_t)YYLeng();
    return Token::TT_PUNCT;
}

{CJK_1}|{CJK_2} {
    FX_DEBUG("CJK: [%s]", YYText());
    pText = YYText();
    length = (size_t)YYLeng();
    return Token::TT_CWORD;
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

SETUP_STREAM_LOGGER(analyzer, UTF8TokenLexer);

UTF8TokenLexer::UTF8TokenLexer(std::istream *input, std::ostream *output) 
    : yyFlexLexer(input, output)
{
}

UTF8TokenLexer::~UTF8TokenLexer() 
{
}

void UTF8TokenLexer::setDebug(bool debug) 
{
    yy_flex_debug = debug;
}

FX_NS_END
