THRIFT_BIN = @FX_THRIFT_HOME@/compiler/cpp/thrift
AM_CPPFLAGS = -I $(top_srcdir)/include\
        -I $(top_srcdir)/extension/include\
        -I $(top_srcdir)/test/cppunit/include\
        -I $(top_srcdir)/test/app/common\
        -I $(top_srcdir)/app/common\
	-I@FX_BOOST_HOME@ \
	-I@FX_THRIFT_HOME@/lib/cpp/src -Igen-cpp -I@FX_LIBEVENT_HOME@ -I@FX_LIBEVENT_HOME@/include

test_srcdir = $(top_srcdir)/test/src
appcommon_test_srcdir = $(top_srcdir)/test/app/common

all: 
	$(THRIFT_BIN) -r --gen cpp:cob_style EchoService.thrift
distclean:
	$(RM) gen-cpp -rf


appcommon_thrift_gen_cpp = \
	gen-cpp/EchoService.cpp

appcommon_test_sources = 

if HAVE_THRIFT
appcommon_test_sources +=\
	$(appcommon_thrift_gen_cpp)\
	RPCServerTestCase.cpp
endif

EXTRA_DIST = \
        RPCServerTestCase.h\
	EchoService.thrift

check_PROGRAMS  = appcommon_test

appcommon_test_SOURCES = \
        $(appcommon_test_sources)

appcommon_test_LDADD= $(top_builddir)/extension/src/libfirtexext.la\
        $(top_builddir)/src/libfirtex.la\
        $(top_builddir)/app/common/libappcommon.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
        $(top_builddir)/test/cppunit/libcppunit.la\
	@FX_LIBEVENT_HOME@/libevent.la\
	@FX_LIBEVENT_HOME@/libevent_pthreads.la\
         -lpthread -ldl

if HAVE_THRIFT
appcommon_test_LDADD += @FX_THRIFT_HOME@/lib/cpp/.libs/libthrift.la
appcommon_test_LDADD += @FX_THRIFT_HOME@/lib/cpp/.libs/libthriftnb.la
endif

if HAVE_SNOWBALL
appcommon_test_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif

if HAVE_HIGHLIGHTER
appcommon_test_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif

appcommon_test_LDFLAGS = $(LIBADD_DL)

if HAVE_TCMALLOC
appcommon_test_LDADD += @FX_TCMALLOC_HOME@/libtcmalloc.la
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
        HEAPCHECK=normal

TESTS = $(check_PROGRAMS)
