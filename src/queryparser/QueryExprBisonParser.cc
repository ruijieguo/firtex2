
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



/* Line 311 of lalr1.cc  */
#line 60 "QueryExprBisonParser.cc"


#include "y.tab.h"

/* User implementation prologue.  */

/* Line 317 of lalr1.cc  */
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


/* Line 317 of lalr1.cc  */
#line 83 "QueryExprBisonParser.cc"

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
#line 152 "QueryExprBisonParser.cc"
#if YYERROR_VERBOSE

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

#endif

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
#line 260 "QueryExprBisonParser.cc"
	break;
      case 4: /* "\"float\"" */

/* Line 480 of lalr1.cc  */
#line 75 "QueryExprBisonParser.yy"
	{delete (yyvaluep->numberVal);};

/* Line 480 of lalr1.cc  */
#line 269 "QueryExprBisonParser.cc"
	break;
      case 5: /* "\"identifier\"" */

/* Line 480 of lalr1.cc  */
#line 76 "QueryExprBisonParser.yy"
	{delete (yyvaluep->stringVal);};

/* Line 480 of lalr1.cc  */
#line 278 "QueryExprBisonParser.cc"
	break;
      case 6: /* "\"string NOT including symbols\"" */

/* Line 480 of lalr1.cc  */
#line 76 "QueryExprBisonParser.yy"
	{delete (yyvaluep->stringVal);};

/* Line 480 of lalr1.cc  */
#line 287 "QueryExprBisonParser.cc"
	break;
      case 7: /* "\"single quoted string\"" */

/* Line 480 of lalr1.cc  */
#line 76 "QueryExprBisonParser.yy"
	{delete (yyvaluep->stringVal);};

/* Line 480 of lalr1.cc  */
#line 296 "QueryExprBisonParser.cc"
	break;
      case 8: /* "\"double quoted string\"" */

/* Line 480 of lalr1.cc  */
#line 76 "QueryExprBisonParser.yy"
	{delete (yyvaluep->stringVal);};

/* Line 480 of lalr1.cc  */
#line 305 "QueryExprBisonParser.cc"
	break;
      case 31: /* "bool_expr" */

/* Line 480 of lalr1.cc  */
#line 77 "QueryExprBisonParser.yy"
	{delete (yyvaluep->queryExprVal);};

/* Line 480 of lalr1.cc  */
#line 314 "QueryExprBisonParser.cc"
	break;
      case 32: /* "expr" */

/* Line 480 of lalr1.cc  */
#line 77 "QueryExprBisonParser.yy"
	{delete (yyvaluep->queryExprVal);};

/* Line 480 of lalr1.cc  */
#line 323 "QueryExprBisonParser.cc"
	break;
      case 33: /* "group_expr" */

/* Line 480 of lalr1.cc  */
#line 78 "QueryExprBisonParser.yy"
	{delete (yyvaluep->queryExprVal);};

/* Line 480 of lalr1.cc  */
#line 332 "QueryExprBisonParser.cc"
	break;
      case 34: /* "req_expr" */

/* Line 480 of lalr1.cc  */
#line 78 "QueryExprBisonParser.yy"
	{delete (yyvaluep->queryExprVal);};

/* Line 480 of lalr1.cc  */
#line 341 "QueryExprBisonParser.cc"
	break;
      case 35: /* "proh_expr" */

/* Line 480 of lalr1.cc  */
#line 78 "QueryExprBisonParser.yy"
	{delete (yyvaluep->queryExprVal);};

/* Line 480 of lalr1.cc  */
#line 350 "QueryExprBisonParser.cc"
	break;
      case 36: /* "any_expr" */

/* Line 480 of lalr1.cc  */
#line 77 "QueryExprBisonParser.yy"
	{delete (yyvaluep->queryExprVal);};

/* Line 480 of lalr1.cc  */
#line 359 "QueryExprBisonParser.cc"
	break;
      case 37: /* "and_expr" */

/* Line 480 of lalr1.cc  */
#line 77 "QueryExprBisonParser.yy"
	{delete (yyvaluep->queryExprVal);};

