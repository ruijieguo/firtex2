%{
#include <stdio.h>
#include <string>
#include <vector>
#include "firtex/queryparser/QueryExpr.h"
#include <limits.h>

FX_NS_DEF(queryparser);
class QueryExprLexer;
class QueryExprParser;
FX_NS_END

FX_NS_USE(queryparser);

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

/** Start symbol is named "query_expr" */
%start query_expr

/** Write out a header file containing the token defines */
%defines

/** Set the parser's class identifier */
%define "parser_class_name" "QueryExprBisonParser"

/** Keep track of the current position within the input */
%locations

%parse-param {FX_NS(queryparser)::QueryExprLexer& lexer}
%parse-param {FX_NS(queryparser)::QueryExprParser& exprParser}

/** verbose error messages */
%error-verbose

%union {
    FX_NS(queryparser)::NumberNode* numberVal;
    std::string* stringVal;
    FX_NS(queryparser)::QueryExpr* queryExprVal;
    FX_NS(queryparser)::TermQueryExpr* termExprVal;
}

%token			END	     0	"end of file"
%token <numberVal> 	TOK_INT		"integer"
%token <numberVal> 	TOK_FLOAT	"float"
%token <stringVal> 	TOK_IDENT       "identifier"
%token <stringVal> 	TOK_CJK         "string NOT including symbols"
%token <stringVal> 	TOK_SINGLE_QUOTED       "single quoted string"
%token <stringVal> 	TOK_DOUBLE_QUOTED	"double quoted string"

%token TOK_AND TOK_OR TOK_RANGE_TO TOK_EQ TOK_LE TOK_GE TOK_LESS TOK_GREATER TOK_MOD_NOT TOK_MOD_REQ TOK_BOOST TOK_PHRASE_SLOP TOK_QUERY_ANY

%type <queryExprVal> query_expr bool_expr expr and_expr or_expr any_expr
%type <queryExprVal> group_expr req_expr proh_expr group_term_expr term_list
%type <termExprVal> atom_expr
%type <stringVal> field_name range_val
%type <termExprVal> atom_term term_expr phrase_expr range_expr boost_term

%destructor {delete $$;} TOK_INT TOK_FLOAT
%destructor {delete $$;} TOK_IDENT TOK_CJK  TOK_SINGLE_QUOTED TOK_DOUBLE_QUOTED
%destructor {delete $$;} expr bool_expr and_expr or_expr any_expr
%destructor {delete $$;} group_expr req_expr proh_expr
%destructor {delete $$;} atom_expr
%destructor {delete $$;} field_name range_val
%destructor {delete $$;} boost_term atom_term term_expr phrase_expr range_expr

%{
#include "firtex/queryparser/QueryExprLexer.h"
#include "firtex/queryparser/QueryExprParser.h"

/**
 * This "connects" the bison parser to the flex scanner class  object.
 * It defines the yylex() function call to pull the next token from the
 * current lexer object. 
 */
#undef yylex
#define yylex lexer.lex
%}

%left TOK_OR;
%left TOK_AND;
%right TOK_MOD_REQ
%right TOK_MOD_NOT

%%

query_expr : bool_expr END 
{ 
    $$ = $1;
    exprParser.finalize($$);
}

bool_expr : expr
          | bool_expr expr 
            {
                $$ = new BooleanQueryExpr($1, $2, exprParser.getDefaultOp()); 
            }

expr : atom_expr {$$ = $1;}
     | any_expr
     | and_expr
     | or_expr
     | group_expr 
     | req_expr
     | proh_expr
     | group_term_expr

group_expr : '(' bool_expr ')' 
{
    $$ = $2;
}

req_expr: TOK_MOD_REQ expr
{
    $2->setRequired(true);
    $$ = $2;
}

proh_expr: TOK_MOD_NOT expr
{
    $2->setProhibited(true);
    $$ = $2;
}

any_expr: TOK_QUERY_ANY
          {
              $$ = new AnyQueryExpr();
          }

and_expr : expr TOK_AND expr 
{
    $$ = new BooleanQueryExpr($1, $3, QueryExpr::OP_AND);
}

or_expr : expr TOK_OR expr 
{
    $$ = new BooleanQueryExpr($1, $3, QueryExpr::OP_OR);
}

group_term_expr : field_name '(' term_list ')'
                  {
                      $3->setFieldName(*$1);
                      delete $1;
                      $$ = $3;
                  }

term_list : expr
            {
                $$ = $1;
            }
          | term_list expr
            {
                $$ = new BooleanQueryExpr($1, $2, exprParser.getDefaultOp());
            }

atom_expr : boost_term
            {
                const std::string &defaultField = exprParser.getDefaultField();
                $1->setFieldName(defaultField);
                $$ = $1;
              }
          | field_name boost_term 
            {
                std::string* fieldName = $1;
                $2->setFieldName(*fieldName);
                delete $1;
                $$ = $2;
            }

boost_term : atom_term
             {
                 $$ = $1;
             }
            | atom_term TOK_BOOST TOK_INT
             {
                 $1->setBoost((float)$3->m_i64Val);
                 delete $3;
                 $$ = $1;
             }
            | atom_term TOK_BOOST TOK_FLOAT
             {
                 $1->setBoost($3->m_fVal);
                 delete $3;
                 $$ = $1;
             }

atom_term : term_expr 
          | phrase_expr 
          | range_expr 
            { $$ = $1; }

term_expr : TOK_IDENT
             { $$ = new TermQueryExpr($1); }
           | TOK_CJK
             { $$ = new TermQueryExpr($1); }
           | TOK_SINGLE_QUOTED
             { $$ = new TermQueryExpr($1); }

phrase_expr : TOK_DOUBLE_QUOTED 
              {
                  $$ = new PhraseQueryExpr($1); 
              }
            | TOK_DOUBLE_QUOTED TOK_PHRASE_SLOP TOK_INT
            {
                $$ = new PhraseQueryExpr($1, (int32_t)$3->m_i64Val); 
                delete $3;
            }

range_expr : TOK_INT
              { 
                  $$ = new TermQueryExpr(new std::string($1->m_sValue)); 
                  delete $1;
              }
            | TOK_FLOAT
              {
                  $$ = new TermQueryExpr(new std::string($1->m_sValue)); 
                  delete $1;
              }
            | TOK_EQ range_val 
              { $$ = new TermQueryExpr($2); }
            | TOK_LESS range_val
              { $$ = new RangeQueryExpr(NULL, false, $2, false);}
            | TOK_LE range_val
              { $$ = new RangeQueryExpr(NULL, false, $2, true);}
            | TOK_GREATER range_val
              { $$ = new RangeQueryExpr($2, false, NULL, false);}
            | TOK_GE range_val
              { $$ = new RangeQueryExpr($2, true, NULL, false);}
            | '[' range_val TOK_RANGE_TO range_val ']'
              { 
                  $$ = new RangeQueryExpr($2, true, $4, true);
              }
            | '[' range_val TOK_RANGE_TO range_val '}'
              { 
                  $$ = new RangeQueryExpr($2, true, $4, false);
              }
            | '{' range_val TOK_RANGE_TO range_val '}'
              {
                  $$ = new RangeQueryExpr($2, false, $4, false);
              }
            | '{' range_val TOK_RANGE_TO range_val ']'
              { 
                  $$ = new RangeQueryExpr($2, false, $4, true);
              }

range_val : TOK_INT
              {
                  $$ = new std::string($1->m_sValue);
                  delete $1;
              }
            | TOK_FLOAT
              {
                  $$ = new std::string($1->m_sValue);
                  delete $1;
              }
            | TOK_IDENT
            | TOK_CJK
            | TOK_SINGLE_QUOTED
              {
                  $$ = $1;
              }

field_name : TOK_IDENT ':'  
              { 
                  $$ = $1;
              }

%% 

/** Additional Code */
void fx_bison::QueryExprBisonParser::error(
        const fx_bison::QueryExprBisonParser::location_type& l,
        const std::string& m)
{
    exprParser.fail(l, m);
}
