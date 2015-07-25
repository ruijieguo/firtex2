
include $(top_srcdir)/external/versions.am

AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -I $(top_srcdir)/include\
        -I $(top_srcdir)/extension/include\
        -I $(top_srcdir)/test/cppunit/include\
        -I $(top_srcdir)/test/app/common\
        -I $(top_srcdir)/app/common\
	-Igen-cpp

external_srcdir = $(top_srcdir)/external

if WITH_BUNDLED_BOOST
AM_CPPFLAGS += -I$(external_srcdir)/$(BOOST)
endif

if WITH_BUNDLED_LIBEVENT
AM_CPPFLAGS += -I$(external_srcdir)/$(LIBEVENT)\
	-I$(external_srcdir)/$(LIBEVENT)/include
endif


if WITH_BUNDLED_THRIFT
AM_CPPFLAGS += -I$(external_srcdir)/$(THRIFT)lib/cpp/src 
THRIFT_BIN = $(external_srcdir)/$(THRIFT)/compiler/cpp/thrift
else
THRIFT_BIN = thrift
endif

test_srcdir = $(top_srcdir)/test/src
appcommon_test_srcdir = $(top_srcdir)/test/app/common

all: 
	$(THRIFT_BIN) -r --gen cpp:cob_style EchoService.thrift
distclean-local:
	$(RM) gen-cpp -rf


appcommon_thrift_gen_cpp = \
	gen-cpp/EchoService.cpp

appcommon_test_sources = 

appcommon_test_sources +=\
	$(appcommon_thrift_gen_cpp)\
	RPCServerTestCase.cpp

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
	$(top_builddir)/contrib/snowball/src/libfirtex_snowball.la\
	$(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la\
         -lpthread -ldl

if WITH_BUNDLED_THRIFT
appcommon_test_LDADD += $(top_builddir)/$(THRIFT)/lib/cpp/libthrift.la
appcommon_test_LDADD += $(top_builddir)/$(THRIFT)/lib/cpp/libthriftnb.la
AM_CPPFLAGS += $(external_srcdir)/$(THRIFT)/lib/cpp/src
else
appcommon_test_LDADD += -llibthrift
appcommon_test_LDADD += -llibthriftnb
endif

appcommon_test_LDFLAGS = $(LIBADD_DL)

if WITH_TCMALLOC
appcommon_test_LDADD += -llibtcmalloc
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
        HEAPCHECK=normal

TESTS = $(check_PROGRAMS)
