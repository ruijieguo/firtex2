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
#line 1 "QueryExprBisonParser.yy"

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



/* Line 293 of lalr1.cc  */
#line 58 "QueryExprBisonParser.cc"


#include "QueryExprBisonParser.hh"

/* User implementation prologue.  */

/* Line 299 of lalr1.cc  */
#line 83 "QueryExprBisonParser.yy"

#include "firtex/queryparser/QueryExprLexer.h"
#include "firtex/queryparser/QueryExprParser.h"

/**
 * This "connects" the bison parser to the flex scanner class  object.
 * It defines the yylex() function call to pull the next token from the
 * current lexer object. 
 */
#undef yylex
#define yylex lexer.lex


/* Line 299 of lalr1.cc  */
#line 81 "QueryExprBisonParser.cc"

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
#line 167 "QueryExprBisonParser.cc"

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  QueryExprBisonParser::yytnamerr_ (const char *yystr)
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
  QueryExprBisonParser::QueryExprBisonParser (FX_NS(queryparser)::QueryExprLexer& lexer_yyarg, FX_NS(queryparser)::QueryExprParser& exprParser_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      lexer (lexer_yyarg),
      exprParser (exprParser_yyarg)
  {
  }

  QueryExprBisonParser::~QueryExprBisonParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  QueryExprBisonParser::yy_symbol_value_print_ (int yytype,
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
  QueryExprBisonParser::yy_symbol_print_ (int yytype,
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
  QueryExprBisonParser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
        case 3: /* "\"integer\"" */

/* Line 480 of lalr1.cc  */
#line 75 "QueryExprBisonParser.yy"
	{delete (yyvaluep->numberVal);};

/* Line 480 of lalr1.cc  */
#line 273 "QueryExprBisonParser.cc"
	break;
      case 4: /* "\"float\"" */

/* Line 480 of lalr1.cc  */
#line 75 "QueryExprBisonParser.yy"
	{delete (yyvaluep->numberVal);};

/* Line 480 of lalr1.cc  */
#line 282 "QueryExprBisonParser.cc"
	break;
      case 5: /* "\"identifier\"" */

/* Line 480 of lalr1.cc  */
#line 76 "QueryExprBisonParser.yy"
	{delete (yyvaluep->stringVal);};

/* Line 480 of lalr1.cc  */
#line 291 "QueryExprBisonParser.cc"
	break;
      case 6: /* "\"string NOT including symbols\"" */

/* Line 480 of lalr1.cc  */
#line 76 "QueryExprBisonParser.yy"
	{delete (yyvaluep->stringVal);};

/* Line 480 of lalr1.cc  */
#line 300 "QueryExprBisonParser.cc"
	break;
      case 7: /* "\"single quoted string\"" */

/* Line 480 of lalr1.cc  */
#line 76 "QueryExprBisonParser.yy"
	{delete (yyvaluep->stringVal);};

/* Line 480 of lalr1.cc  */
#line 309 "QueryExprBisonParser.cc"
	break;
      case 8: /* "\"double quoted string\"" */

/* Line 480 of lalr1.cc  */
#line 76 "QueryExprBisonParser.yy"
	{delete (yyvaluep->stringVal);};

/* Line 480 of lalr1.cc  */
#line 318 "QueryExprBisonParser.cc"
	break;
      case 30: /* "bool_expr" */

/* Line 480 of lalr1.cc  */
#line 77 "QueryExprBisonParser.yy"
	{delete (yyvaluep->queryExprVal);};

/* Line 480 of lalr1.cc  */
#line 327 "QueryExprBisonParser.cc"
	break;
      case 31: /* "expr" */

/* Line 480 of lalr1.cc  */
#line 77 "QueryExprBisonParser.yy"
	{delete (yyvaluep->queryExprVal);};

/* Line 480 of lalr1.cc  */
#line 336 "QueryExprBisonParser.cc"
	break;
      case 32: /* "group_expr" */

/* Line 480 of lalr1.cc  */
#line 78 "QueryExprBisonParser.yy"
	{delete (yyvaluep->queryExprVal);};

/* Line 480 of lalr1.cc  */
#line 345 "QueryExprBisonParser.cc"
	break;
      case 33: /* "req_expr" */

/* Line 480 of lalr1.cc  */
#line 78 "QueryExprBisonParser.yy"
	{delete (yyvaluep->queryExprVal);};

/* Line 480 of lalr1.cc  */
#line 354 "QueryExprBisonParser.cc"
	break;
      case 34: /* "proh_expr" */

/* Line 480 of lalr1.cc  */
#line 78 "QueryExprBisonParser.yy"
	{delete (yyvaluep->queryExprVal);};

/* Line 480 of lalr1.cc  */
#line 363 "QueryExprBisonParser.cc"
	break;
      case 35: /* "and_expr" */

/* Line 480 of lalr1.cc  */
#line 77 "QueryExprBisonParser.yy"
	{delete (yyvaluep->queryExprVal);};

/* Line 480 of lalr1.cc  */
#line 372 "QueryExprBisonParser.cc"
	break;
      case 36: /* "or_expr" */

/* Line 480 of lalr1.cc  */
#line 77 "QueryExprBisonParser.yy"
	{delete (yyvaluep->queryExprVal);};

/* Line 480 of lalr1.cc  */
#line 381 "QueryExprBisonParser.cc"
	break;
      case 39: /* "atom_expr" */

/* Line 480 of lalr1.cc  */
#line 79 "QueryExprBisonParser.yy"
	{delete (yyvaluep->termExprVal);};

/* Line 480 of lalr1.cc  */
#line 390 "QueryExprBisonParser.cc"
	break;
      case 40: /* "boost_term" */

/* Line 480 of lalr1.cc  */
#line 81 "QueryExprBisonParser.yy"
	{delete (yyvaluep->termExprVal);};

/* Line 480 of lalr1.cc  */
#line 399 "QueryExprBisonParser.cc"
	break;
      case 41: /* "atom_term" */

/* Line 480 of lalr1.cc  */
#line 81 "QueryExprBisonParser.yy"
	{delete (yyvaluep->termExprVal);};

/* Line 480 of lalr1.cc  */
#line 408 "QueryExprBisonParser.cc"
	break;
      case 42: /* "term_expr" */

/* Line 480 of lalr1.cc  */
#line 81 "QueryExprBisonParser.yy"
	{delete (yyvaluep->termExprVal);};

/* Line 480 of lalr1.cc  */
#line 417 "QueryExprBisonParser.cc"
	break;
      case 43: /* "phrase_expr" */

/* Line 480 of lalr1.cc  */
#line 81 "QueryExprBisonParser.yy"
	{delete (yyvaluep->termExprVal);};

/* Line 480 of lalr1.cc  */
#line 426 "QueryExprBisonParser.cc"
	break;
      case 44: /* "range_expr" */

/* Line 480 of lalr1.cc  */
#line 81 "QueryExprBisonParser.yy"
	{delete (yyvaluep->termExprVal);};

/* Line 480 of lalr1.cc  */
#line 435 "QueryExprBisonParser.cc"
	break;
      case 45: /* "range_val" */

/* Line 480 of lalr1.cc  */
#line 80 "QueryExprBisonParser.yy"
	{delete (yyvaluep->stringVal);};

/* Line 480 of lalr1.cc  */
#line 444 "QueryExprBisonParser.cc"
	break;
      case 46: /* "field_name" */

/* Line 480 of lalr1.cc  */
#line 80 "QueryExprBisonParser.yy"
	{delete (yyvaluep->stringVal);};

/* Line 480 of lalr1.cc  */
#line 453 "QueryExprBisonParser.cc"
	break;

	default:
	  break;
      }
  }

  void
  QueryExprBisonParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  QueryExprBisonParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  QueryExprBisonParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  QueryExprBisonParser::debug_level_type
  QueryExprBisonParser::debug_level () const
  {
    return yydebug_;
  }

  void
  QueryExprBisonParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  inline bool
  QueryExprBisonParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  QueryExprBisonParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  QueryExprBisonParser::parse ()
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
#line 104 "QueryExprBisonParser.yy"
    { 
    (yyval.queryExprVal) = (yysemantic_stack_[(2) - (1)].queryExprVal);
    exprParser.finalize((yyval.queryExprVal));
}
    break;

  case 4:

/* Line 690 of lalr1.cc  */
#line 111 "QueryExprBisonParser.yy"
    {
                (yyval.queryExprVal) = new BooleanQueryExpr((yysemantic_stack_[(2) - (1)].queryExprVal), (yysemantic_stack_[(2) - (2)].queryExprVal), exprParser.getDefaultOp()); 
            }
    break;

  case 5:

/* Line 690 of lalr1.cc  */
#line 115 "QueryExprBisonParser.yy"
    {(yyval.queryExprVal) = (yysemantic_stack_[(1) - (1)].termExprVal);}
    break;

  case 12:

/* Line 690 of lalr1.cc  */
#line 124 "QueryExprBisonParser.yy"
    {
    (yyval.queryExprVal) = (yysemantic_stack_[(3) - (2)].queryExprVal);
}
    break;

  case 13:

/* Line 690 of lalr1.cc  */
#line 129 "QueryExprBisonParser.yy"
    {
    (yysemantic_stack_[(2) - (2)].queryExprVal)->setRequired(true);
    (yyval.queryExprVal) = (yysemantic_stack_[(2) - (2)].queryExprVal);
}
    break;

  case 14:

/* Line 690 of lalr1.cc  */
#line 135 "QueryExprBisonParser.yy"
    {
    (yysemantic_stack_[(2) - (2)].queryExprVal)->setProhibited(true);
    (yyval.queryExprVal) = (yysemantic_stack_[(2) - (2)].queryExprVal);
}
    break;

  case 15:

/* Line 690 of lalr1.cc  */
#line 141 "QueryExprBisonParser.yy"
    {
    (yyval.queryExprVal) = new BooleanQueryExpr((yysemantic_stack_[(3) - (1)].queryExprVal), (yysemantic_stack_[(3) - (3)].queryExprVal), QueryExpr::OP_AND);
}
    break;

  case 16:

/* Line 690 of lalr1.cc  */
#line 146 "QueryExprBisonParser.yy"
    {
    (yyval.queryExprVal) = new BooleanQueryExpr((yysemantic_stack_[(3) - (1)].queryExprVal), (yysemantic_stack_[(3) - (3)].queryExprVal), QueryExpr::OP_OR);
}
    break;

  case 17:

/* Line 690 of lalr1.cc  */
#line 151 "QueryExprBisonParser.yy"
    {
                      (yysemantic_stack_[(4) - (3)].queryExprVal)->setFieldName(*(yysemantic_stack_[(4) - (1)].stringVal));
                      delete (yysemantic_stack_[(4) - (1)].stringVal);
                      (yyval.queryExprVal) = (yysemantic_stack_[(4) - (3)].queryExprVal);
                  }
    break;

  case 18:

/* Line 690 of lalr1.cc  */
#line 158 "QueryExprBisonParser.yy"
    {
                (yyval.queryExprVal) = (yysemantic_stack_[(1) - (1)].queryExprVal);
            }
    break;

  case 19:

/* Line 690 of lalr1.cc  */
#line 162 "QueryExprBisonParser.yy"
    {
                (yyval.queryExprVal) = new BooleanQueryExpr((yysemantic_stack_[(2) - (1)].queryExprVal), (yysemantic_stack_[(2) - (2)].queryExprVal), exprParser.getDefaultOp());
            }
    break;

  case 20:

/* Line 690 of lalr1.cc  */
#line 167 "QueryExprBisonParser.yy"
    {
		  const std::string &defaultField = exprParser.getDefaultField();
	          (yysemantic_stack_[(1) - (1)].termExprVal)->setFieldName(defaultField);
		  (yyval.termExprVal) = (yysemantic_stack_[(1) - (1)].termExprVal);
              }
    break;

  case 21:

/* Line 690 of lalr1.cc  */
#line 173 "QueryExprBisonParser.yy"
    {
		  std::string* fieldName = (yysemantic_stack_[(2) - (1)].stringVal);
		  (yysemantic_stack_[(2) - (2)].termExprVal)->setFieldName(*fieldName);
		  delete (yysemantic_stack_[(2) - (1)].stringVal);
		  (yyval.termExprVal) = (yysemantic_stack_[(2) - (2)].termExprVal);
              }
    break;

  case 22:

/* Line 690 of lalr1.cc  */
#line 181 "QueryExprBisonParser.yy"
    {
                 (yyval.termExprVal) = (yysemantic_stack_[(1) - (1)].termExprVal);
             }
    break;

  case 23:

/* Line 690 of lalr1.cc  */
#line 185 "QueryExprBisonParser.yy"
    {
                 (yysemantic_stack_[(3) - (1)].termExprVal)->setBoost((float)(yysemantic_stack_[(3) - (3)].numberVal)->m_i64Val);
                 delete (yysemantic_stack_[(3) - (3)].numberVal);
                 (yyval.termExprVal) = (yysemantic_stack_[(3) - (1)].termExprVal);
             }
    break;

  case 24:

/* Line 690 of lalr1.cc  */
#line 191 "QueryExprBisonParser.yy"
    {
                 (yysemantic_stack_[(3) - (1)].termExprVal)->setBoost((yysemantic_stack_[(3) - (3)].numberVal)->m_fVal);
                 delete (yysemantic_stack_[(3) - (3)].numberVal);
                 (yyval.termExprVal) = (yysemantic_stack_[(3) - (1)].termExprVal);
             }
    break;

  case 27:

/* Line 690 of lalr1.cc  */
#line 200 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = (yysemantic_stack_[(1) - (1)].termExprVal); }
    break;

  case 28:

/* Line 690 of lalr1.cc  */
#line 203 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = new TermQueryExpr((yysemantic_stack_[(1) - (1)].stringVal)); }
    break;

  case 29:

/* Line 690 of lalr1.cc  */
#line 205 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = new TermQueryExpr((yysemantic_stack_[(1) - (1)].stringVal)); }
    break;

  case 30:

/* Line 690 of lalr1.cc  */
#line 207 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = new TermQueryExpr((yysemantic_stack_[(1) - (1)].stringVal)); }
    break;

  case 31:

/* Line 690 of lalr1.cc  */
#line 210 "QueryExprBisonParser.yy"
    {
                  (yyval.termExprVal) = new PhraseQueryExpr((yysemantic_stack_[(1) - (1)].stringVal)); 
              }
    break;

  case 32:

/* Line 690 of lalr1.cc  */
#line 214 "QueryExprBisonParser.yy"
    {
                (yyval.termExprVal) = new PhraseQueryExpr((yysemantic_stack_[(3) - (1)].stringVal), (int32_t)(yysemantic_stack_[(3) - (3)].numberVal)->m_i64Val); 
                delete (yysemantic_stack_[(3) - (3)].numberVal);
            }
    break;

  case 33:

/* Line 690 of lalr1.cc  */
#line 220 "QueryExprBisonParser.yy"
    { 
                  (yyval.termExprVal) = new TermQueryExpr(new std::string((yysemantic_stack_[(1) - (1)].numberVal)->m_sValue)); 
                  delete (yysemantic_stack_[(1) - (1)].numberVal);
              }
    break;

  case 34:

/* Line 690 of lalr1.cc  */
#line 225 "QueryExprBisonParser.yy"
    {
                  (yyval.termExprVal) = new TermQueryExpr(new std::string((yysemantic_stack_[(1) - (1)].numberVal)->m_sValue)); 
                  delete (yysemantic_stack_[(1) - (1)].numberVal);
              }
    break;

  case 35:

/* Line 690 of lalr1.cc  */
#line 230 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = new TermQueryExpr((yysemantic_stack_[(2) - (2)].stringVal)); }
    break;

  case 36:

/* Line 690 of lalr1.cc  */
#line 232 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = new RangeQueryExpr(NULL, false, (yysemantic_stack_[(2) - (2)].stringVal), false);}
    break;

  case 37:

/* Line 690 of lalr1.cc  */
#line 234 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = new RangeQueryExpr(NULL, false, (yysemantic_stack_[(2) - (2)].stringVal), true);}
    break;

  case 38:

/* Line 690 of lalr1.cc  */
#line 236 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = new RangeQueryExpr((yysemantic_stack_[(2) - (2)].stringVal), false, NULL, false);}
    break;

  case 39:

/* Line 690 of lalr1.cc  */
#line 238 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = new RangeQueryExpr((yysemantic_stack_[(2) - (2)].stringVal), true, NULL, false);}
    break;

  case 40:

/* Line 690 of lalr1.cc  */
#line 240 "QueryExprBisonParser.yy"
    { 
                  (yyval.termExprVal) = new RangeQueryExpr((yysemantic_stack_[(5) - (2)].stringVal), true, (yysemantic_stack_[(5) - (4)].stringVal), true);
              }
    break;

  case 41:

/* Line 690 of lalr1.cc  */
#line 244 "QueryExprBisonParser.yy"
    { 
                  (yyval.termExprVal) = new RangeQueryExpr((yysemantic_stack_[(5) - (2)].stringVal), true, (yysemantic_stack_[(5) - (4)].stringVal), false);
              }
    break;

  case 42:

/* Line 690 of lalr1.cc  */
#line 248 "QueryExprBisonParser.yy"
    {
                  (yyval.termExprVal) = new RangeQueryExpr((yysemantic_stack_[(5) - (2)].stringVal), false, (yysemantic_stack_[(5) - (4)].stringVal), false);
              }
    break;

  case 43:

/* Line 690 of lalr1.cc  */
#line 252 "QueryExprBisonParser.yy"
    { 
                  (yyval.termExprVal) = new RangeQueryExpr((yysemantic_stack_[(5) - (2)].stringVal), false, (yysemantic_stack_[(5) - (4)].stringVal), true);
              }
    break;

  case 44:

/* Line 690 of lalr1.cc  */
#line 257 "QueryExprBisonParser.yy"
    {
                  (yyval.stringVal) = new std::string((yysemantic_stack_[(1) - (1)].numberVal)->m_sValue);
                  delete (yysemantic_stack_[(1) - (1)].numberVal);
              }
    break;

  case 45:

/* Line 690 of lalr1.cc  */
#line 262 "QueryExprBisonParser.yy"
    {
                  (yyval.stringVal) = new std::string((yysemantic_stack_[(1) - (1)].numberVal)->m_sValue);
                  delete (yysemantic_stack_[(1) - (1)].numberVal);
              }
    break;

  case 48:

/* Line 690 of lalr1.cc  */
#line 269 "QueryExprBisonParser.yy"
    {
                  (yyval.stringVal) = (yysemantic_stack_[(1) - (1)].stringVal);
              }
    break;

  case 49:

/* Line 690 of lalr1.cc  */
#line 274 "QueryExprBisonParser.yy"
    { 
                  (yyval.stringVal) = (yysemantic_stack_[(2) - (1)].stringVal);
              }
    break;



/* Line 690 of lalr1.cc  */
#line 990 "QueryExprBisonParser.cc"
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
  QueryExprBisonParser::yysyntax_error_ (int yystate, int yytoken)
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
  const signed char QueryExprBisonParser::yypact_ninf_ = -14;
  const short int
  QueryExprBisonParser::yypact_[] =
  {
       107,   -14,   -14,    -3,   -14,   -14,    12,    43,    43,    43,
      43,    43,   107,   107,   107,    43,    43,    33,    13,    -7,
     -14,   -14,   -14,   -14,   -14,   -14,   -14,   -14,    18,   -14,
     -14,   -14,   131,   -14,    35,   -14,   -14,   -14,   -14,   -14,
     -14,   -14,   -14,   -14,   -14,   -14,   -14,    59,    29,    32,
     -14,   -14,    -7,   107,   107,     1,   -14,   107,   -14,   -14,
     -14,    43,    43,   -14,    42,   -14,   -14,    -7,    83,   -13,
      -2,   -14,    -7,   -14,   -14,   -14,   -14
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  QueryExprBisonParser::yydefact_[] =
  {
         0,    33,    34,    28,    29,    30,    31,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     3,
       8,     9,    10,     6,     7,    11,     5,    20,    22,    25,
      26,    27,     0,    49,     0,    44,    45,    46,    47,    48,
      35,    37,    39,    36,    38,    14,    13,     0,     0,     0,
       1,     2,     4,     0,     0,     0,    28,     0,    21,    32,
      12,     0,     0,    15,    16,    23,    24,    18,     0,     0,
       0,    17,    19,    40,    41,    43,    42
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  QueryExprBisonParser::yypgoto_[] =
  {
       -14,   -14,    30,   -12,   -14,   -14,   -14,   -14,   -14,   -14,
     -14,   -14,    20,   -14,   -14,   -14,   -14,    -1,   -14
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  QueryExprBisonParser::yydefgoto_[] =
  {
        -1,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      68,    26,    27,    28,    29,    30,    31,    40,    32
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char QueryExprBisonParser::yytable_ninf_ = -1;
  const unsigned char
  QueryExprBisonParser::yytable_[] =
  {
        45,    46,    53,    54,    65,    66,    52,    41,    42,    43,
      44,    73,    74,    51,    48,    49,     1,     2,     3,     4,
       5,     6,    75,    76,    33,     7,     8,     9,    10,    11,
      12,    13,    34,    50,    14,    52,    15,    55,    59,    16,
      61,    63,    64,    62,    47,    67,    35,    36,    37,    38,
      39,    53,    58,     0,     0,     0,    72,     0,     0,     0,
      69,    70,     1,     2,     3,     4,     5,     6,     0,     0,
       0,     7,     8,     9,    10,    11,    12,    13,     0,     0,
      14,    60,    15,     0,     0,    16,     1,     2,     3,     4,
       5,     6,     0,     0,     0,     7,     8,     9,    10,    11,
      12,    13,     0,     0,    14,    71,    15,     0,     0,    16,
       1,     2,     3,     4,     5,     6,     0,     0,     0,     7,
       8,     9,    10,    11,    12,    13,     0,     0,    14,     0,
      15,     0,     0,    16,     1,     2,    56,     4,     5,     6,
       0,     0,     0,     7,     8,     9,    10,    11,     0,     0,
       0,     0,    57,     0,    15,     0,     0,    16
  };

  /* YYCHECK.  */
  const signed char
  QueryExprBisonParser::yycheck_[] =
  {
        12,    13,     9,    10,     3,     4,    18,     8,     9,    10,
      11,    24,    25,     0,    15,    16,     3,     4,     5,     6,
       7,     8,    24,    25,    27,    12,    13,    14,    15,    16,
      17,    18,    20,     0,    21,    47,    23,    19,     3,    26,
      11,    53,    54,    11,    14,    57,     3,     4,     5,     6,
       7,     9,    32,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      61,    62,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    17,    18,    -1,    -1,
      21,    22,    23,    -1,    -1,    26,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      17,    18,    -1,    -1,    21,    22,    23,    -1,    -1,    26,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    17,    18,    -1,    -1,    21,    -1,
      23,    -1,    -1,    26,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    21,    -1,    23,    -1,    -1,    26
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  QueryExprBisonParser::yystos_[] =
  {
         0,     3,     4,     5,     6,     7,     8,    12,    13,    14,
      15,    16,    17,    18,    21,    23,    26,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    39,    40,    41,    42,
      43,    44,    46,    27,    20,     3,     4,     5,     6,     7,
      45,    45,    45,    45,    45,    31,    31,    30,    45,    45,
       0,     0,    31,     9,    10,    19,     5,    21,    40,     3,
      22,    11,    11,    31,    31,     3,     4,    31,    38,    45,
      45,    22,    31,    24,    25,    24,    25
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  QueryExprBisonParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,    40,    41,    91,    93,   125,   123,    58
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  QueryExprBisonParser::yyr1_[] =
  {
         0,    28,    29,    30,    30,    31,    31,    31,    31,    31,
      31,    31,    32,    33,    34,    35,    36,    37,    38,    38,
      39,    39,    40,    40,    40,    41,    41,    41,    42,    42,
      42,    43,    43,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    45,    45,    45,    45,    45,    46
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  QueryExprBisonParser::yyr2_[] =
  {
         0,     2,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     3,     2,     2,     3,     3,     4,     1,     2,
       1,     2,     1,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     2,     2,     2,     2,     2,
       5,     5,     5,     5,     1,     1,     1,     1,     1,     2
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const QueryExprBisonParser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "\"integer\"", "\"float\"",
  "\"identifier\"", "\"string NOT including symbols\"",
  "\"single quoted string\"", "\"double quoted string\"", "TOK_AND",
  "TOK_OR", "TOK_RANGE_TO", "TOK_EQ", "TOK_LE", "TOK_GE", "TOK_LESS",
  "TOK_GREATER", "TOK_MOD_NOT", "TOK_MOD_REQ", "TOK_BOOST",
  "TOK_PHRASE_SLOP", "'('", "')'", "'['", "']'", "'}'", "'{'", "':'",
  "$accept", "query_expr", "bool_expr", "expr", "group_expr", "req_expr",
  "proh_expr", "and_expr", "or_expr", "group_term_expr", "term_list",
  "atom_expr", "boost_term", "atom_term", "term_expr", "phrase_expr",
  "range_expr", "range_val", "field_name", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const QueryExprBisonParser::rhs_number_type
  QueryExprBisonParser::yyrhs_[] =
  {
        29,     0,    -1,    30,     0,    -1,    31,    -1,    30,    31,
      -1,    39,    -1,    35,    -1,    36,    -1,    32,    -1,    33,
      -1,    34,    -1,    37,    -1,    21,    30,    22,    -1,    18,
      31,    -1,    17,    31,    -1,    31,     9,    31,    -1,    31,
      10,    31,    -1,    46,    21,    38,    22,    -1,    31,    -1,
      38,    31,    -1,    40,    -1,    46,    40,    -1,    41,    -1,
      41,    19,     3,    -1,    41,    19,     4,    -1,    42,    -1,
      43,    -1,    44,    -1,     5,    -1,     6,    -1,     7,    -1,
       8,    -1,     8,    20,     3,    -1,     3,    -1,     4,    -1,
      12,    45,    -1,    15,    45,    -1,    13,    45,    -1,    16,
      45,    -1,    14,    45,    -1,    23,    45,    11,    45,    24,
      -1,    23,    45,    11,    45,    25,    -1,    26,    45,    11,
      45,    25,    -1,    26,    45,    11,    45,    24,    -1,     3,
      -1,     4,    -1,     5,    -1,     6,    -1,     7,    -1,     5,
      27,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  QueryExprBisonParser::yyprhs_[] =
  {
         0,     0,     3,     6,     8,    11,    13,    15,    17,    19,
      21,    23,    25,    29,    32,    35,    39,    43,    48,    50,
      53,    55,    58,    60,    64,    68,    70,    72,    74,    76,
      78,    80,    82,    86,    88,    90,    93,    96,    99,   102,
     105,   111,   117,   123,   129,   131,   133,   135,   137,   139
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  QueryExprBisonParser::yyrline_[] =
  {
         0,   103,   103,   109,   110,   115,   116,   117,   118,   119,
     120,   121,   123,   128,   134,   140,   145,   150,   157,   161,
     166,   172,   180,   184,   190,   197,   198,   199,   202,   204,
     206,   209,   213,   219,   224,   229,   231,   233,   235,   237,
     239,   243,   247,   251,   256,   261,   266,   267,   268,   273
  };

  // Print the state stack on the debug stream.
  void
  QueryExprBisonParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  QueryExprBisonParser::yy_reduce_print_ (int yyrule)
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
  QueryExprBisonParser::token_number_type
  QueryExprBisonParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      21,    22,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    27,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    23,     2,    24,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    26,     2,    25,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int QueryExprBisonParser::yyeof_ = 0;
  const int QueryExprBisonParser::yylast_ = 157;
  const int QueryExprBisonParser::yynnts_ = 19;
  const int QueryExprBisonParser::yyempty_ = -2;
  const int QueryExprBisonParser::yyfinal_ = 50;
  const int QueryExprBisonParser::yyterror_ = 1;
  const int QueryExprBisonParser::yyerrcode_ = 256;
  const int QueryExprBisonParser::yyntokens_ = 28;

  const unsigned int QueryExprBisonParser::yyuser_token_number_max_ = 275;
  const QueryExprBisonParser::token_number_type QueryExprBisonParser::yyundef_token_ = 2;


} // fx_bison

/* Line 1136 of lalr1.cc  */
#line 1558 "QueryExprBisonParser.cc"


/* Line 1138 of lalr1.cc  */
#line 278 "QueryExprBisonParser.yy"
 

/** Additional Code */
void fx_bison::QueryExprBisonParser::error(
        const fx_bison::QueryExprBisonParser::location_type& l,
        const std::string& m)
{
    exprParser.fail(l, m);
}

