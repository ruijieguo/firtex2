INCLUDES = -I $(top_srcdir)/include\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/test/cppunit/include\
	-I $(top_srcdir)/test/src

EXTRA_DIST = LoggerTestCase.h

check_PROGRAMS  = common_test

common_test_SOURCES = LoggerTestCase.cpp

common_test_LDADD= $(top_builddir)/extension/src/libfirtexext.la\
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	@FX_LIBEVENT_HOME@/libevent.la\
	@FX_LIBEVENT_HOME@/libevent_pthreads.la\
	 -lpthread -ldl

if HAVE_SNOWBALL
common_test_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
common_test_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 


common_test_LDFLAGS = $(LIBADD_DL)

if HAVE_TCMALLOC
common_test_LDADD += @FX_TCMALLOC_HOME@/libtcmalloc.la
endif


TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)

