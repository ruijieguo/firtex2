INCLUDES= -I $(top_srcdir)/include \
	-I $(top_srcdir)/test/src\
	-I $(top_srcdir)/extension/include \
	-I $(top_srcdir)/extension/include/firtex/extension/external\
	-I $(top_srcdir)/test/cppunit/include\
	-I@FX_LIBEVENT_HOME@/include

EXTRA_DIST = \
	EvHttpServerTestCase.cpp

check_PROGRAMS  = ext_network_test

ext_network_test_SOURCES = \
	EvHttpServerTestCase.cpp

ext_network_test_LDADD = $(top_builddir)/extension/src/network/libfirtexext_network.la\
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	@FX_LIBEVENT_HOME@/libevent.la\
	@FX_LIBEVENT_HOME@/libevent_pthreads.la\
	 -lpthread -ldl

ext_network_test_LDFLAGS = $(LIBADD_DL)

if HAVE_SNOWBALL
ext_network_test_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
ext_network_test_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 

if HAVE_TCMALLOC
ext_network_test_LDADD += @FX_TCMALLOC_HOME@/libtcmalloc.la
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64\
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)
