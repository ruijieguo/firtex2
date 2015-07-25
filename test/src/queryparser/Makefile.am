AM_CPPFLAGS = -I $(top_srcdir)/include\
	-I $(top_srcdir)/src\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/test/cppunit/include\
	-I $(top_srcdir)/test/src

test_srcdir = $(top_srcdir)/test/src/

queryparser_test_sources = \
	QueryExprLexerTestCase.cpp\
	QueryExprBisonParserTestCase.cpp\
	QueryParserTestCase.cpp\
	ExprParserTestCase.cpp

EXTRA_DIST = \
	QueryExprLexerTestCase.h\
	QueryExprBisonParserTestCase.h\
	QueryParserTestCase.h\
	ExprParserTestCase.h


check_PROGRAMS  = queryparser_test

queryparser_test_SOURCES = $(queryparser_test_sources)

queryparser_test_LDADD= $(top_builddir)/extension/src/libfirtexext.la\
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	 -lpthread -ldl

if WITH_TCMALLOC
queryparser_test_LDADD += @FX_TCMALLOC_HOME@/libtcmalloc.la
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)

