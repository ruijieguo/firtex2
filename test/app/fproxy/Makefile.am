AM_CPPFLAGS = -I $(top_srcdir)/include\
        -I $(top_srcdir)/extension/include\
        -I $(top_srcdir)/test/cppunit/include\
        -I $(top_srcdir)/test/app/fproxy\
        -I $(top_srcdir)/app/fproxy\
        -I $(top_srcdir)/app/common

AUTOMAKE_OPTIONS = subdir-objects

test_srcdir = $(top_srcdir)/test/src
fproxy_test_srcdir = $(top_srcdir)/test/app/fproxy

fproxy_test_sources = \
        ClusterClauseTestCase.cpp

EXTRA_DIST = \
        ClusterClauseTestCase.h

check_PROGRAMS  = fproxy_test

fproxy_test_SOURCES = \
        $(fproxy_test_sources)\
        ../../../app/fproxy/ClusterClause.cpp

fproxy_test_LDADD= $(top_builddir)/extension/src/libfirtexext.la\
        $(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
        $(top_builddir)/test/cppunit/libcppunit.la\
	$(top_builddir)/contrib/snowball/src/libfirtex_snowball.la\
	$(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la\
         -lpthread -ldl

fproxy_test_LDFLAGS = $(LIBADD_DL)

if WITH_TCMALLOC
fproxy_test_LDADD += -llibtcmalloc
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
        HEAPCHECK=normal

TESTS = $(check_PROGRAMS)
