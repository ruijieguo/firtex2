%{
#include <string>
#include "firtex/common/StdHeader.h"
#include "firtex/analyzer/DateTimeLexer.h"

using namespace fx_bison;

typedef DateTimeBisonParser::token token;
typedef DateTimeBisonParser::token_type token_type;

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
#define	YY_DECL	token_type firtex::analyzer::DateTimeLexer::lex(   \
            semantic_type* yylval, location_type* yylloc)
%}

/** enable c++ scanner class generation */
%option c++
%option noyywrap

/** Change the name of the scanner class. results in "DateTimeFlexLexer" */
%option prefix="DateTime"
%option yyClass="DateTimeLexer"

/* The manual says "somewhat more optimized" */
%option batch

/**
 * Enable scanner to generate debug output. disable this for release
 * versions. 
 */
%option debug

DIGIT        [0-9] 
WHITESPACE   [ \t\f\u3000]
NUM          ({DIGIT})+

%%

{NUM} {
    FX_TRACE("NUM: [%s]", YYText());
    yylval->intVal = strtol(YYText(), NULL, 10);
    return token::TOK_INT;
}

"/" {
    FX_TRACE("[%s]", YYText());
    return token::TOK_SLASH;
} 

"-" {
    FX_TRACE("[%s]", YYText());
    return token::TOK_DASH;
} 

"\:" {
    FX_TRACE("[%s]", YYText());
    return token::TOK_COLON;
}

"年" {
    FX_TRACE("[%s]", YYText());
    return token::TOK_YEAR_SEP;
}

"\xC4\xEA" {
    FX_TRACE("[%s]", YYText());
    return token::TOK_YEAR_SEP;
}

"月" {
    FX_TRACE("[%s]", YYText());
    return token::TOK_MONTH_SEP;
}

"\xD4\xC2" {
    FX_TRACE("[%s]", YYText());
    return token::TOK_MONTH_SEP;
}


"日" {
    FX_TRACE("[%s]", YYText());
    return token::TOK_DAY_SEP;
}

"\xC8\xD5" {
    FX_TRACE("[%s]", YYText());
    return token::TOK_DAY_SEP;
}

"点" {
    FX_TRACE("[%s]", YYText());
    return token::TOK_HOUR_SEP;
}

"\xB5\xE3" {
    FX_TRACE("[%s]", YYText());
    return token::TOK_HOUR_SEP;
}

"分" {
    FX_TRACE("[%s]", YYText());
    return token::TOK_MIN_SEP;
}

"\xB7\xD6" {
    FX_TRACE("[%s]", YYText());
    return token::TOK_MIN_SEP;
}

"秒" {
    FX_TRACE("[%s]", YYText());
    return token::TOK_SECOND_SEP;
}

"\xC3\xEB" {
    FX_TRACE("[%s]", YYText());
    return token::TOK_SECOND_SEP;
}

"Z" {
    yylval->intVal = 0;
    return token::TOK_TZD;
}
"UT" {
    yylval->intVal = 0;
    return token::TOK_TZD;
}
"GMT" {
    yylval->intVal = 0;
    return token::TOK_TZD;
}
"+0000" {
    yylval->intVal = 0;
    return token::TOK_TZD;
}
"BST" {
    yylval->intVal = 1*3600;
    return token::TOK_TZD;
}
"IST" {
    yylval->intVal = 1*3600;
    return token::TOK_TZD;
}
"WET" {
    yylval->intVal = 0;
    return token::TOK_TZD;
}
"WEST" {
    yylval->intVal = 1*3600;
    return token::TOK_TZD;
}
"CET" {
    yylval->intVal = 1*3600;
    return token::TOK_TZD;
}
"CEST" {
    yylval->intVal = 2*3600;
    return token::TOK_TZD;
}
"EET" {
    yylval->intVal = 2*3600;
    return token::TOK_TZD;
}
"EEST" {
    yylval->intVal = 3*3600;
    return token::TOK_TZD;
}
"MSK" {
    yylval->intVal = 3*3600;
    return token::TOK_TZD;
}
"MSD" {
    yylval->intVal = 4*3600;
    return token::TOK_TZD;
}
"NST" {
    yylval->intVal = -3*3600-1800;
    return token::TOK_TZD;
}
"NDT" {
    yylval->intVal = -2*3600-1800;
    return token::TOK_TZD;
}
"AST" {
    yylval->intVal = -4*3600;
    return token::TOK_TZD;
}
"ADT" {
    yylval->intVal = -3*3600;
    return token::TOK_TZD;
}
"EST" {
    yylval->intVal = -5*3600;
    return token::TOK_TZD;
}
"EDT" {
    yylval->intVal = -4*3600;
    return token::TOK_TZD;
}
"CST" {
    yylval->intVal = -6*3600;
    return token::TOK_TZD;
}
"CDT" {
    yylval->intVal = -5*3600;
    return token::TOK_TZD;
}
"MST" {
    yylval->intVal = -7*3600;
    return token::TOK_TZD;
}
"MDT" {
    yylval->intVal = -6*3600;
    return token::TOK_TZD;
}
"PST" {
    yylval->intVal = -8*3600;
    return token::TOK_TZD;
}
"PDT" {
    yylval->intVal = -7*3600;
    return token::TOK_TZD;
}
"AKST" {
    yylval->intVal = -9*3600;
    return token::TOK_TZD;
}
"AKDT" {
    yylval->intVal = -8*3600;
    return token::TOK_TZD;
}
"HST" {
    yylval->intVal = -10*3600;
    return token::TOK_TZD;
}
"AEST" {
    yylval->intVal = 10*3600;
    return token::TOK_TZD;
}
"AEDT" {
    yylval->intVal = 11*3600;
    return token::TOK_TZD;
}
"ACST" {
    yylval->intVal = 9*3600+1800;
    return token::TOK_TZD;
}
"ACDT" {
    yylval->intVal = 10*3600+1800;
    return token::TOK_TZD;
}
"AWST" {
    yylval->intVal = 8*3600;
    return token::TOK_TZD;
}
"AWDT" {
    yylval->intVal = 9*3600;
    return token::TOK_TZD;
}

