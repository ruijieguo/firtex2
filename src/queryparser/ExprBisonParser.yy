%{
#include "firtex/common/StdHeader.h"
#include "firtex/queryparser/ExprNode.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <limits.h>

FX_NS_DEF(queryparser);
class ExprLexer;
class ExprParser;
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
%start exprline

/** Write out a header file containing the token defines */
%defines

/** Set the parser's class identifier */
%define "parser_class_name" "ExprBisonParser"

/** Keep track of the current position within the input */
%locations

%parse-param {FX_NS(queryparser)::ExprLexer& lexer}
%parse-param {FX_NS(queryparser)::ExprParser& exprParser}

/** verbose error messages */
%error-verbose

%union 
{
    int64_t intVal;
    double  doubleVal;
    int32_t funcIdx;
    int32_t nodeIdx;
    std::string* stringVal;
}

%token			END	     0	    "end of file"
%token <intVal>         TOK_INT
%token <doubleVal>	TOK_DOUBLE
%token <stringVal> 	TOK_IDENT           "identifier"
%token <stringVal> 	TOK_QUOTED_STRING   "double quoted string"
%token <funcIdx>	TOK_FUNC
%token <funcIdx>	TOK_FUNC_IN
%token <funcIdx>	TOK_FUNC_DATE

%token TOK_AND TOK_OR TOK_NOT TOK_EQ TOK_LTE TOK_GTE TOK_NE TOK_NEG TOK_CONST_LIST

%type <nodeIdx>		attr
%type <nodeIdx>		expr
%type <nodeIdx>		arglist
%type <nodeIdx>		constlist
%type <nodeIdx>		function
%type <nodeIdx>		exprline

%destructor {delete $$;} TOK_IDENT TOK_QUOTED_STRING

%{
#include "firtex/queryparser/ExprLexer.h"
#include "firtex/queryparser/ExprParser.h"

/**
 * This "connects" the bison parser to the flex scanner class  object.
 * It defines the yylex() function call to pull the next token from the
 * current lexer object. 
 */
#undef yylex
#define yylex lexer.lex
%}

%left TOK_OR
%left TOK_AND
%left '|'
%left '&'
%left TOK_EQ TOK_NE
%left '<' '>' TOK_LTE TOK_GTE
%left '+' '-'
%left '*' '/'
%nonassoc TOK_NOT
%nonassoc TOK_NEG

%%

exprline: expr END
          { 
              $$ = $1;              
              exprParser.finalize($1); 
          }

attr: TOK_IDENT			
        {
            $$ = exprParser.addAttrNode(*$1); 
            delete $1;
            if ($$ < 0)
            {
                YYERROR;
            }
        }
	| TOK_QUOTED_STRING
          {
              $$ = exprParser.addAttrNode(*$1); 
              delete $1;
              if ($$ < 0)
              {
                  YYERROR;
              }
          }
	;

expr:
	attr
	| function
	| TOK_INT			{ $$ = exprParser.addIntNode($1); }
        | TOK_DOUBLE		        { $$ = exprParser.addFloatNode($1); }
        | '-' expr %prec TOK_NEG	{ $$ = exprParser.addOpNode(ExprNode::TOK_NEG, $2, -1); }
	| TOK_NOT expr
          {
              $$ = exprParser.addOpNode(ExprNode::TOK_NOT, $2, -1);
              if ($$ < 0) 
              {
                  YYERROR; 
              }
          }
        | expr '+' expr			{ $$ = exprParser.addOpNode(ExprNode::TOK_PLUS, $1, $3 ); }
	| expr '-' expr			{ $$ = exprParser.addOpNode(ExprNode::TOK_MINUS, $1, $3 ); }
	| expr '*' expr			{ $$ = exprParser.addOpNode(ExprNode::TOK_MUL, $1, $3 ); }
	| expr '/' expr			{ $$ = exprParser.addOpNode(ExprNode::TOK_DIV, $1, $3 ); }
	| expr '<' expr			{ $$ = exprParser.addOpNode(ExprNode::TOK_LE, $1, $3 ); }
	| expr '>' expr			{ $$ = exprParser.addOpNode(ExprNode::TOK_GR, $1, $3 ); }
	| expr '&' expr			{ $$ = exprParser.addOpNode(ExprNode::TOK_BIT_AND, $1, $3 ); }
	| expr '|' expr			{ $$ = exprParser.addOpNode(ExprNode::TOK_BIT_OR, $1, $3 ); }
        | expr TOK_LTE expr		{ $$ = exprParser.addOpNode(ExprNode::TOK_LTE, $1, $3 ); }
        | expr TOK_GTE expr		{ $$ = exprParser.addOpNode(ExprNode::TOK_GTE, $1, $3 ); }
	| expr TOK_EQ expr		{ $$ = exprParser.addOpNode(ExprNode::TOK_EQ, $1, $3 ); }
	| expr TOK_NE expr		{ $$ = exprParser.addOpNode(ExprNode::TOK_NE, $1, $3 ); }
	| expr TOK_AND expr		{ $$ = exprParser.addOpNode(ExprNode::TOK_AND, $1, $3); }
	| expr TOK_OR expr              { $$ = exprParser.addOpNode(ExprNode::TOK_OR, $1, $3); }
	| '(' expr ')'			{ $$ = $2; }
	;

arglist:
	expr				{ $$ = $1; }
        | arglist ',' expr		{ $$ = exprParser.addOpNode(ExprNode::TOK_COMMA, $1, $3); }
	;

constlist:
	TOK_INT			        { $$ = exprParser.addConstListNode($1); }
        | TOK_DOUBLE		        { $$ = exprParser.addConstListNode($1); }
	| constlist ',' TOK_INT	        { exprParser.appendToConstList($$, $3); }
        | constlist ',' TOK_DOUBLE	{ exprParser.appendToConstList($$, $3); }
	;

function:
	TOK_FUNC '(' arglist ')'
          { 
              $$ = exprParser.addFuncNode($1, $3);
              if ($$<0)
              {
                  YYERROR; 
              }
          }
	| TOK_FUNC '(' ')'	
          {
              $$ = exprParser.addFuncNode($1, -1); 
              if ($$ < 0)
              {
                  YYERROR;
              }
          }
	| TOK_FUNC_IN '(' attr ',' constlist ')'
	  {
              $$ = exprParser.addFuncNode($1, $3, $5);
          }
	| TOK_FUNC_DATE '(' TOK_IDENT ')'
	  {
              $$ = exprParser.addDateFuncNode($1, *$3);
          }
	| TOK_FUNC_DATE '(' TOK_QUOTED_STRING ')'
	  {
              $$ = exprParser.addDateFuncNode($1, *$3);
          }
	;

%% 

/** Additional Code */
void fx_bison::ExprBisonParser::error(
        const fx_bison::ExprBisonParser::location_type& l,
        const std::string& m)
{
    exprParser.fail(l, m);
}
