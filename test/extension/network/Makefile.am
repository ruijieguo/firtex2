AM_CPPFLAGS = -I $(top_srcdir)/include \
	-I $(top_srcdir)/test/src\
	-I $(top_srcdir)/extension/include \
	-I $(top_srcdir)/extension/include/firtex/extension/external\
	-I $(top_srcdir)/test/cppunit/include

EXTRA_DIST = \
	EvHttpServerTestCase.cpp

check_PROGRAMS  = ext_network_test

ext_network_test_SOURCES = \
	EvHttpServerTestCase.cpp

ext_network_test_LDADD = \
	$(top_builddir)/extension/src/network/libfirtexext_network.la\
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	 -lpthread -ldl



if WITH_TCMALLOC
ext_network_test_LDADD += -llibtcmalloc
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64\
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)
