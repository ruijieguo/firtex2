AM_CPPFLAGS = -I $(top_srcdir)/include\
        -I $(top_srcdir)/extension/include\
        -I $(top_srcdir)/test/cppunit/include\
        -I $(top_srcdir)/test/app/fproxy\
        -I $(top_srcdir)/app/fproxy\
        -I $(top_srcdir)/app/common

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
	@FX_LIBEVENT_HOME@/libevent.la\
	@FX_LIBEVENT_HOME@/libevent_pthreads.la\
         -lpthread -ldl

if HAVE_SNOWBALL
fproxy_test_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif

if HAVE_HIGHLIGHTER
fproxy_test_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif

fproxy_test_LDFLAGS = $(LIBADD_DL)

if HAVE_TCMALLOC
fproxy_test_LDADD += @FX_TCMALLOC_HOME@/libtcmalloc.la
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
        HEAPCHECK=normal

TESTS = $(check_PROGRAMS)
