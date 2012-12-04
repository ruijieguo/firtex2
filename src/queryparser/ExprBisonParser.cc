
/* A Bison parser, made by GNU Bison 2.4.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008 Free Software
   Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

// Take the name prefix into account.
#define yylex   fx_bisonlex

/* First part of user declarations.  */

/* Line 311 of lalr1.cc  */
#line 1 "ExprBisonParser.yy"

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



/* Line 311 of lalr1.cc  */
#line 61 "ExprBisonParser.cc"


#include "y.tab.h"

/* User implementation prologue.  */

/* Line 317 of lalr1.cc  */
#line 82 "ExprBisonParser.yy"

#include "firtex/queryparser/ExprLexer.h"
#include "firtex/queryparser/ExprParser.h"

/**
 * This "connects" the bison parser to the flex scanner class  object.
 * It defines the yylex() function call to pull the next token from the
 * current lexer object. 
 */
#undef yylex
#define yylex lexer.lex


/* Line 317 of lalr1.cc  */
#line 84 "ExprBisonParser.cc"

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


/* Line 380 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

namespace fx_bison {

/* Line 380 of lalr1.cc  */
#line 153 "ExprBisonParser.cc"
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  ExprBisonParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  ExprBisonParser::ExprBisonParser (FX_NS(queryparser)::ExprLexer& lexer_yyarg, FX_NS(queryparser)::ExprParser& exprParser_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      lexer (lexer_yyarg),
      exprParser (exprParser_yyarg)
  {
  }

  ExprBisonParser::~ExprBisonParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  ExprBisonParser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  ExprBisonParser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  ExprBisonParser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
        case 5: /* "\"identifier\"" */

/* Line 480 of lalr1.cc  */
#line 80 "ExprBisonParser.yy"
	{delete (yyvaluep->stringVal);};

/* Line 480 of lalr1.cc  */
#line 261 "ExprBisonParser.cc"
	break;
      case 6: /* "\"double quoted string\"" */

/* Line 480 of lalr1.cc  */
#line 80 "ExprBisonParser.yy"
	{delete (yyvaluep->stringVal);};

/* Line 480 of lalr1.cc  */
#line 270 "ExprBisonParser.cc"
	break;

	default:
	  break;
      }
  }

  void
  ExprBisonParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  ExprBisonParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  ExprBisonParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  ExprBisonParser::debug_level_type
  ExprBisonParser::debug_level () const
  {
    return yydebug_;
  }

  void
  ExprBisonParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  int
  ExprBisonParser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 2:

/* Line 678 of lalr1.cc  */
#line 109 "ExprBisonParser.yy"
    { 
              (yyval.nodeIdx) = (yysemantic_stack_[(2) - (1)].nodeIdx);              
              exprParser.finalize((yysemantic_stack_[(2) - (1)].nodeIdx)); 
          }
    break;

  case 3:

/* Line 678 of lalr1.cc  */
#line 115 "ExprBisonParser.yy"
    {
            (yyval.nodeIdx) = exprParser.addAttrNode(*(yysemantic_stack_[(1) - (1)].stringVal)); 
            delete (yysemantic_stack_[(1) - (1)].stringVal);
            if ((yyval.nodeIdx) < 0)
            {
                YYERROR;
            }
        }
    break;

  case 4:

/* Line 678 of lalr1.cc  */
#line 124 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addAttrNode(*(yysemantic_stack_[(1) - (1)].stringVal)); 
              delete (yysemantic_stack_[(1) - (1)].stringVal);
              if ((yyval.nodeIdx) < 0)
              {
                  YYERROR;
              }
          }
    break;

  case 7:

/* Line 678 of lalr1.cc  */
#line 137 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addIntNode((yysemantic_stack_[(1) - (1)].intVal)); }
    break;

  case 8:

/* Line 678 of lalr1.cc  */
#line 138 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addFloatNode((yysemantic_stack_[(1) - (1)].doubleVal)); }
    break;

  case 9:

/* Line 678 of lalr1.cc  */
#line 139 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_NEG, (yysemantic_stack_[(2) - (2)].nodeIdx), -1); }
    break;

  case 10:

/* Line 678 of lalr1.cc  */
#line 141 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_NOT, (yysemantic_stack_[(2) - (2)].nodeIdx), -1);
              if ((yyval.nodeIdx) < 0) 
              {
                  YYERROR; 
              }
          }
    break;

  case 11:

