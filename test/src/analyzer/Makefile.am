INCLUDES = -I $(top_srcdir)/include\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/test/cppunit/include\
	-I $(top_srcdir)/test/src

test_srcdir = $(top_srcdir)/test/src/
analyzer_test_srcdir = $(test_srcdir)/analyzer

analyzer_test_sources = \
	$(analyzer_test_srcdir)/DateTimeAnalyzerTestCase.cpp
	$(analyzer_test_srcdir)/DoubleArrayTestCase.cpp\
	$(analyzer_test_srcdir)/StandardTokenizerTestCase.cpp\
	$(analyzer_test_srcdir)/StandardAnalyzerTestCase.cpp\
	$(analyzer_test_srcdir)/SingleWordAnalyzerTestCase.cpp\
	$(analyzer_test_srcdir)/SynonymFilterTestCase.cpp\
	$(analyzer_test_srcdir)/SynonymMapTestCase.cpp

EXTRA_DIST = \
	$(analyzer_test_srcdir)/DateTimeAnalyzerTestCase.h\
	$(analyzer_test_srcdir)/StandardTokenizerTestCase.h\
	$(analyzer_test_srcdir)/StandardAnalyzerTestCase.h\
	$(analyzer_test_srcdir)/SingleWordAnalyzerTestCase.h\
	$(analyzer_test_srcdir)/SynonymFilterTestCase.h\
	$(analyzer_test_srcdir)/SynonymMapTestCase.h

check_PROGRAMS  = analyzer_test

analyzer_test_SOURCES = \
	$(analyzer_test_sources)\
	$(test_srcdir)/main.cpp\
	$(test_srcdir)/TestHelper.cpp

analyzer_test_LDADD= $(top_builddir)/extension/src/libfirtexext.la\
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	@FX_LIBEVENT_HOME@/libevent.la\
	@FX_LIBEVENT_HOME@/libevent_pthreads.la\
	 -lpthread -ldl

if HAVE_SNOWBALL
analyzer_test_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
analyzer_test_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 

analyzer_test_LDFLAGS = $(LIBADD_DL)

if HAVE_TCMALLOC
analyzer_test_LDADD += @FX_TCMALLOC_HOME@/libtcmalloc.la
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)

