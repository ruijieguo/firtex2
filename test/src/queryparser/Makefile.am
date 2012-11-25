INCLUDES = -I $(top_srcdir)/include\
	-I $(top_srcdir)/src\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/test/cppunit/include\
	-I $(top_srcdir)/test/src

test_srcdir = $(top_srcdir)/test/src/
queryparser_test_srcdir = $(test_srcdir)/queryparser

queryparser_test_sources = \
	$(queryparser_test_srcdir)/QueryExprLexerTestCase.cpp\
	$(queryparser_test_srcdir)/QueryExprBisonParserTestCase.cpp\
	$(queryparser_test_srcdir)/QueryParserTestCase.cpp\
	$(queryparser_test_srcdir)/ExprParserTestCase.cpp

EXTRA_DIST = 

check_PROGRAMS  = queryparser_test

queryparser_test_SOURCES = \
	$(queryparser_test_sources)\
	$(test_srcdir)/main.cpp\
	$(test_srcdir)/TestHelper.cpp

queryparser_test_LDADD= $(top_builddir)/extension/src/libfirtexext.la\
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	@FX_LIBEVENT_HOME@/libevent.la\
	@FX_LIBEVENT_HOME@/libevent_pthreads.la\
	 -lpthread -ldl

if HAVE_SNOWBALL
queryparser_test_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
queryparser_test_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 

queryparser_test_LDFLAGS = $(LIBADD_DL)

if HAVE_TCMALLOC
queryparser_test_LDADD += @FX_TCMALLOC_HOME@/libtcmalloc.la
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)