/* Line 678 of lalr1.cc  */
#line 148 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_PLUS, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 12:

/* Line 678 of lalr1.cc  */
#line 149 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_MINUS, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 13:

/* Line 678 of lalr1.cc  */
#line 150 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_MUL, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 14:

/* Line 678 of lalr1.cc  */
#line 151 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_DIV, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 15:

/* Line 678 of lalr1.cc  */
#line 152 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_LE, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 16:

/* Line 678 of lalr1.cc  */
#line 153 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_GR, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 17:

/* Line 678 of lalr1.cc  */
#line 154 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_BIT_AND, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 18:

/* Line 678 of lalr1.cc  */
#line 155 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_BIT_OR, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 19:

/* Line 678 of lalr1.cc  */
#line 156 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_LTE, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 20:

/* Line 678 of lalr1.cc  */
#line 157 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_GTE, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 21:

/* Line 678 of lalr1.cc  */
#line 158 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_EQ, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 22:

/* Line 678 of lalr1.cc  */
#line 159 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_NE, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 23:

/* Line 678 of lalr1.cc  */
#line 160 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_AND, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx)); }
    break;

  case 24:

/* Line 678 of lalr1.cc  */
#line 161 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_OR, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx)); }
    break;

  case 25:

/* Line 678 of lalr1.cc  */
#line 162 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = (yysemantic_stack_[(3) - (2)].nodeIdx); }
    break;

  case 26:

/* Line 678 of lalr1.cc  */
#line 166 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = (yysemantic_stack_[(1) - (1)].nodeIdx); }
    break;

  case 27:

/* Line 678 of lalr1.cc  */
#line 167 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_COMMA, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx)); }
    break;

  case 28:

/* Line 678 of lalr1.cc  */
#line 171 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addConstListNode((yysemantic_stack_[(1) - (1)].intVal)); }
    break;

  case 29:

/* Line 678 of lalr1.cc  */
#line 172 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addConstListNode((yysemantic_stack_[(1) - (1)].doubleVal)); }
    break;

  case 30:

/* Line 678 of lalr1.cc  */
#line 173 "ExprBisonParser.yy"
    { exprParser.appendToConstList((yyval.nodeIdx), (yysemantic_stack_[(3) - (3)].intVal)); }
    break;

  case 31:

/* Line 678 of lalr1.cc  */
#line 174 "ExprBisonParser.yy"
    { exprParser.appendToConstList((yyval.nodeIdx), (yysemantic_stack_[(3) - (3)].doubleVal)); }
    break;

  case 32:

/* Line 678 of lalr1.cc  */
#line 179 "ExprBisonParser.yy"
    { 
              (yyval.nodeIdx) = exprParser.addFuncNode((yysemantic_stack_[(4) - (1)].funcIdx), (yysemantic_stack_[(4) - (3)].nodeIdx));
              if ((yyval.nodeIdx)<0)
              {
                  YYERROR; 
              }
          }
    break;

  case 33:

/* Line 678 of lalr1.cc  */
#line 187 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addFuncNode((yysemantic_stack_[(3) - (1)].funcIdx), -1); 
              if ((yyval.nodeIdx) < 0)
              {
                  YYERROR;
              }
          }
    break;

  case 34:

/* Line 678 of lalr1.cc  */
#line 195 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addFuncNode((yysemantic_stack_[(6) - (1)].funcIdx), (yysemantic_stack_[(6) - (3)].nodeIdx), (yysemantic_stack_[(6) - (5)].nodeIdx));
          }
    break;

  case 35:

/* Line 678 of lalr1.cc  */
#line 199 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addDateFuncNode((yysemantic_stack_[(4) - (1)].funcIdx), *(yysemantic_stack_[(4) - (3)].stringVal));
          }
    break;

  case 36:

/* Line 678 of lalr1.cc  */
#line 203 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addDateFuncNode((yysemantic_stack_[(4) - (1)].funcIdx), *(yysemantic_stack_[(4) - (3)].stringVal));
          }
    break;



