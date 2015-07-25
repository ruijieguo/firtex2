AM_CPPFLAGS = -I $(top_srcdir)/include\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/test/cppunit/include\
	-I $(top_srcdir)/test/src

test_srcdir = $(top_srcdir)/test/src/
analyzer_test_srcdir = $(test_srcdir)/analyzer

analyzer_test_sources = \
	StandardAnalyzerTestCase.cpp\
	DateTimeAnalyzerTestCase.cpp\
	DoubleArrayTestCase.cpp\
	StandardTokenizerTestCase.cpp\
	StandardAnalyzerTestCase.cpp\
	SingleWordAnalyzerTestCase.cpp\
	SynonymFilterTestCase.cpp\
	SynonymMapTestCase.cpp

EXTRA_DIST = \
	DateTimeAnalyzerTestCase.h\
	StandardTokenizerTestCase.h\
	StandardAnalyzerTestCase.h\
	SingleWordAnalyzerTestCase.h\
	SynonymFilterTestCase.h\
	SynonymMapTestCase.h

check_PROGRAMS  = analyzer_test

analyzer_test_SOURCES = $(analyzer_test_sources)

analyzer_test_LDADD= $(top_builddir)/extension/src/libfirtexext.la\
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	$(top_builddir)/contrib/snowball/src/libfirtex_snowball.la\
	$(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la\
	 -lpthread -ldl

if WITH_TCMALLOC
analyzer_test_LDADD += -llibtcmalloc
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)

