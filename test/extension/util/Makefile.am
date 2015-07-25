include $(top_srcdir)/external/versions.am
AM_CPPFLAGS = -I $(top_srcdir)/include \
	-I $(top_srcdir)/test/src\
	-I $(top_srcdir)/extension/include \
	-I $(top_srcdir)/test/cppunit/include

EXTRA_DIST = OptionParserTestCase.h

check_PROGRAMS  = ext_util_test

ext_util_test_SOURCES = \
	OptionParserTestCase.cpp\
	RegexTestCase.cpp

ext_util_test_LDADD= $(top_builddir)/extension/src/util/libfirtexext_util.la \
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	$(top_builddir)/extension/src/com/libfirtexext_com.la \
	 -lpthread -ldl

external_srcdir = $(top_srcdir)/external
if WITH_BUNDLED_PCRE2
AM_CPPFLAGS += -I$(external_srcdir)/$(PCRE2)/src
ext_util_test_LDADD += $(external_srcdir)/$(PCRE2)/libpcre2-posix.la
endif

if WITH_TCMALLOC
ext_util_test_LDADD += -llibtcmalloc
endif


TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)
