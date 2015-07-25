AM_CPPFLAGS = -I $(top_srcdir)/include\
        -I $(top_srcdir)/extension/include\
        -I $(top_srcdir)/test/cppunit/include\
        -I $(top_srcdir)/test/app/searcher\
        -I $(top_srcdir)/test/src\
        -I $(top_srcdir)/app/fsearchd\
        -I $(top_srcdir)/app/common

AUTOMAKE_OPTIONS = subdir-objects

test_srcdir = $(top_srcdir)/test/src
searcher_test_srcdir = $(top_srcdir)/test/app/searcher

searcher_test_sources = \
	SearchResourceTestCase.cpp

EXTRA_DIST = \
	SearchResourceTestCase.h

check_PROGRAMS  = searcher_test

searcher_test_SOURCES = \
        $(searcher_test_sources)\
	../../../app/fsearchd/SearchResource.cpp\
	../../../app/common/ServiceConf.cpp\
	../../src/index/IndexBuildHelper.cpp

searcher_test_LDADD= $(top_builddir)/extension/src/libfirtexext.la\
        $(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
        $(top_builddir)/test/cppunit/libcppunit.la\
	$(top_builddir)/contrib/snowball/src/libfirtex_snowball.la\
	$(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la\
         -lpthread -ldl

searcher_test_LDFLAGS = $(LIBADD_DL)

if WITH_TCMALLOC
searcher_test_LDADD += -llibtcmalloc
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
        HEAPCHECK=normal

TESTS = $(check_PROGRAMS)