"Sunday" {
    yylval->intVal = 0;
    return token::TOK_WEEK;
}

"Sun" {
    yylval->intVal = 0;
    return token::TOK_WEEK;
}
 
"Monday" {
    yylval->intVal = 1;
    return token::TOK_WEEK;
}

"Mon" {
    yylval->intVal = 1;
    return token::TOK_WEEK;
}
 
"Tuesday" {
    yylval->intVal = 2;
    return token::TOK_WEEK;
}

"Tue" {
    yylval->intVal = 2;
    return token::TOK_WEEK;
}
 
"Wednesday" {
    yylval->intVal = 3;
    return token::TOK_WEEK;
}

"Wed" {
    yylval->intVal = 3;
    return token::TOK_WEEK;
}
 
"Thursday" {
    yylval->intVal = 4;
    return token::TOK_WEEK;
}

"Thu" {
    yylval->intVal = 4;
    return token::TOK_WEEK;
}
  
"Friday" {
    yylval->intVal = 5;
    return token::TOK_WEEK;
}

"Fri" {
    yylval->intVal = 5;
    return token::TOK_WEEK;
}
 
"Saturday" {
    yylval->intVal = 6;
    return token::TOK_WEEK;
}

"Sat" {
    yylval->intVal = 6;
    return token::TOK_WEEK;
}


"January" {
    yylval->intVal = 1;
    return token::TOK_MONTH;
}

"Jan" {
    yylval->intVal = 1;
    return token::TOK_MONTH;
}
 
"February" {
        yylval->intVal = 2;
    return token::TOK_MONTH;
}

"Feb" {
        yylval->intVal = 2;
    return token::TOK_MONTH;
}
 
"March" {
    yylval->intVal = 3;
    return token::TOK_MONTH;
}

"Mar" {
    yylval->intVal = 3;
    return token::TOK_MONTH;
}
 
"April" {
    yylval->intVal = 4;
    return token::TOK_MONTH;
}

"Apr" {
    yylval->intVal = 4;
    return token::TOK_MONTH;
}
 
"May" {
    yylval->intVal = 5;
    return token::TOK_MONTH;
}
 
"June" {
    yylval->intVal = 6;
    return token::TOK_MONTH;
}

"Jun" {
    yylval->intVal = 6;
    return token::TOK_MONTH;
}
  
"July" {
    yylval->intVal = 7;    
    return token::TOK_MONTH;
}

"Jul" {
    yylval->intVal = 7;    
    return token::TOK_MONTH;
}
 
"August" {
    yylval->intVal = 8;
    return token::TOK_MONTH;
}

"Aug" {
    yylval->intVal = 8;
    return token::TOK_MONTH;
}
 
"September" {
    yylval->intVal = 9;
    return token::TOK_MONTH;
}

"Sep" {
    yylval->intVal = 9;
    return token::TOK_MONTH;
}
  
"October" {
    yylval->intVal = 10;
    return token::TOK_MONTH;
}

"Oct" {
    yylval->intVal = 10;
    return token::TOK_MONTH;
}
 
"November" {
    yylval->intVal = 11;
    return token::TOK_MONTH;
}

"Nov" {
    yylval->intVal = 11;
    return token::TOK_MONTH;
}
 
"December" {
    yylval->intVal = 12;
    return token::TOK_MONTH;
}

"Dec" {
    yylval->intVal = 12;
    return token::TOK_MONTH;
}

{WHITESPACE} { }

. {
    FX_TRACE("[%s]", YYText());
    return (token_type)YYText()[0];
}

<<EOF>> {
    return token::END;
}

%%
/** Additional Code */

FX_NS_USE(common);

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, DateTimeLexer);

DateTimeLexer::DateTimeLexer(std::istream *input, std::ostream *output) 
    : yyFlexLexer(input, output)
{
}

DateTimeLexer::~DateTimeLexer() 
{
}

void DateTimeLexer::setDebug(bool debug) 
{
    yy_flex_debug = debug;
}

FX_NS_END
