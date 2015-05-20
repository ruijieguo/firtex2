
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
#line 83 "ExprBisonParser.yy"

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
#line 81 "ExprBisonParser.yy"
	{delete (yyvaluep->stringVal);};

/* Line 480 of lalr1.cc  */
#line 261 "ExprBisonParser.cc"
	break;
      case 6: /* "\"double quoted string\"" */

/* Line 480 of lalr1.cc  */
#line 81 "ExprBisonParser.yy"
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
#line 110 "ExprBisonParser.yy"
    { 
              (yyval.nodeIdx) = (yysemantic_stack_[(2) - (1)].nodeIdx);              
              exprParser.finalize((yysemantic_stack_[(2) - (1)].nodeIdx)); 
          }
    break;

  case 3:

/* Line 678 of lalr1.cc  */
#line 116 "ExprBisonParser.yy"
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
#line 125 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addAttrNode(*(yysemantic_stack_[(1) - (1)].stringVal)); 
              delete (yysemantic_stack_[(1) - (1)].stringVal);
              if ((yyval.nodeIdx) < 0)
              {
                  YYERROR;
              }
          }
    break;

  case 8:

/* Line 678 of lalr1.cc  */
#line 139 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addIntNode((yysemantic_stack_[(1) - (1)].intVal)); }
    break;

  case 9:

/* Line 678 of lalr1.cc  */
#line 140 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addFloatNode((yysemantic_stack_[(1) - (1)].doubleVal)); }
    break;

  case 10:

/* Line 678 of lalr1.cc  */
#line 141 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_NEG, (yysemantic_stack_[(2) - (2)].nodeIdx), -1); }
    break;

  case 11:

/* Line 678 of lalr1.cc  */
#line 143 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_NOT, (yysemantic_stack_[(2) - (2)].nodeIdx), -1);
              if ((yyval.nodeIdx) < 0) 
              {
                  YYERROR; 
              }
          }
    break;

  case 12:

/* Line 678 of lalr1.cc  */
#line 150 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_PLUS, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 13:

/* Line 678 of lalr1.cc  */
#line 151 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_MINUS, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 14:

/* Line 678 of lalr1.cc  */
#line 152 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_MUL, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 15:

/* Line 678 of lalr1.cc  */
#line 153 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_DIV, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 16:

/* Line 678 of lalr1.cc  */
#line 154 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_LE, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 17:

/* Line 678 of lalr1.cc  */
#line 155 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_GR, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 18:

/* Line 678 of lalr1.cc  */
#line 156 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_BIT_AND, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 19:

/* Line 678 of lalr1.cc  */
#line 157 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_BIT_OR, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 20:

/* Line 678 of lalr1.cc  */
#line 158 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_LTE, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 21:

/* Line 678 of lalr1.cc  */
#line 159 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_GTE, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 22:

/* Line 678 of lalr1.cc  */
#line 160 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_EQ, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 23:

/* Line 678 of lalr1.cc  */
#line 161 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_NE, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 24:

/* Line 678 of lalr1.cc  */
#line 162 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_AND, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx)); }
    break;

  case 25:

/* Line 678 of lalr1.cc  */
#line 163 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_OR, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx)); }
    break;

  case 26:

/* Line 678 of lalr1.cc  */
#line 164 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = (yysemantic_stack_[(3) - (2)].nodeIdx); }
    break;

  case 27:

/* Line 678 of lalr1.cc  */
#line 168 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = (yysemantic_stack_[(1) - (1)].nodeIdx); }
    break;

  case 28:

/* Line 678 of lalr1.cc  */
#line 169 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_COMMA, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx)); }
    break;

  case 29:

/* Line 678 of lalr1.cc  */
#line 173 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addConstListNode((yysemantic_stack_[(1) - (1)].intVal)); }
    break;

  case 30:

/* Line 678 of lalr1.cc  */
#line 174 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addConstListNode((yysemantic_stack_[(1) - (1)].doubleVal)); }
    break;

  case 31:

/* Line 678 of lalr1.cc  */
#line 175 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addConstListNode(*(yysemantic_stack_[(1) - (1)].stringVal)); delete (yysemantic_stack_[(1) - (1)].stringVal);}
    break;

  case 32:

/* Line 678 of lalr1.cc  */
#line 176 "ExprBisonParser.yy"
    { exprParser.appendToConstList((yyval.nodeIdx), (yysemantic_stack_[(3) - (3)].intVal)); }
    break;

  case 33:

/* Line 678 of lalr1.cc  */
#line 177 "ExprBisonParser.yy"
    { exprParser.appendToConstList((yyval.nodeIdx), (yysemantic_stack_[(3) - (3)].doubleVal)); }
    break;

  case 34:

/* Line 678 of lalr1.cc  */
#line 178 "ExprBisonParser.yy"
    { exprParser.appendToConstList((yyval.nodeIdx), *(yysemantic_stack_[(3) - (3)].stringVal)); delete (yysemantic_stack_[(3) - (3)].stringVal);}
    break;

  case 35:

/* Line 678 of lalr1.cc  */
#line 183 "ExprBisonParser.yy"
    { 
              (yyval.nodeIdx) = exprParser.addFuncNode((yysemantic_stack_[(4) - (1)].funcIdx), (yysemantic_stack_[(4) - (3)].nodeIdx));
              if ((yyval.nodeIdx)<0)
              {
                  YYERROR; 
              }
          }
    break;

  case 36:

/* Line 678 of lalr1.cc  */
#line 191 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addFuncNode((yysemantic_stack_[(3) - (1)].funcIdx), -1); 
              if ((yyval.nodeIdx) < 0)
              {
                  YYERROR;
              }
          }
    break;

  case 37:

/* Line 678 of lalr1.cc  */
#line 199 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addFuncNode((yysemantic_stack_[(6) - (1)].funcIdx), (yysemantic_stack_[(6) - (3)].nodeIdx), (yysemantic_stack_[(6) - (5)].nodeIdx));
          }
    break;

  case 38:

/* Line 678 of lalr1.cc  */
#line 203 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addDateFuncNode((yysemantic_stack_[(4) - (1)].funcIdx), *(yysemantic_stack_[(4) - (3)].stringVal));
              delete (yysemantic_stack_[(4) - (3)].stringVal);
          }
    break;

  case 39:

/* Line 678 of lalr1.cc  */
#line 208 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addDateFuncNode((yysemantic_stack_[(4) - (1)].funcIdx), *(yysemantic_stack_[(4) - (3)].stringVal));
          }
    break;

  case 40:

/* Line 678 of lalr1.cc  */
#line 215 "ExprBisonParser.yy"
    { 
              (yyval.nodeIdx) = exprParser.addFeatureNode(*(yysemantic_stack_[(5) - (2)].stringVal), (yysemantic_stack_[(5) - (4)].nodeIdx));
              delete (yysemantic_stack_[(5) - (2)].stringVal);
              if ((yyval.nodeIdx)<0)
              {
                  YYERROR; 
              }
          }
    break;

  case 41:

/* Line 678 of lalr1.cc  */
#line 224 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addFeatureNode(*(yysemantic_stack_[(4) - (2)].stringVal), -1); 
              delete (yysemantic_stack_[(4) - (2)].stringVal);
              if ((yyval.nodeIdx) < 0)
              {
                  YYERROR;
              }
          }
    break;

  case 42:

/* Line 678 of lalr1.cc  */
#line 233 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addFeatureNode(*(yysemantic_stack_[(2) - (2)].stringVal), -1); 
              delete (yysemantic_stack_[(2) - (2)].stringVal);
              if ((yyval.nodeIdx) < 0)
              {
                  YYERROR;
              }
          }
    break;



