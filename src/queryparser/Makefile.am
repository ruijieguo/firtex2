INCLUDES = -I $(top_srcdir)/include -I $(top_srcdir)/src

# this output override is needed because the c++ scanner is outputted into
# lex.yy.cc and the automake ylwrap script only checks for lex.yy.c
AM_LFLAGS=-olex.yy.c
AM_YFLAGS=-r state -v -g

noinst_LTLIBRARIES = libqueryparser.la

libqueryparser_la_SOURCES = \
	QueryExprBisonParser.yy\
	ExprBisonParser.yy\
	QueryExprLexer.ll\
	ExprLexer.ll\
	QueryExpr.cpp\
	QueryExprParser.cpp\
	ExprParser.cpp\
	ExprOptimizer.cpp\
	DefaultQueryBuilder.cpp\
	DefaultExprEvaluatorBuilder.cpp\
	QueryParser.cpp

distclean:
	$(RM) *.cc *.hh

#EXTRA_DIST += 	
