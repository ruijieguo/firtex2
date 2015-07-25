AM_CPPFLAGS = -I $(top_srcdir)/include\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/test/cppunit/include\
	-I $(top_srcdir)/test/src

test_srcdir = $(top_srcdir)/test/src/
config_test_srcdir = $(test_srcdir)/config


EXTRA_DIST = \
	ConfiguratorTestCase.h\
	XMLConfiguratorTestCase.h

check_PROGRAMS  = config_test

config_test_SOURCES = \
	ConfiguratorTestCase.cpp\
	XMLConfiguratorTestCase.cpp

config_test_LDADD= $(top_builddir)/extension/src/libfirtexext.la\
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	 -lpthread -ldl

if WITH_TCMALLOC
config_test_LDADD += -llibtcmalloc
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
		HEAPCHECK=normal

TESTS = $(check_PROGRAMS)

