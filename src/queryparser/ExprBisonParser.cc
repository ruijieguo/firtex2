/* A Bison parser, made by GNU Bison 2.5.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2011 Free Software Foundation, Inc.
   
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

/* Line 293 of lalr1.cc  */
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



/* Line 293 of lalr1.cc  */
#line 59 "ExprBisonParser.cc"


#include "ExprBisonParser.hh"

/* User implementation prologue.  */

/* Line 299 of lalr1.cc  */
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


/* Line 299 of lalr1.cc  */
#line 82 "ExprBisonParser.cc"

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                               \
 do                                                                    \
   if (N)                                                              \
     {                                                                 \
       (Current).begin = YYRHSLOC (Rhs, 1).begin;                      \
       (Current).end   = YYRHSLOC (Rhs, N).end;                        \
     }                                                                 \
   else                                                                \
     {                                                                 \
       (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;        \
     }                                                                 \
 while (false)
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


namespace fx_bison {

/* Line 382 of lalr1.cc  */
#line 168 "ExprBisonParser.cc"

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
#line 274 "ExprBisonParser.cc"
	break;
      case 6: /* "\"double quoted string\"" */

/* Line 480 of lalr1.cc  */
#line 80 "ExprBisonParser.yy"
	{delete (yyvaluep->stringVal);};

/* Line 480 of lalr1.cc  */
#line 283 "ExprBisonParser.cc"
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

  inline bool
  ExprBisonParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  ExprBisonParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

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
    location_type yyerror_range[3];

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
    if (yy_pact_value_is_default_ (yyn))
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
	if (yy_table_value_is_error_ (yyn))
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

/* Line 690 of lalr1.cc  */
#line 109 "ExprBisonParser.yy"
    { 
              (yyval.nodeIdx) = (yysemantic_stack_[(2) - (1)].nodeIdx);              
              exprParser.finalize((yysemantic_stack_[(2) - (1)].nodeIdx)); 
          }
    break;

  case 3:

/* Line 690 of lalr1.cc  */
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

/* Line 690 of lalr1.cc  */
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

/* Line 690 of lalr1.cc  */
#line 137 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addIntNode((yysemantic_stack_[(1) - (1)].intVal)); }
    break;

  case 8:

/* Line 690 of lalr1.cc  */
#line 138 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addFloatNode((yysemantic_stack_[(1) - (1)].doubleVal)); }
    break;

  case 9:

/* Line 690 of lalr1.cc  */
#line 139 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_NEG, (yysemantic_stack_[(2) - (2)].nodeIdx), -1); }
    break;

  case 10:

/* Line 690 of lalr1.cc  */
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

/* Line 690 of lalr1.cc  */
#line 148 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_PLUS, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 12:

/* Line 690 of lalr1.cc  */
#line 149 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_MINUS, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 13:

/* Line 690 of lalr1.cc  */
#line 150 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_MUL, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 14:

/* Line 690 of lalr1.cc  */
#line 151 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_DIV, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 15:

/* Line 690 of lalr1.cc  */
#line 152 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_LE, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 16:

/* Line 690 of lalr1.cc  */
#line 153 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_GR, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 17:

/* Line 690 of lalr1.cc  */
#line 154 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_BIT_AND, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 18:

/* Line 690 of lalr1.cc  */
#line 155 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_BIT_OR, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 19:

/* Line 690 of lalr1.cc  */
#line 156 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_LTE, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 20:

/* Line 690 of lalr1.cc  */
#line 157 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_GTE, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 21:

/* Line 690 of lalr1.cc  */
#line 158 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_EQ, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 22:

/* Line 690 of lalr1.cc  */
#line 159 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_NE, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx) ); }
    break;

  case 23:

/* Line 690 of lalr1.cc  */
#line 160 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_AND, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx)); }
    break;

  case 24:

/* Line 690 of lalr1.cc  */
#line 161 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_OR, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx)); }
    break;

  case 25:

/* Line 690 of lalr1.cc  */
#line 162 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = (yysemantic_stack_[(3) - (2)].nodeIdx); }
    break;

  case 26:

/* Line 690 of lalr1.cc  */
#line 166 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = (yysemantic_stack_[(1) - (1)].nodeIdx); }
    break;

  case 27:

/* Line 690 of lalr1.cc  */
#line 167 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addOpNode(ExprNode::TOK_COMMA, (yysemantic_stack_[(3) - (1)].nodeIdx), (yysemantic_stack_[(3) - (3)].nodeIdx)); }
    break;

  case 28:

/* Line 690 of lalr1.cc  */
#line 171 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addConstListNode((yysemantic_stack_[(1) - (1)].intVal)); }
    break;

  case 29:

/* Line 690 of lalr1.cc  */
#line 172 "ExprBisonParser.yy"
    { (yyval.nodeIdx) = exprParser.addConstListNode((yysemantic_stack_[(1) - (1)].doubleVal)); }
    break;

  case 30:

/* Line 690 of lalr1.cc  */
#line 173 "ExprBisonParser.yy"
    { exprParser.appendToConstList((yyval.nodeIdx), (yysemantic_stack_[(3) - (3)].intVal)); }
    break;

  case 31:

/* Line 690 of lalr1.cc  */
#line 174 "ExprBisonParser.yy"
    { exprParser.appendToConstList((yyval.nodeIdx), (yysemantic_stack_[(3) - (3)].doubleVal)); }
    break;

  case 32:

/* Line 690 of lalr1.cc  */
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

/* Line 690 of lalr1.cc  */
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

/* Line 690 of lalr1.cc  */
#line 195 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addFuncNode((yysemantic_stack_[(6) - (1)].funcIdx), (yysemantic_stack_[(6) - (3)].nodeIdx), (yysemantic_stack_[(6) - (5)].nodeIdx));
          }
    break;

  case 35:

/* Line 690 of lalr1.cc  */
#line 199 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addDateFuncNode((yysemantic_stack_[(4) - (1)].funcIdx), *(yysemantic_stack_[(4) - (3)].stringVal));
          }
    break;

  case 36:

