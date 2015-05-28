INCLUDES = -I $(top_srcdir)/include\
        -I $(top_srcdir)/extension/include\
        -I $(top_srcdir)/test/cppunit/include\
        -I $(top_srcdir)/test/src\
        -I $(top_srcdir)/test/app/watchdog\
        -I $(top_srcdir)/app/watchdog\
        -I $(top_srcdir)/app/common

test_srcdir = $(top_srcdir)/test/src
watchdog_test_srcdir = $(top_srcdir)/test/app/watchdog

watchdog_test_sources = \
        WatchdogServiceBaseTestCase.cpp

EXTRA_DIST = \
        WatchdogServiceBaseTestCase.h

check_PROGRAMS  = watchdog_test

watchdog_test_SOURCES = \
        $(watchdog_test_sources)\
        ../../../app/watchdog/WatchdogServiceBase.cpp

watchdog_test_LDADD= $(top_builddir)/extension/src/libfirtexext.la\
        $(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
        $(top_builddir)/test/cppunit/libcppunit.la\
	@FX_LIBEVENT_HOME@/libevent.la\
	@FX_LIBEVENT_HOME@/libevent_pthreads.la\
         -lpthread -ldl

if HAVE_SNOWBALL
watchdog_test_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif

if HAVE_HIGHLIGHTER
watchdog_test_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif

watchdog_test_LDFLAGS = $(LIBADD_DL)

if HAVE_TCMALLOC
watchdog_test_LDADD += @FX_TCMALLOC_HOME@/libtcmalloc.la
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
        HEAPCHECK=normal

TESTS = $(check_PROGRAMS)
