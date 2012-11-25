INCLUDES= -I $(top_srcdir)/include \
	-I $(top_srcdir)/test/src\
	-I $(top_srcdir)/extension/include \
	-I $(top_srcdir)/test/cppunit/include

test_srcdir = $(top_srcdir)/test/extension

util_test_srcdir = $(test_srcdir)/util

util_test_sources = \
	$(util_test_srcdir)/OptionParserTestCase.cpp

EXTRA_DIST = $(util_test_srcdir)/OptionParserTestCase.h

check_PROGRAMS  = ext_util_test

ext_util_test_SOURCES = \
	$(util_test_sources)\
	$(top_srcdir)/test/src/main.cpp\
	$(top_srcdir)/test/src/TestHelper.cpp

ext_util_test_LDADD= $(top_builddir)/extension/src/util/libfirtexext_util.la \
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	$(top_builddir)/extension/src/com/libfirtexext_com.la \
	 -lpthread -ldl

if HAVE_SNOWBALL
ext_util_test_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
ext_util_test_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 


ext_util_test_LDFLAGS = $(LIBADD_DL)

if HAVE_TCMALLOC
ext_util_test_LDADD += @FX_TCMALLOC_HOME@/libtcmalloc.la
endif


TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)