/* Line 480 of lalr1.cc  */
#line 368 "QueryExprBisonParser.cc"
	break;
      case 38: /* "or_expr" */

/* Line 480 of lalr1.cc  */
#line 77 "QueryExprBisonParser.yy"
	{delete (yyvaluep->queryExprVal);};

/* Line 480 of lalr1.cc  */
#line 377 "QueryExprBisonParser.cc"
	break;
      case 41: /* "atom_expr" */

/* Line 480 of lalr1.cc  */
#line 79 "QueryExprBisonParser.yy"
	{delete (yyvaluep->termExprVal);};

/* Line 480 of lalr1.cc  */
#line 386 "QueryExprBisonParser.cc"
	break;
      case 42: /* "boost_term" */

/* Line 480 of lalr1.cc  */
#line 81 "QueryExprBisonParser.yy"
	{delete (yyvaluep->termExprVal);};

/* Line 480 of lalr1.cc  */
#line 395 "QueryExprBisonParser.cc"
	break;
      case 43: /* "atom_term" */

/* Line 480 of lalr1.cc  */
#line 81 "QueryExprBisonParser.yy"
	{delete (yyvaluep->termExprVal);};

/* Line 480 of lalr1.cc  */
#line 404 "QueryExprBisonParser.cc"
	break;
      case 44: /* "term_expr" */

/* Line 480 of lalr1.cc  */
#line 81 "QueryExprBisonParser.yy"
	{delete (yyvaluep->termExprVal);};

/* Line 480 of lalr1.cc  */
#line 413 "QueryExprBisonParser.cc"
	break;
      case 45: /* "phrase_expr" */

/* Line 480 of lalr1.cc  */
#line 81 "QueryExprBisonParser.yy"
	{delete (yyvaluep->termExprVal);};

/* Line 480 of lalr1.cc  */
#line 422 "QueryExprBisonParser.cc"
	break;
      case 46: /* "range_expr" */

/* Line 480 of lalr1.cc  */
#line 81 "QueryExprBisonParser.yy"
	{delete (yyvaluep->termExprVal);};

/* Line 480 of lalr1.cc  */
#line 431 "QueryExprBisonParser.cc"
	break;
      case 47: /* "range_val" */

/* Line 480 of lalr1.cc  */
#line 80 "QueryExprBisonParser.yy"
	{delete (yyvaluep->stringVal);};

/* Line 480 of lalr1.cc  */
#line 440 "QueryExprBisonParser.cc"
	break;
      case 48: /* "field_name" */

/* Line 480 of lalr1.cc  */
#line 80 "QueryExprBisonParser.yy"
	{delete (yyvaluep->stringVal);};

/* Line 480 of lalr1.cc  */
#line 449 "QueryExprBisonParser.cc"
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
#line 104 "QueryExprBisonParser.yy"
    { 
    (yyval.queryExprVal) = (yysemantic_stack_[(2) - (1)].queryExprVal);
    exprParser.finalize((yyval.queryExprVal));
}
    break;

  case 4:

/* Line 678 of lalr1.cc  */
#line 111 "QueryExprBisonParser.yy"
    {
                (yyval.queryExprVal) = new BooleanQueryExpr((yysemantic_stack_[(2) - (1)].queryExprVal), (yysemantic_stack_[(2) - (2)].queryExprVal), exprParser.getDefaultOp()); 
            }
    break;

  case 5:

/* Line 678 of lalr1.cc  */
#line 115 "QueryExprBisonParser.yy"
    {(yyval.queryExprVal) = (yysemantic_stack_[(1) - (1)].termExprVal);}
    break;

  case 13:

/* Line 678 of lalr1.cc  */
#line 125 "QueryExprBisonParser.yy"
    {
    (yyval.queryExprVal) = (yysemantic_stack_[(3) - (2)].queryExprVal);
}
    break;

  case 14:

