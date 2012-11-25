INCLUDES = -I $(top_srcdir)/include\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/test/cppunit/include\
	-I $(top_srcdir)/test/src

test_srcdir = $(top_srcdir)/test/src/
search_test_srcdir = $(test_srcdir)/search

search_test_sources = \
	$(search_test_srcdir)/DateTimeSearchTestCase.cpp\
	$(search_test_srcdir)/ClauseTestCase.cpp\
	$(search_test_srcdir)/FieldSelectClauseTestCase.cpp\
	$(search_test_srcdir)/IndexSearcherTestCase.cpp\
	$(search_test_srcdir)/StatementTestCase.cpp

EXTRA_DIST = \
	$(search_test_srcdir)/IndexSearcherTestCase.h\
	$(search_test_srcdir)/DateTimeSearchTestCase.h

check_PROGRAMS  = search_test

search_test_SOURCES = \
	$(search_test_sources)\
	$(test_srcdir)/main.cpp\
	$(test_srcdir)/TestHelper.cpp\
	$(test_srcdir)/index/IndexBuildHelper.cpp

search_test_LDADD= $(top_builddir)/extension/src/libfirtexext.la\
	$(top_builddir)/src/libfirtex.la\
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

