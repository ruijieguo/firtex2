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
#line 1 "DateTimeBisonParser.yy"

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



/* Line 293 of lalr1.cc  */
#line 59 "DateTimeBisonParser.cc"


#include "DateTimeBisonParser.hh"

/* User implementation prologue.  */

/* Line 299 of lalr1.cc  */
#line 65 "DateTimeBisonParser.yy"

#include "firtex/analyzer/DateTimeLexer.h"
#include "firtex/analyzer/DateTimeAnalyzer.h"

/**
 * This "connects" the bison parser to the flex scanner class  object.
 * It defines the yylex() function call to pull the next token from the
 * current lexer object. 
 */
#undef yylex
#define yylex lexer.lex


/* Line 299 of lalr1.cc  */
#line 82 "DateTimeBisonParser.cc"

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
#line 168 "DateTimeBisonParser.cc"

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  DateTimeBisonParser::yytnamerr_ (const char *yystr)
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
  DateTimeBisonParser::DateTimeBisonParser (FX_NS(analyzer)::DateTimeLexer& lexer_yyarg, int64_t& result_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      lexer (lexer_yyarg),
      result (result_yyarg)
  {
  }

  DateTimeBisonParser::~DateTimeBisonParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  DateTimeBisonParser::yy_symbol_value_print_ (int yytype,
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
  DateTimeBisonParser::yy_symbol_print_ (int yytype,
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
  DateTimeBisonParser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  DateTimeBisonParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  DateTimeBisonParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  DateTimeBisonParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  DateTimeBisonParser::debug_level_type
  DateTimeBisonParser::debug_level () const
  {
    return yydebug_;
  }

  void
  DateTimeBisonParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  inline bool
  DateTimeBisonParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  DateTimeBisonParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  DateTimeBisonParser::parse ()
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
#line 80 "DateTimeBisonParser.yy"
    {
                     /**
                      * The date/time format defined in the ISO 8601 standard:
                      * 2008-01-01T12:00:00Z
                      */
                     if (FX_NS(utility)::DateTime::isValid((yysemantic_stack_[(13) - (1)].intVal), (yysemantic_stack_[(13) - (3)].intVal), (yysemantic_stack_[(13) - (5)].intVal), (yysemantic_stack_[(13) - (7)].intVal), (yysemantic_stack_[(13) - (9)].intVal), (yysemantic_stack_[(13) - (11)].intVal)))
                     {
                         (yyval.intVal) = DateTimeAnalyzer::makeTime((yysemantic_stack_[(13) - (1)].intVal), (yysemantic_stack_[(13) - (3)].intVal), (yysemantic_stack_[(13) - (5)].intVal), (yysemantic_stack_[(13) - (7)].intVal), (yysemantic_stack_[(13) - (9)].intVal), (yysemantic_stack_[(13) - (11)].intVal));
                         result = (yyval.intVal);
                     }
                     else
                     {
                         YYERROR; 
                     }
                 }
    break;

  case 3:

/* Line 690 of lalr1.cc  */
#line 96 "DateTimeBisonParser.yy"
    {
                     /**
                      * The date/time format defined in RFC 1123:
                      * Sat, 1 Jan 2005 12:00:00 GMT
                      */
                     if (FX_NS(utility)::DateTime::isValid((yysemantic_stack_[(12) - (5)].intVal), (yysemantic_stack_[(12) - (4)].intVal), (yysemantic_stack_[(12) - (3)].intVal), (yysemantic_stack_[(12) - (6)].intVal), (yysemantic_stack_[(12) - (8)].intVal), (yysemantic_stack_[(12) - (10)].intVal)))
                     {
                         (yyval.intVal) = DateTimeAnalyzer::makeTime((yysemantic_stack_[(12) - (5)].intVal), (yysemantic_stack_[(12) - (4)].intVal), (yysemantic_stack_[(12) - (3)].intVal), (yysemantic_stack_[(12) - (6)].intVal), (yysemantic_stack_[(12) - (8)].intVal), (yysemantic_stack_[(12) - (10)].intVal));
                         result = (yyval.intVal);
                     }
                     else
                     {
                         YYERROR; 
                     }
                 }
    break;

  case 4:

/* Line 690 of lalr1.cc  */
#line 112 "DateTimeBisonParser.yy"
    {
                     /**
                      * The date/time format defined in RFC 850 (obsoleted by RFC 1036).
                      *   Saturday, 1-Jan-05 11:00:00 GMT
                      */
                     if (FX_NS(utility)::DateTime::isValid((yysemantic_stack_[(14) - (7)].intVal), (yysemantic_stack_[(14) - (5)].intVal), (yysemantic_stack_[(14) - (3)].intVal), (yysemantic_stack_[(14) - (8)].intVal), (yysemantic_stack_[(14) - (10)].intVal), (yysemantic_stack_[(14) - (12)].intVal)))
                     {
                         (yyval.intVal) = DateTimeAnalyzer::makeTime((yysemantic_stack_[(14) - (7)].intVal), (yysemantic_stack_[(14) - (5)].intVal), (yysemantic_stack_[(14) - (3)].intVal), (yysemantic_stack_[(14) - (8)].intVal), (yysemantic_stack_[(14) - (10)].intVal), (yysemantic_stack_[(14) - (12)].intVal));
                         result = (yyval.intVal);
                     }
                     else
                     {
                         YYERROR; 
                     }
                 }
    break;

  case 5:

/* Line 690 of lalr1.cc  */
#line 128 "DateTimeBisonParser.yy"
    {
                     /** format: 2008/1/1 12:00:00 */
                     if (FX_NS(utility)::DateTime::isValid((yysemantic_stack_[(11) - (1)].intVal), (yysemantic_stack_[(11) - (3)].intVal), (yysemantic_stack_[(11) - (5)].intVal), (yysemantic_stack_[(11) - (6)].intVal), (yysemantic_stack_[(11) - (8)].intVal), (yysemantic_stack_[(11) - (10)].intVal)))
                     {
                         (yyval.intVal) = DateTimeAnalyzer::makeTime((yysemantic_stack_[(11) - (1)].intVal), (yysemantic_stack_[(11) - (3)].intVal), (yysemantic_stack_[(11) - (5)].intVal), (yysemantic_stack_[(11) - (6)].intVal), (yysemantic_stack_[(11) - (8)].intVal), (yysemantic_stack_[(11) - (10)].intVal));
                         result = (yyval.intVal);
                     }
                     else
                     {
                         YYERROR; 
                     }
                 }
    break;

  case 6:

/* Line 690 of lalr1.cc  */
#line 141 "DateTimeBisonParser.yy"
    {
                   /** simple, sortable date/time format: 2008-01-01 12:00:00 */
                   if (FX_NS(utility)::DateTime::isValid((yysemantic_stack_[(11) - (1)].intVal), (yysemantic_stack_[(11) - (3)].intVal), (yysemantic_stack_[(11) - (5)].intVal), (yysemantic_stack_[(11) - (6)].intVal), (yysemantic_stack_[(11) - (8)].intVal), (yysemantic_stack_[(11) - (10)].intVal)))
                   {
                       (yyval.intVal) = DateTimeAnalyzer::makeTime((yysemantic_stack_[(11) - (1)].intVal), (yysemantic_stack_[(11) - (3)].intVal), (yysemantic_stack_[(11) - (5)].intVal), (yysemantic_stack_[(11) - (6)].intVal), (yysemantic_stack_[(11) - (8)].intVal), (yysemantic_stack_[(11) - (10)].intVal));
                         result = (yyval.intVal);
                   }
                   else
                   {
                       YYERROR;
                   }
               }
    break;

  case 7:

/* Line 690 of lalr1.cc  */
#line 154 "DateTimeBisonParser.yy"
    {
                     /** 
                      * The date/time format produced by the ANSI C asctime() function:
                      * Sat Jan 01 12:00:00 2008
                      */
                     if (FX_NS(utility)::DateTime::isValid((yysemantic_stack_[(10) - (9)].intVal), (yysemantic_stack_[(10) - (2)].intVal), (yysemantic_stack_[(10) - (3)].intVal), (yysemantic_stack_[(10) - (4)].intVal), (yysemantic_stack_[(10) - (6)].intVal), (yysemantic_stack_[(10) - (8)].intVal)))
                     {
                         (yyval.intVal) = DateTimeAnalyzer::makeTime((yysemantic_stack_[(10) - (9)].intVal), (yysemantic_stack_[(10) - (2)].intVal), (yysemantic_stack_[(10) - (3)].intVal), (yysemantic_stack_[(10) - (4)].intVal), (yysemantic_stack_[(10) - (6)].intVal), (yysemantic_stack_[(10) - (8)].intVal));
                         result = (yyval.intVal);
                     }
                     else
                     {
                         YYERROR; 
                     }
                 }
    break;

  case 8:

/* Line 690 of lalr1.cc  */
#line 170 "DateTimeBisonParser.yy"
    {
                     /** format: 2008年1月1日 12点00分00秒 */
                     if (FX_NS(utility)::DateTime::isValid((yysemantic_stack_[(13) - (1)].intVal), (yysemantic_stack_[(13) - (3)].intVal), (yysemantic_stack_[(13) - (5)].intVal), (yysemantic_stack_[(13) - (7)].intVal), (yysemantic_stack_[(13) - (9)].intVal), (yysemantic_stack_[(13) - (11)].intVal)))
                     {
                         (yyval.intVal) = DateTimeAnalyzer::makeTime((yysemantic_stack_[(13) - (1)].intVal), (yysemantic_stack_[(13) - (3)].intVal), (yysemantic_stack_[(13) - (5)].intVal), (yysemantic_stack_[(13) - (7)].intVal), (yysemantic_stack_[(13) - (9)].intVal), (yysemantic_stack_[(13) - (11)].intVal));
                         result = (yyval.intVal);
                     }
                     else
                     {
                         YYERROR; 
                     }
                 }
    break;



/* Line 690 of lalr1.cc  */
#line 608 "DateTimeBisonParser.cc"
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
  DateTimeBisonParser::yysyntax_error_ (int yystate, int yytoken)
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
  const signed char DateTimeBisonParser::yypact_ninf_ = -7;
  const signed char
  DateTimeBisonParser::yypact_[] =
  {
         9,     1,    -4,     2,     3,     7,    13,    14,    15,    -7,
      -3,    11,    -6,    17,    -1,    18,    19,    20,    16,    21,
      22,    24,    -2,    23,    25,    26,    28,    29,    30,    27,
      31,    32,    33,    34,    37,    40,    35,    36,    42,    43,
      44,    39,    41,    48,    49,    50,    45,    46,    53,    54,
      51,    47,    58,    56,    59,    63,    64,    62,    65,    -7,
      61,    60,    -7,    -7,    66,    52,    70,    69,    73,    74,
      -7,    71,    -7,    -7,    75,    -7
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  DateTimeBisonParser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       0,     0,     5,     6,     0,     0,     0,     0,     0,     0,
       3,     0,     2,     8,     0,     4
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  DateTimeBisonParser::yypgoto_[] =
  {
        -7,    -7
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  DateTimeBisonParser::yydefgoto_[] =
  {
        -1,     3
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char DateTimeBisonParser::yytable_ninf_ = -1;
  const unsigned char
  DateTimeBisonParser::yytable_[] =
  {
         7,    28,     9,    19,    15,    17,    10,    20,     4,     5,
      11,     6,     1,     8,    29,     2,    12,    13,    14,    16,
      18,    21,    22,    23,    25,    24,    26,    27,    31,    32,
      36,     0,     0,     0,    37,    30,    33,    40,    34,    35,
      41,    38,    39,    42,    43,    45,    46,    47,    48,    44,
      49,    50,    51,    52,    53,    54,    55,    56,    59,    60,
      57,    58,    61,    62,    63,    64,    66,    69,    65,    67,
      70,    68,    71,    72,    73,    75,    74
  };

  /* YYCHECK.  */
  const signed char
  DateTimeBisonParser::yycheck_[] =
  {
         4,     3,     0,     4,     7,    11,     3,     8,     7,     8,
       3,    10,     3,    17,    16,     6,     3,     3,     3,     8,
       3,     3,     3,     3,     3,     9,     4,     3,     3,     3,
       3,    -1,    -1,    -1,     3,    12,     8,     3,     9,     9,
       3,     9,     9,     3,     9,     3,     3,     3,     9,    13,
       9,     3,     3,     3,     9,     9,     3,     3,     0,     3,
       9,    14,     3,     0,     0,     3,     5,    15,     3,     9,
       0,     5,     3,     0,     0,     0,     5
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  DateTimeBisonParser::yystos_[] =
  {
         0,     3,     6,    19,     7,     8,    10,     4,    17,     0,
       3,     3,     3,     3,     3,     7,     8,    11,     3,     4,
       8,     3,     3,     3,     9,     3,     4,     3,     3,    16,
      12,     3,     3,     8,     9,     9,     3,     3,     9,     9,
       3,     3,     3,     9,    13,     3,     3,     3,     9,     9,
       3,     3,     3,     9,     9,     3,     3,     9,    14,     0,
       3,     3,     0,     0,     3,     3,     5,     9,     5,    15,
       0,     3,     0,     0,     5,     0
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  DateTimeBisonParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,    84,    44
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  DateTimeBisonParser::yyr1_[] =
  {
         0,    18,    19,    19,    19,    19,    19,    19,    19
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  DateTimeBisonParser::yyr2_[] =
  {
         0,     2,    13,    12,    14,    11,    11,    10,    13
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const DateTimeBisonParser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "\"integer\"", "TOK_MONTH",
  "TOK_TZD", "TOK_WEEK", "TOK_SLASH", "TOK_DASH", "TOK_COLON",
  "TOK_YEAR_SEP", "TOK_MONTH_SEP", "TOK_DAY_SEP", "TOK_HOUR_SEP",
  "TOK_MIN_SEP", "TOK_SECOND_SEP", "'T'", "','", "$accept",
  "date_time_expr", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const DateTimeBisonParser::rhs_number_type
  DateTimeBisonParser::yyrhs_[] =
  {
        19,     0,    -1,     3,     8,     3,     8,     3,    16,     3,
       9,     3,     9,     3,     5,     0,    -1,     6,    17,     3,
       4,     3,     3,     9,     3,     9,     3,     5,     0,    -1,
       6,    17,     3,     8,     4,     8,     3,     3,     9,     3,
       9,     3,     5,     0,    -1,     3,     7,     3,     7,     3,
       3,     9,     3,     9,     3,     0,    -1,     3,     8,     3,
       8,     3,     3,     9,     3,     9,     3,     0,    -1,     6,
       4,     3,     3,     9,     3,     9,     3,     3,     0,    -1,
       3,    10,     3,    11,     3,    12,     3,    13,     3,    14,
       3,    15,     0,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  DateTimeBisonParser::yyprhs_[] =
  {
         0,     0,     3,    17,    30,    45,    57,    69,    80
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  DateTimeBisonParser::yyrline_[] =
  {
         0,    79,    79,    95,   111,   127,   140,   153,   169
  };

  // Print the state stack on the debug stream.
  void
  DateTimeBisonParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  DateTimeBisonParser::yy_reduce_print_ (int yyrule)
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
  DateTimeBisonParser::token_number_type
  DateTimeBisonParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    17,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    16,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int DateTimeBisonParser::yyeof_ = 0;
  const int DateTimeBisonParser::yylast_ = 76;
  const int DateTimeBisonParser::yynnts_ = 2;
  const int DateTimeBisonParser::yyempty_ = -2;
  const int DateTimeBisonParser::yyfinal_ = 9;
  const int DateTimeBisonParser::yyterror_ = 1;
  const int DateTimeBisonParser::yyerrcode_ = 256;
  const int DateTimeBisonParser::yyntokens_ = 18;

  const unsigned int DateTimeBisonParser::yyuser_token_number_max_ = 270;
  const DateTimeBisonParser::token_number_type DateTimeBisonParser::yyundef_token_ = 2;


} // fx_bison

/* Line 1136 of lalr1.cc  */
#line 1131 "DateTimeBisonParser.cc"


/* Line 1138 of lalr1.cc  */
#line 183 "DateTimeBisonParser.yy"
 

/** Additional Code */

void fx_bison::DateTimeBisonParser::error(
        const fx_bison::DateTimeBisonParser::location_type& l,
        const std::string& m)
{
//    exprParser.fail(l, m);
}


