include $(top_srcdir)/external/versions.am

AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -I $(top_srcdir)/include\
        -I $(top_srcdir)/extension/include\
        -I $(top_srcdir)/test/cppunit/include\
        -I $(top_srcdir)/test/src\
        -I $(top_srcdir)/test/app/watchdog\
        -I $(top_srcdir)/app/watchdog\
        -I $(top_srcdir)/app/common

test_srcdir = $(top_srcdir)/test/src
watchdog_test_srcdir = $(top_srcdir)/test/app/watchdog

EXTRA_DIST = \
        WatchdogServiceBaseTestCase.h

check_PROGRAMS  = watchdog_test

watchdog_test_SOURCES = \
        WatchdogServiceBaseTestCase.cpp\
        ../../../app/watchdog/WatchdogServiceBase.cpp

watchdog_test_LDADD= $(top_builddir)/extension/src/libfirtexext.la\
        $(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
        $(top_builddir)/test/cppunit/libcppunit.la\
	$(top_builddir)/contrib/snowball/src/libfirtex_snowball.la\
	$(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la\
         -lpthread -ldl

if WITH_BUNDLED_LIBEVENT
watchdog_test_LDADD += $(external_srcdir)/$(LIBEVENT)/libevent.la
watchdog_test_LDADD += $(external_srcdir)/$(LIBEVENT)/libevent_pthreads.la
else
watchdog_test_LDADD += -llibevent
watchdog_test_LDADD += -llibevent_pthreads
endif

if WITH_TCMALLOC
watchdog_test_LDADD += -llibtcmalloc
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
        HEAPCHECK=normal

TESTS = $(check_PROGRAMS)