/* Line 678 of lalr1.cc  */
#line 735 "ExprBisonParser.cc"
	default:
          break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[0] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  ExprBisonParser::yysyntax_error_ (int yystate, int tok)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char ExprBisonParser::yypact_ninf_ = -21;
  const short int
  ExprBisonParser::yypact_[] =
  {
        67,   -21,   -21,   -21,   -21,   -13,     7,    18,    67,    67,
      67,    50,   -21,    33,   -21,    57,     3,     5,   -21,   -21,
      82,   -21,   -21,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,   -21,   101,   -16,
      51,    60,    61,   -21,    16,   118,   159,   -20,   -20,   159,
     132,   146,   -20,   -20,    42,    42,   -21,   -21,   -21,    67,
      74,   -21,   -21,   101,   -21,   -21,    54,   -21,    83,   -21,
     -21
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  ExprBisonParser::yydefact_[] =
  {
         0,     7,     8,     3,     4,     0,     0,     0,     0,     0,
       0,     0,     5,     0,     6,     0,     0,     0,    10,     9,
       0,     1,     2,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    33,    26,     0,
       0,     0,     0,    25,    23,    24,    21,    19,    20,    22,
      18,    17,    15,    16,    11,    12,    13,    14,    32,     0,
       0,    35,    36,    27,    28,    29,     0,    34,     0,    30,
      31
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  ExprBisonParser::yypgoto_[] =
  {
       -21,   -21,    84,    -8,   -21,   -21,   -21
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  ExprBisonParser::yydefgoto_[] =
  {
        -1,    11,    12,    13,    39,    66,    14
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char ExprBisonParser::yytable_ninf_ = -1;
  const unsigned char
  ExprBisonParser::yytable_[] =
  {
        18,    19,    20,    33,    34,    35,    36,    38,     3,     4,
      41,    42,    58,    59,    15,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    25,
      26,    27,    28,    22,    16,    29,    30,    31,    32,    33,
      34,    35,    36,    23,    24,    17,    25,    26,    27,    28,
      21,    63,    29,    30,    31,    32,    33,    34,    35,    36,
       1,     2,     3,     4,     5,     6,     7,    35,    36,     8,
       1,     2,     3,     4,     5,     6,     7,    64,    65,     8,
      60,     9,    67,    68,    10,    37,    69,    70,    61,    62,
       0,     9,    23,    24,    10,    25,    26,    27,    28,     0,
      40,    29,    30,    31,    32,    33,    34,    35,    36,     0,
      43,    23,    24,     0,    25,    26,    27,    28,     0,     0,
      29,    30,    31,    32,    33,    34,    35,    36,    23,     0,
       0,    25,    26,    27,    28,     0,     0,    29,    30,    31,
      32,    33,    34,    35,    36,    25,    26,    27,    28,     0,
       0,     0,    30,    31,    32,    33,    34,    35,    36,    25,
      26,    27,    28,     0,     0,     0,     0,    31,    32,    33,
      34,    35,    36,    26,    27,     0,     0,     0,     0,     0,
      31,    32,    33,    34,    35,    36
  };

  /* YYCHECK.  */
  const signed char
  ExprBisonParser::yycheck_[] =
  {
         8,     9,    10,    23,    24,    25,    26,    15,     5,     6,
       5,     6,    28,    29,    27,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    13,
      14,    15,    16,     0,    27,    19,    20,    21,    22,    23,
      24,    25,    26,    10,    11,    27,    13,    14,    15,    16,
       0,    59,    19,    20,    21,    22,    23,    24,    25,    26,
       3,     4,     5,     6,     7,     8,     9,    25,    26,    12,
       3,     4,     5,     6,     7,     8,     9,     3,     4,    12,
      29,    24,    28,    29,    27,    28,     3,     4,    28,    28,
      -1,    24,    10,    11,    27,    13,    14,    15,    16,    -1,
      16,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      28,    10,    11,    -1,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    10,    -1,
      -1,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    13,    14,    15,    16,    -1,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    13,
      14,    15,    16,    -1,    -1,    -1,    -1,    21,    22,    23,
      24,    25,    26,    14,    15,    -1,    -1,    -1,    -1,    -1,
      21,    22,    23,    24,    25,    26
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  ExprBisonParser::yystos_[] =
  {
         0,     3,     4,     5,     6,     7,     8,     9,    12,    24,
      27,    31,    32,    33,    36,    27,    27,    27,    33,    33,
      33,     0,     0,    10,    11,    13,    14,    15,    16,    19,
      20,    21,    22,    23,    24,    25,    26,    28,    33,    34,
      32,     5,     6,    28,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    28,    29,
      29,    28,    28,    33,     3,     4,    35,    28,    29,     3,
       4
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  ExprBisonParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   124,
      38,    60,    62,    43,    45,    42,    47,    40,    41,    44
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  ExprBisonParser::yyr1_[] =
  {
         0,    30,    31,    32,    32,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    34,    34,    35,    35,
      35,    35,    36,    36,    36,    36,    36
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  ExprBisonParser::yyr2_[] =
  {
         0,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     3,     1,     1,
       3,     3,     4,     3,     6,     4,     4
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const ExprBisonParser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "TOK_INT", "TOK_DOUBLE",
  "\"identifier\"", "\"double quoted string\"", "TOK_FUNC", "TOK_FUNC_IN",
  "TOK_FUNC_DATE", "TOK_AND", "TOK_OR", "TOK_NOT", "TOK_EQ", "TOK_LTE",
  "TOK_GTE", "TOK_NE", "TOK_NEG", "TOK_CONST_LIST", "'|'", "'&'", "'<'",
  "'>'", "'+'", "'-'", "'*'", "'/'", "'('", "')'", "','", "$accept",
  "exprline", "attr", "expr", "arglist", "constlist", "function", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const ExprBisonParser::rhs_number_type
  ExprBisonParser::yyrhs_[] =
  {
        31,     0,    -1,    33,     0,    -1,     5,    -1,     6,    -1,
      32,    -1,    36,    -1,     3,    -1,     4,    -1,    24,    33,
      -1,    12,    33,    -1,    33,    23,    33,    -1,    33,    24,
      33,    -1,    33,    25,    33,    -1,    33,    26,    33,    -1,
      33,    21,    33,    -1,    33,    22,    33,    -1,    33,    20,
      33,    -1,    33,    19,    33,    -1,    33,    14,    33,    -1,
      33,    15,    33,    -1,    33,    13,    33,    -1,    33,    16,
      33,    -1,    33,    10,    33,    -1,    33,    11,    33,    -1,
      27,    33,    28,    -1,    33,    -1,    34,    29,    33,    -1,
       3,    -1,     4,    -1,    35,    29,     3,    -1,    35,    29,
       4,    -1,     7,    27,    34,    28,    -1,     7,    27,    28,
      -1,     8,    27,    32,    29,    35,    28,    -1,     9,    27,
       5,    28,    -1,     9,    27,     6,    28,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  ExprBisonParser::yyprhs_[] =
  {
         0,     0,     3,     6,     8,    10,    12,    14,    16,    18,
      21,    24,    28,    32,    36,    40,    44,    48,    52,    56,
      60,    64,    68,    72,    76,    80,    84,    86,    90,    92,
      94,    98,   102,   107,   111,   118,   123
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  ExprBisonParser::yyrline_[] =
  {
         0,   108,   108,   114,   123,   135,   136,   137,   138,   139,
     140,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   166,   167,   171,   172,
     173,   174,   178,   186,   194,   198,   202
  };

  // Print the state stack on the debug stream.
  void
  ExprBisonParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  ExprBisonParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  ExprBisonParser::token_number_type
  ExprBisonParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    20,     2,
      27,    28,    25,    23,    29,    24,     2,    26,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      21,     2,    22,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    19,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int ExprBisonParser::yyeof_ = 0;
  const int ExprBisonParser::yylast_ = 185;
  const int ExprBisonParser::yynnts_ = 7;
  const int ExprBisonParser::yyempty_ = -2;
  const int ExprBisonParser::yyfinal_ = 21;
  const int ExprBisonParser::yyterror_ = 1;
  const int ExprBisonParser::yyerrcode_ = 256;
  const int ExprBisonParser::yyntokens_ = 30;

  const unsigned int ExprBisonParser::yyuser_token_number_max_ = 273;
  const ExprBisonParser::token_number_type ExprBisonParser::yyundef_token_ = 2;


/* Line 1054 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // fx_bison

/* Line 1054 of lalr1.cc  */
#line 1231 "ExprBisonParser.cc"


/* Line 1056 of lalr1.cc  */
#line 208 "ExprBisonParser.yy"
 

/** Additional Code */
void fx_bison::ExprBisonParser::error(
        const fx_bison::ExprBisonParser::location_type& l,
        const std::string& m)
{
    exprParser.fail(l, m);
}