/* Line 678 of lalr1.cc  */
#line 130 "QueryExprBisonParser.yy"
    {
    (yysemantic_stack_[(2) - (2)].queryExprVal)->setRequired(true);
    (yyval.queryExprVal) = (yysemantic_stack_[(2) - (2)].queryExprVal);
}
    break;

  case 15:

/* Line 678 of lalr1.cc  */
#line 136 "QueryExprBisonParser.yy"
    {
    (yysemantic_stack_[(2) - (2)].queryExprVal)->setProhibited(true);
    (yyval.queryExprVal) = (yysemantic_stack_[(2) - (2)].queryExprVal);
}
    break;

  case 16:

/* Line 678 of lalr1.cc  */
#line 142 "QueryExprBisonParser.yy"
    {
              (yyval.queryExprVal) = new AnyQueryExpr();
          }
    break;

  case 17:

/* Line 678 of lalr1.cc  */
#line 147 "QueryExprBisonParser.yy"
    {
    (yyval.queryExprVal) = new BooleanQueryExpr((yysemantic_stack_[(3) - (1)].queryExprVal), (yysemantic_stack_[(3) - (3)].queryExprVal), QueryExpr::OP_AND);
}
    break;

  case 18:

/* Line 678 of lalr1.cc  */
#line 152 "QueryExprBisonParser.yy"
    {
    (yyval.queryExprVal) = new BooleanQueryExpr((yysemantic_stack_[(3) - (1)].queryExprVal), (yysemantic_stack_[(3) - (3)].queryExprVal), QueryExpr::OP_OR);
}
    break;

  case 19:

/* Line 678 of lalr1.cc  */
#line 157 "QueryExprBisonParser.yy"
    {
                      (yysemantic_stack_[(4) - (3)].queryExprVal)->setFieldName(*(yysemantic_stack_[(4) - (1)].stringVal));
                      delete (yysemantic_stack_[(4) - (1)].stringVal);
                      (yyval.queryExprVal) = (yysemantic_stack_[(4) - (3)].queryExprVal);
                  }
    break;

  case 20:

/* Line 678 of lalr1.cc  */
#line 164 "QueryExprBisonParser.yy"
    {
                (yyval.queryExprVal) = (yysemantic_stack_[(1) - (1)].queryExprVal);
            }
    break;

  case 21:

/* Line 678 of lalr1.cc  */
#line 168 "QueryExprBisonParser.yy"
    {
                (yyval.queryExprVal) = new BooleanQueryExpr((yysemantic_stack_[(2) - (1)].queryExprVal), (yysemantic_stack_[(2) - (2)].queryExprVal), exprParser.getDefaultOp());
            }
    break;

  case 22:

/* Line 678 of lalr1.cc  */
#line 173 "QueryExprBisonParser.yy"
    {
                const std::string &defaultField = exprParser.getDefaultField();
                (yysemantic_stack_[(1) - (1)].termExprVal)->setFieldName(defaultField);
                (yyval.termExprVal) = (yysemantic_stack_[(1) - (1)].termExprVal);
              }
    break;

  case 23:

/* Line 678 of lalr1.cc  */
#line 179 "QueryExprBisonParser.yy"
    {
                std::string* fieldName = (yysemantic_stack_[(2) - (1)].stringVal);
                (yysemantic_stack_[(2) - (2)].termExprVal)->setFieldName(*fieldName);
                delete (yysemantic_stack_[(2) - (1)].stringVal);
                (yyval.termExprVal) = (yysemantic_stack_[(2) - (2)].termExprVal);
            }
    break;

  case 24:

/* Line 678 of lalr1.cc  */
#line 187 "QueryExprBisonParser.yy"
    {
                 (yyval.termExprVal) = (yysemantic_stack_[(1) - (1)].termExprVal);
             }
    break;

  case 25:

/* Line 678 of lalr1.cc  */
#line 191 "QueryExprBisonParser.yy"
    {
                 (yysemantic_stack_[(3) - (1)].termExprVal)->setBoost((float)(yysemantic_stack_[(3) - (3)].numberVal)->m_i64Val);
                 delete (yysemantic_stack_[(3) - (3)].numberVal);
                 (yyval.termExprVal) = (yysemantic_stack_[(3) - (1)].termExprVal);
             }
    break;

  case 26:

