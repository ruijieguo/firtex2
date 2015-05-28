INCLUDES = -I $(top_srcdir)/include\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/test/cppunit/include\
	-I $(top_srcdir)/test/src

check_PROGRAMS  = search_test

search_test_SOURCES = \
	DateTimeSearchTestCase.cpp\
	ClauseTestCase.cpp\
	FieldSelectClauseTestCase.cpp\
	ScorerProfileClauseTestCase.cpp\
	IndexSearcherTestCase.cpp\
	StatementTestCase.cpp\
	../index/IndexBuildHelper.cpp

EXTRA_DIST = \
	DateTimeSearchTestCase.h\
	ClauseTestCase.h\
	FieldSelectClauseTestCase.h\
	ScorerProfileClauseTestCase.h\
	IndexSearcherTestCase.h\
	StatementTestCase.h

search_test_LDADD= $(top_builddir)/extension/src/libfirtexext.la\
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	@FX_LIBEVENT_HOME@/libevent.la\
	@FX_LIBEVENT_HOME@/libevent_pthreads.la\
	 -lpthread -ldl
if HAVE_SNOWBALL
search_test_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
search_test_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 

search_test_LDFLAGS = $(LIBADD_DL)

if HAVE_TCMALLOC
search_test_LDADD += @FX_TCMALLOC_HOME@/libtcmalloc.la
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)