/* Line 690 of lalr1.cc  */
#line 203 "ExprBisonParser.yy"
    {
              (yyval.nodeIdx) = exprParser.addDateFuncNode((yysemantic_stack_[(4) - (1)].funcIdx), *(yysemantic_stack_[(4) - (3)].stringVal));
          }
    break;



/* Line 690 of lalr1.cc  */
#line 760 "ExprBisonParser.cc"
	default:
          break;
      }
    /* User semantic actions sometimes alter yychar, and that requires
       that yytoken be updated with the new translation.  We take the
       approach of translating immediately before every use of yytoken.
       One alternative is translating here after every semantic action,
       but that translation would be missed if the semantic action
       invokes YYABORT, YYACCEPT, or YYERROR immediately after altering
       yychar.  In the case of YYABORT or YYACCEPT, an incorrect
       destructor might then be invoked immediately.  In the case of
       YYERROR, subsequent parser actions might lead to an incorrect
       destructor call or verbose syntax error message before the
       lookahead is translated.  */
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
    /* Make sure we have latest lookahead translation.  See comments at
       user semantic actions for why this is necessary.  */
    yytoken = yytranslate_ (yychar);

    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	if (yychar == yyempty_)
	  yytoken = yyempty_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[1] = yylloc;
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

    yyerror_range[1] = yylocation_stack_[yylen - 1];
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
	if (!yy_pact_value_is_default_ (yyn))
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

	yyerror_range[1] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[2] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
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
      {
        /* Make sure we have latest lookahead translation.  See comments
           at user semantic actions for why this is necessary.  */
        yytoken = yytranslate_ (yychar);
        yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval,
                     &yylloc);
      }

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
  ExprBisonParser::yysyntax_error_ (int yystate, int yytoken)
  {
    std::string yyres;
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yytoken) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yychar.
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (yytoken != yyempty_)
      {
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            /* Stay within bounds of both yycheck and yytname.  */
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = 0;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
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

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
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
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
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


} // fx_bison

/* Line 1136 of lalr1.cc  */
#line 1322 "ExprBisonParser.cc"


/* Line 1138 of lalr1.cc  */
#line 208 "ExprBisonParser.yy"
 

/** Additional Code */
void fx_bison::ExprBisonParser::error(
        const fx_bison::ExprBisonParser::location_type& l,
        const std::string& m)
{
    exprParser.fail(l, m);
}