/* Line 678 of lalr1.cc  */
#line 197 "QueryExprBisonParser.yy"
    {
                 (yysemantic_stack_[(3) - (1)].termExprVal)->setBoost((yysemantic_stack_[(3) - (3)].numberVal)->m_fVal);
                 delete (yysemantic_stack_[(3) - (3)].numberVal);
                 (yyval.termExprVal) = (yysemantic_stack_[(3) - (1)].termExprVal);
             }
    break;

  case 29:

/* Line 678 of lalr1.cc  */
#line 206 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = (yysemantic_stack_[(1) - (1)].termExprVal); }
    break;

  case 30:

/* Line 678 of lalr1.cc  */
#line 209 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = new TermQueryExpr((yysemantic_stack_[(1) - (1)].stringVal)); }
    break;

  case 31:

/* Line 678 of lalr1.cc  */
#line 211 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = new TermQueryExpr((yysemantic_stack_[(1) - (1)].stringVal)); }
    break;

  case 32:

/* Line 678 of lalr1.cc  */
#line 213 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = new TermQueryExpr((yysemantic_stack_[(1) - (1)].stringVal)); }
    break;

  case 33:

/* Line 678 of lalr1.cc  */
#line 216 "QueryExprBisonParser.yy"
    {
                  (yyval.termExprVal) = new PhraseQueryExpr((yysemantic_stack_[(1) - (1)].stringVal)); 
              }
    break;

  case 34:

/* Line 678 of lalr1.cc  */
#line 220 "QueryExprBisonParser.yy"
    {
                (yyval.termExprVal) = new PhraseQueryExpr((yysemantic_stack_[(3) - (1)].stringVal), (int32_t)(yysemantic_stack_[(3) - (3)].numberVal)->m_i64Val); 
                delete (yysemantic_stack_[(3) - (3)].numberVal);
            }
    break;

  case 35:

/* Line 678 of lalr1.cc  */
#line 226 "QueryExprBisonParser.yy"
    { 
                  (yyval.termExprVal) = new TermQueryExpr(new std::string((yysemantic_stack_[(1) - (1)].numberVal)->m_sValue)); 
                  delete (yysemantic_stack_[(1) - (1)].numberVal);
              }
    break;

  case 36:

/* Line 678 of lalr1.cc  */
#line 231 "QueryExprBisonParser.yy"
    {
                  (yyval.termExprVal) = new TermQueryExpr(new std::string((yysemantic_stack_[(1) - (1)].numberVal)->m_sValue)); 
                  delete (yysemantic_stack_[(1) - (1)].numberVal);
              }
    break;

  case 37:

/* Line 678 of lalr1.cc  */
#line 236 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = new TermQueryExpr((yysemantic_stack_[(2) - (2)].stringVal)); }
    break;

  case 38:

/* Line 678 of lalr1.cc  */
#line 238 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = new RangeQueryExpr(NULL, false, (yysemantic_stack_[(2) - (2)].stringVal), false);}
    break;

  case 39:

/* Line 678 of lalr1.cc  */
#line 240 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = new RangeQueryExpr(NULL, false, (yysemantic_stack_[(2) - (2)].stringVal), true);}
    break;

  case 40:

/* Line 678 of lalr1.cc  */
#line 242 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = new RangeQueryExpr((yysemantic_stack_[(2) - (2)].stringVal), false, NULL, false);}
    break;

  case 41:

/* Line 678 of lalr1.cc  */
#line 244 "QueryExprBisonParser.yy"
    { (yyval.termExprVal) = new RangeQueryExpr((yysemantic_stack_[(2) - (2)].stringVal), true, NULL, false);}
    break;

  case 42:

/* Line 678 of lalr1.cc  */
#line 246 "QueryExprBisonParser.yy"
    { 
                  (yyval.termExprVal) = new RangeQueryExpr((yysemantic_stack_[(5) - (2)].stringVal), true, (yysemantic_stack_[(5) - (4)].stringVal), true);
              }
    break;

  case 43:

/* Line 678 of lalr1.cc  */
#line 250 "QueryExprBisonParser.yy"
    { 
                  (yyval.termExprVal) = new RangeQueryExpr((yysemantic_stack_[(5) - (2)].stringVal), true, (yysemantic_stack_[(5) - (4)].stringVal), false);
              }
    break;

  case 44:

/* Line 678 of lalr1.cc  */
#line 254 "QueryExprBisonParser.yy"
    {
                  (yyval.termExprVal) = new RangeQueryExpr((yysemantic_stack_[(5) - (2)].stringVal), false, (yysemantic_stack_[(5) - (4)].stringVal), false);
              }
    break;

  case 45:

/* Line 678 of lalr1.cc  */
#line 258 "QueryExprBisonParser.yy"
    { 
                  (yyval.termExprVal) = new RangeQueryExpr((yysemantic_stack_[(5) - (2)].stringVal), false, (yysemantic_stack_[(5) - (4)].stringVal), true);
              }
    break;

  case 46:

/* Line 678 of lalr1.cc  */
#line 263 "QueryExprBisonParser.yy"
    {
                  (yyval.stringVal) = new std::string((yysemantic_stack_[(1) - (1)].numberVal)->m_sValue);
                  delete (yysemantic_stack_[(1) - (1)].numberVal);
              }
    break;

  case 47:

/* Line 678 of lalr1.cc  */
#line 268 "QueryExprBisonParser.yy"
    {
                  (yyval.stringVal) = new std::string((yysemantic_stack_[(1) - (1)].numberVal)->m_sValue);
                  delete (yysemantic_stack_[(1) - (1)].numberVal);
              }
    break;

  case 50:

/* Line 678 of lalr1.cc  */
#line 275 "QueryExprBisonParser.yy"
    {
                  (yyval.stringVal) = (yysemantic_stack_[(1) - (1)].stringVal);
              }
    break;

  case 51:

/* Line 678 of lalr1.cc  */
#line 280 "QueryExprBisonParser.yy"
    { 
                  (yyval.stringVal) = (yysemantic_stack_[(2) - (1)].stringVal);
              }
    break;



