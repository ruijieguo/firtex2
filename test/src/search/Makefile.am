AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -I $(top_srcdir)/include\
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
	$(top_builddir)/contrib/snowball/src/libfirtex_snowball.la\
	$(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la\
	 -lpthread -ldl

if WITH_TCMALLOC
search_test_LDADD += -llibtcmalloc
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)