/* Line 678 of lalr1.cc  */
#line 792 "ExprBisonParser.cc"
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
  const signed char ExprBisonParser::yypact_ninf_ = -23;
  const short int
  ExprBisonParser::yypact_[] =
  {
        75,   -23,   -23,   -23,   -23,    33,    35,    39,    75,    75,
      75,    20,    76,   -23,    93,   -23,   -23,    37,    10,    42,
     -23,   -23,   110,    64,   -23,   -23,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
     -23,   129,    29,    59,    66,    67,   -23,    47,    -2,   146,
     187,   -22,   -22,   187,   160,   174,   -22,   -22,    60,    60,
     -23,   -23,   -23,    75,     2,   -23,   -23,   -23,    44,   129,
     -23,   -23,   -23,    61,   -23,   -23,    65,   -23,   -23,   -23
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  ExprBisonParser::yydefact_[] =
  {
         0,     8,     9,     3,     4,     0,     0,     0,     0,     0,
       0,     0,     0,     5,     0,     6,     7,     0,     0,     0,
      11,    10,     0,    42,     1,     2,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      36,    27,     0,     0,     0,     0,    26,     0,    24,    25,
      22,    20,    21,    23,    19,    18,    16,    17,    12,    13,
      14,    15,    35,     0,     0,    38,    39,    41,     0,    28,
      29,    30,    31,     0,    40,    37,     0,    32,    33,    34
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  ExprBisonParser::yypgoto_[] =
  {
       -23,   -23,    74,     0,    49,   -23,   -23,   -23
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  ExprBisonParser::yydefgoto_[] =
  {
        -1,    12,    13,    41,    42,    73,    15,    16
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char ExprBisonParser::yytable_ninf_ = -1;
  const unsigned char
  ExprBisonParser::yytable_[] =
  {
        14,    36,    37,    38,    39,    70,    71,    72,    20,    21,
      22,    28,    29,    30,    31,     3,     4,    32,    33,    34,
      35,    36,    37,    38,    39,    23,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
       1,     2,     3,     4,     5,     6,     7,    44,    45,     8,
       1,     2,     3,     4,     5,     6,     7,    62,    63,     8,
      17,     9,    18,    69,    10,    40,    19,    11,    77,    78,
      79,     9,    74,    63,    10,    67,    24,    11,     1,     2,
       3,     4,     5,     6,     7,    38,    39,     8,    64,    75,
      76,    47,    43,    25,    65,    66,    68,     0,     0,     9,
       0,     0,    10,    26,    27,    11,    28,    29,    30,    31,
       0,     0,    32,    33,    34,    35,    36,    37,    38,    39,
      26,    27,     0,    28,    29,    30,    31,     0,     0,    32,
      33,    34,    35,    36,    37,    38,    39,     0,    46,    26,
      27,     0,    28,    29,    30,    31,     0,     0,    32,    33,
      34,    35,    36,    37,    38,    39,    26,     0,     0,    28,
      29,    30,    31,     0,     0,    32,    33,    34,    35,    36,
      37,    38,    39,    28,    29,    30,    31,     0,     0,     0,
      33,    34,    35,    36,    37,    38,    39,    28,    29,    30,
      31,     0,     0,     0,     0,    34,    35,    36,    37,    38,
      39,    29,    30,     0,     0,     0,     0,     0,    34,    35,
      36,    37,    38,    39
  };

  /* YYCHECK.  */
  const signed char
  ExprBisonParser::yycheck_[] =
  {
         0,    23,    24,    25,    26,     3,     4,     5,     8,     9,
      10,    13,    14,    15,    16,     5,     6,    19,    20,    21,
      22,    23,    24,    25,    26,     5,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
       3,     4,     5,     6,     7,     8,     9,     5,     6,    12,
       3,     4,     5,     6,     7,     8,     9,    28,    29,    12,
      27,    24,    27,    63,    27,    28,    27,    30,     3,     4,
       5,    24,    28,    29,    27,    28,     0,    30,     3,     4,
       5,     6,     7,     8,     9,    25,    26,    12,    29,    28,
      29,    27,    18,     0,    28,    28,    47,    -1,    -1,    24,
      -1,    -1,    27,    10,    11,    30,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      10,    11,    -1,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    28,    10,
      11,    -1,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    10,    -1,    -1,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    13,    14,    15,    16,    -1,    -1,    -1,
      20,    21,    22,    23,    24,    25,    26,    13,    14,    15,
      16,    -1,    -1,    -1,    -1,    21,    22,    23,    24,    25,
      26,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    22,
      23,    24,    25,    26
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  ExprBisonParser::yystos_[] =
  {
         0,     3,     4,     5,     6,     7,     8,     9,    12,    24,
      27,    30,    32,    33,    34,    37,    38,    27,    27,    27,
      34,    34,    34,     5,     0,     0,    10,    11,    13,    14,
      15,    16,    19,    20,    21,    22,    23,    24,    25,    26,
      28,    34,    35,    33,     5,     6,    28,    27,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    28,    29,    29,    28,    28,    28,    35,    34,
       3,     4,     5,    36,    28,    28,    29,     3,     4,     5
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  ExprBisonParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   124,
      38,    60,    62,    43,    45,    42,    47,    40,    41,    44,
      64
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  ExprBisonParser::yyr1_[] =
  {
         0,    31,    32,    33,    33,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    35,    35,    36,
      36,    36,    36,    36,    36,    37,    37,    37,    37,    37,
      38,    38,    38
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  ExprBisonParser::yyr2_[] =
  {
         0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     3,     1,
       1,     1,     3,     3,     3,     4,     3,     6,     4,     4,
       5,     4,     2
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
  "'>'", "'+'", "'-'", "'*'", "'/'", "'('", "')'", "','", "'@'", "$accept",
  "exprline", "attr", "expr", "arglist", "constlist", "function",
  "feature", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const ExprBisonParser::rhs_number_type
  ExprBisonParser::yyrhs_[] =
  {
        32,     0,    -1,    34,     0,    -1,     5,    -1,     6,    -1,
      33,    -1,    37,    -1,    38,    -1,     3,    -1,     4,    -1,
      24,    34,    -1,    12,    34,    -1,    34,    23,    34,    -1,
      34,    24,    34,    -1,    34,    25,    34,    -1,    34,    26,
      34,    -1,    34,    21,    34,    -1,    34,    22,    34,    -1,
      34,    20,    34,    -1,    34,    19,    34,    -1,    34,    14,
      34,    -1,    34,    15,    34,    -1,    34,    13,    34,    -1,
      34,    16,    34,    -1,    34,    10,    34,    -1,    34,    11,
      34,    -1,    27,    34,    28,    -1,    34,    -1,    35,    29,
      34,    -1,     3,    -1,     4,    -1,     5,    -1,    36,    29,
       3,    -1,    36,    29,     4,    -1,    36,    29,     5,    -1,
       7,    27,    35,    28,    -1,     7,    27,    28,    -1,     8,
      27,    33,    29,    36,    28,    -1,     9,    27,     5,    28,
      -1,     9,    27,     6,    28,    -1,    30,     5,    27,    35,
      28,    -1,    30,     5,    27,    28,    -1,    30,     5,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  ExprBisonParser::yyprhs_[] =
  {
         0,     0,     3,     6,     8,    10,    12,    14,    16,    18,
      20,    23,    26,    30,    34,    38,    42,    46,    50,    54,
      58,    62,    66,    70,    74,    78,    82,    86,    88,    92,
      94,    96,    98,   102,   106,   110,   115,   119,   126,   131,
     136,   142,   147
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  ExprBisonParser::yyrline_[] =
  {
         0,   109,   109,   115,   124,   136,   137,   138,   139,   140,
     141,   142,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   168,   169,   173,
     174,   175,   176,   177,   178,   182,   190,   198,   202,   207,
     214,   223,   232
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
      21,     2,    22,     2,    30,     2,     2,     2,     2,     2,
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
  const int ExprBisonParser::yylast_ = 213;
  const int ExprBisonParser::yynnts_ = 8;
  const int ExprBisonParser::yyempty_ = -2;
  const int ExprBisonParser::yyfinal_ = 24;
  const int ExprBisonParser::yyterror_ = 1;
  const int ExprBisonParser::yyerrcode_ = 256;
  const int ExprBisonParser::yyntokens_ = 31;

  const unsigned int ExprBisonParser::yyuser_token_number_max_ = 273;
  const ExprBisonParser::token_number_type ExprBisonParser::yyundef_token_ = 2;


/* Line 1054 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // fx_bison

/* Line 1054 of lalr1.cc  */
#line 1302 "ExprBisonParser.cc"


/* Line 1056 of lalr1.cc  */
#line 243 "ExprBisonParser.yy"
 

/** Additional Code */
void fx_bison::ExprBisonParser::error(
        const fx_bison::ExprBisonParser::location_type& l,
        const std::string& m)
{
    exprParser.fail(l, m);
}