/* Line 678 of lalr1.cc  */
#line 983 "QueryExprBisonParser.cc"
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
  QueryExprBisonParser::yysyntax_error_ (int yystate, int tok)
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
  const signed char QueryExprBisonParser::yypact_ninf_ = -13;
  const short int
  QueryExprBisonParser::yypact_[] =
  {
       112,   -13,   -13,     1,   -13,   -13,    21,    -1,    -1,    -1,
      -1,    -1,   112,   112,   -13,   112,    -1,    -1,    38,    18,
       3,   -13,   -13,   -13,   -13,   -13,   -13,   -13,   -13,   -13,
      27,   -13,   -13,   -13,   137,   -13,    45,   -13,   -13,   -13,
     -13,   -13,   -13,   -13,   -13,   -13,   -13,   -13,   -13,    62,
      39,    40,   -13,   -13,     3,   112,   112,    11,   -13,   112,
     -13,   -13,   -13,    -1,    -1,   -13,    43,   -13,   -13,     3,
      87,    -6,     2,   -13,     3,   -13,   -13,   -13,   -13
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  QueryExprBisonParser::yydefact_[] =
  {
         0,    35,    36,    30,    31,    32,    33,     0,     0,     0,
       0,     0,     0,     0,    16,     0,     0,     0,     0,     0,
       3,     9,    10,    11,     6,     7,     8,    12,     5,    22,
      24,    27,    28,    29,     0,    51,     0,    46,    47,    48,
      49,    50,    37,    39,    41,    38,    40,    15,    14,     0,
       0,     0,     1,     2,     4,     0,     0,     0,    30,     0,
      23,    34,    13,     0,     0,    17,    18,    25,    26,    20,
       0,     0,     0,    19,    21,    42,    43,    45,    44
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  QueryExprBisonParser::yypgoto_[] =
  {
       -13,   -13,    34,   -12,   -13,   -13,   -13,   -13,   -13,   -13,
     -13,   -13,   -13,    19,   -13,   -13,   -13,   -13,     0,   -13
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  QueryExprBisonParser::yydefgoto_[] =
  {
        -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    70,    28,    29,    30,    31,    32,    33,    42,    34
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char QueryExprBisonParser::yytable_ninf_ = -1;
  const unsigned char
  QueryExprBisonParser::yytable_[] =
  {
        47,    48,    37,    38,    39,    40,    41,    54,    43,    44,
      45,    46,    55,    56,    67,    68,    50,    51,    53,    75,
      76,     1,     2,     3,     4,     5,     6,    77,    78,    35,
       7,     8,     9,    10,    11,    12,    13,    54,    52,    14,
      15,    36,    16,    65,    66,    17,    57,    69,    61,    49,
      63,    64,    55,    60,     0,     0,     0,     0,    74,     0,
       0,     0,     0,    71,    72,     1,     2,     3,     4,     5,
       6,     0,     0,     0,     7,     8,     9,    10,    11,    12,
      13,     0,     0,    14,    15,    62,    16,     0,     0,    17,
       1,     2,     3,     4,     5,     6,     0,     0,     0,     7,
       8,     9,    10,    11,    12,    13,     0,     0,    14,    15,
      73,    16,     0,     0,    17,     1,     2,     3,     4,     5,
       6,     0,     0,     0,     7,     8,     9,    10,    11,    12,
      13,     0,     0,    14,    15,     0,    16,     0,     0,    17,
       1,     2,    58,     4,     5,     6,     0,     0,     0,     7,
       8,     9,    10,    11,     0,     0,     0,     0,     0,    59,
       0,    16,     0,     0,    17
  };

  /* YYCHECK.  */
  const signed char
  QueryExprBisonParser::yycheck_[] =
  {
        12,    13,     3,     4,     5,     6,     7,    19,     8,     9,
      10,    11,     9,    10,     3,     4,    16,    17,     0,    25,
      26,     3,     4,     5,     6,     7,     8,    25,    26,    28,
      12,    13,    14,    15,    16,    17,    18,    49,     0,    21,
      22,    20,    24,    55,    56,    27,    19,    59,     3,    15,
      11,    11,     9,    34,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    63,    64,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    21,    22,    23,    24,    -1,    -1,    27,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    17,    18,    -1,    -1,    21,    22,
      23,    24,    -1,    -1,    27,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    21,    22,    -1,    24,    -1,    -1,    27,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    24,    -1,    -1,    27
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  QueryExprBisonParser::yystos_[] =
  {
         0,     3,     4,     5,     6,     7,     8,    12,    13,    14,
      15,    16,    17,    18,    21,    22,    24,    27,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    41,    42,
      43,    44,    45,    46,    48,    28,    20,     3,     4,     5,
       6,     7,    47,    47,    47,    47,    47,    32,    32,    31,
      47,    47,     0,     0,    32,     9,    10,    19,     5,    22,
      42,     3,    23,    11,    11,    32,    32,     3,     4,    32,
      40,    47,    47,    23,    32,    25,    26,    25,    26
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  QueryExprBisonParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,    40,    41,    91,    93,   125,   123,    58
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  QueryExprBisonParser::yyr1_[] =
  {
         0,    29,    30,    31,    31,    32,    32,    32,    32,    32,
      32,    32,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    40,    41,    41,    42,    42,    42,    43,    43,    43,
      44,    44,    44,    45,    45,    46,    46,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    47,    47,    47,    47,
      47,    48
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  QueryExprBisonParser::yyr2_[] =
  {
         0,     2,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     2,     2,     1,     3,     3,     4,
       1,     2,     1,     2,     1,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     1,     2,     2,     2,
       2,     2,     5,     5,     5,     5,     1,     1,     1,     1,
       1,     2
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
  "TOK_PHRASE_SLOP", "TOK_QUERY_ANY", "'('", "')'", "'['", "']'", "'}'",
  "'{'", "':'", "$accept", "query_expr", "bool_expr", "expr", "group_expr",
  "req_expr", "proh_expr", "any_expr", "and_expr", "or_expr",
  "group_term_expr", "term_list", "atom_expr", "boost_term", "atom_term",
  "term_expr", "phrase_expr", "range_expr", "range_val", "field_name", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const QueryExprBisonParser::rhs_number_type
  QueryExprBisonParser::yyrhs_[] =
  {
        30,     0,    -1,    31,     0,    -1,    32,    -1,    31,    32,
      -1,    41,    -1,    36,    -1,    37,    -1,    38,    -1,    33,
      -1,    34,    -1,    35,    -1,    39,    -1,    22,    31,    23,
      -1,    18,    32,    -1,    17,    32,    -1,    21,    -1,    32,
       9,    32,    -1,    32,    10,    32,    -1,    48,    22,    40,
      23,    -1,    32,    -1,    40,    32,    -1,    42,    -1,    48,
      42,    -1,    43,    -1,    43,    19,     3,    -1,    43,    19,
       4,    -1,    44,    -1,    45,    -1,    46,    -1,     5,    -1,
       6,    -1,     7,    -1,     8,    -1,     8,    20,     3,    -1,
       3,    -1,     4,    -1,    12,    47,    -1,    15,    47,    -1,
      13,    47,    -1,    16,    47,    -1,    14,    47,    -1,    24,
      47,    11,    47,    25,    -1,    24,    47,    11,    47,    26,
      -1,    27,    47,    11,    47,    26,    -1,    27,    47,    11,
      47,    25,    -1,     3,    -1,     4,    -1,     5,    -1,     6,
      -1,     7,    -1,     5,    28,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  QueryExprBisonParser::yyprhs_[] =
  {
         0,     0,     3,     6,     8,    11,    13,    15,    17,    19,
      21,    23,    25,    27,    31,    34,    37,    39,    43,    47,
      52,    54,    57,    59,    62,    64,    68,    72,    74,    76,
      78,    80,    82,    84,    86,    90,    92,    94,    97,   100,
     103,   106,   109,   115,   121,   127,   133,   135,   137,   139,
     141,   143
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  QueryExprBisonParser::yyrline_[] =
  {
         0,   103,   103,   109,   110,   115,   116,   117,   118,   119,
     120,   121,   122,   124,   129,   135,   141,   146,   151,   156,
     163,   167,   172,   178,   186,   190,   196,   203,   204,   205,
     208,   210,   212,   215,   219,   225,   230,   235,   237,   239,
     241,   243,   245,   249,   253,   257,   262,   267,   272,   273,
     274,   279
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
      22,    23,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    28,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    24,     2,    25,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    27,     2,    26,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int QueryExprBisonParser::yyeof_ = 0;
  const int QueryExprBisonParser::yylast_ = 164;
  const int QueryExprBisonParser::yynnts_ = 20;
  const int QueryExprBisonParser::yyempty_ = -2;
  const int QueryExprBisonParser::yyfinal_ = 52;
  const int QueryExprBisonParser::yyterror_ = 1;
  const int QueryExprBisonParser::yyerrcode_ = 256;
  const int QueryExprBisonParser::yyntokens_ = 29;

  const unsigned int QueryExprBisonParser::yyuser_token_number_max_ = 276;
  const QueryExprBisonParser::token_number_type QueryExprBisonParser::yyundef_token_ = 2;


/* Line 1054 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // fx_bison

/* Line 1054 of lalr1.cc  */
#line 1491 "QueryExprBisonParser.cc"


/* Line 1056 of lalr1.cc  */
#line 284 "QueryExprBisonParser.yy"
 

/** Additional Code */
void fx_bison::QueryExprBisonParser::error(
        const fx_bison::QueryExprBisonParser::location_type& l,
        const std::string& m)
{
    exprParser.fail(l, m);
}

