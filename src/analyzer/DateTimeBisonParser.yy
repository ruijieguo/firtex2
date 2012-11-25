%{
#include <stdio.h>
#include <string>
#include <vector>
#include "firtex/utility/DateTime.h"
#include <limits.h>
#include <time.h>

FX_NS_DEF(analyzer);
class DateTimeLexer;
class DateTimeAnalyzer;
FX_NS_END

FX_NS_USE(analyzer);

%}

/** yacc/bison Declarations */

/* Require bison 2.3 or later */
%require "2.3"

/** use newer C++ skeleton file */
%skeleton "lalr1.cc"

/** namespace to enclose parser in */
%name-prefix="fx_bison"

/**
 * Add debug output code to generated parser. disable this for release
 * versions. 
 */
%debug

/** Start symbol is named "date_time_expr" */
%start date_time_expr

/** Write out a header file containing the token defines */
%defines

/** Set the parser's class identifier */
%define "parser_class_name" "DateTimeBisonParser"

/** Keep track of the current position within the input */
%locations

%parse-param {FX_NS(analyzer)::DateTimeLexer& lexer}
%parse-param {int64_t& result}

/** verbose error messages */
%error-verbose

%union {
    int64_t intVal;
}

%token			END	     0	"end of file"
%token <intVal> 	TOK_INT		"integer"

%token <intVal> TOK_MONTH TOK_TZD TOK_WEEK
%token TOK_SLASH TOK_DASH TOK_COLON TOK_YEAR_SEP TOK_MONTH_SEP TOK_DAY_SEP TOK_HOUR_SEP TOK_MIN_SEP TOK_SECOND_SEP

%type <intVal> date_time_expr

%{
#include "firtex/analyzer/DateTimeLexer.h"
#include "firtex/analyzer/DateTimeAnalyzer.h"

/**
 * This "connects" the bison parser to the flex scanner class  object.
 * It defines the yylex() function call to pull the next token from the
 * current lexer object. 
 */
#undef yylex
#define yylex lexer.lex
%}

%%
date_time_expr : TOK_INT TOK_DASH TOK_INT TOK_DASH TOK_INT 'T' TOK_INT TOK_COLON TOK_INT TOK_COLON TOK_INT TOK_TZD END
                 {
                     /**
                      * The date/time format defined in the ISO 8601 standard:
                      * 2008-01-01T12:00:00Z
                      */
                     if (FX_NS(utility)::DateTime::isValid($1, $3, $5, $7, $9, $11))
                     {
                         $$ = DateTimeAnalyzer::makeTime($1, $3, $5, $7, $9, $11);
                         result = $$;
                     }
                     else
                     {
                         YYERROR; 
                     }
                 }
               | TOK_WEEK ',' TOK_INT TOK_MONTH TOK_INT TOK_INT TOK_COLON TOK_INT TOK_COLON TOK_INT TOK_TZD END
                 {
                     /**
                      * The date/time format defined in RFC 1123:
                      * Sat, 1 Jan 2005 12:00:00 GMT
                      */
                     if (FX_NS(utility)::DateTime::isValid($5, $4, $3, $6, $8, $10))
                     {
                         $$ = DateTimeAnalyzer::makeTime($5, $4, $3, $6, $8, $10);
                         result = $$;
                     }
                     else
                     {
                         YYERROR; 
                     }
                 }
               | TOK_WEEK ',' TOK_INT TOK_DASH TOK_MONTH TOK_DASH TOK_INT TOK_INT TOK_COLON TOK_INT TOK_COLON TOK_INT TOK_TZD END
                 {
                     /**
                      * The date/time format defined in RFC 850 (obsoleted by RFC 1036).
                      *   Saturday, 1-Jan-05 11:00:00 GMT
                      */
                     if (FX_NS(utility)::DateTime::isValid($7, $5, $3, $8, $10, $12))
                     {
                         $$ = DateTimeAnalyzer::makeTime($7, $5, $3, $8, $10, $12);
                         result = $$;
                     }
                     else
                     {
                         YYERROR; 
                     }
                 }
               | TOK_INT TOK_SLASH TOK_INT TOK_SLASH TOK_INT TOK_INT TOK_COLON TOK_INT TOK_COLON TOK_INT END
                 {
                     /** format: 2008/1/1 12:00:00 */
                     if (FX_NS(utility)::DateTime::isValid($1, $3, $5, $6, $8, $10))
                     {
                         $$ = DateTimeAnalyzer::makeTime($1, $3, $5, $6, $8, $10);
                         result = $$;
                     }
                     else
                     {
                         YYERROR; 
                     }
                 }
               | TOK_INT TOK_DASH TOK_INT TOK_DASH TOK_INT TOK_INT TOK_COLON TOK_INT TOK_COLON TOK_INT END
               {
                   /** simple, sortable date/time format: 2008-01-01 12:00:00 */
                   if (FX_NS(utility)::DateTime::isValid($1, $3, $5, $6, $8, $10))
                   {
                       $$ = DateTimeAnalyzer::makeTime($1, $3, $5, $6, $8, $10);
                         result = $$;
                   }
                   else
                   {
                       YYERROR;
                   }
               }
               | TOK_WEEK TOK_MONTH TOK_INT TOK_INT TOK_COLON TOK_INT TOK_COLON TOK_INT TOK_INT END
                 {
                     /** 
                      * The date/time format produced by the ANSI C asctime() function:
                      * Sat Jan 01 12:00:00 2008
                      */
                     if (FX_NS(utility)::DateTime::isValid($9, $2, $3, $4, $6, $8))
                     {
                         $$ = DateTimeAnalyzer::makeTime($9, $2, $3, $4, $6, $8);
                         result = $$;
                     }
                     else
                     {
                         YYERROR; 
                     }
                 } 
               | TOK_INT TOK_YEAR_SEP TOK_INT TOK_MONTH_SEP TOK_INT TOK_DAY_SEP TOK_INT TOK_HOUR_SEP TOK_INT TOK_MIN_SEP TOK_INT TOK_SECOND_SEP END
                 {
                     /** format: 2008年1月1日 12点00分00秒 */
                     if (FX_NS(utility)::DateTime::isValid($1, $3, $5, $7, $9, $11))
                     {
                         $$ = DateTimeAnalyzer::makeTime($1, $3, $5, $7, $9, $11);
                         result = $$;
                     }
                     else
                     {
                         YYERROR; 
                     }
                 }

%% 

/** Additional Code */

void fx_bison::DateTimeBisonParser::error(
        const fx_bison::DateTimeBisonParser::location_type& l,
        const std::string& m)
{
//    exprParser.fail(l, m);
}

