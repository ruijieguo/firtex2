include $(top_srcdir)/external/versions.am

AM_CPPFLAGS = -I $(top_srcdir)/include\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/test/cppunit/include\
	-I $(top_srcdir)/test/src

check_PROGRAMS  = utility_test

utility_test_SOURCES = \
	BinaryFileTestCase.cpp\
	DirectoryIteratorTestCase.cpp\
	FileTestCase.cpp\
	HashMapTestCase.cpp\
	HashTestCase.cpp\
	KeyValueParserTestCase.cpp\
	MMapFileTestCase.cpp\
	NumberParserTestCase.cpp\
	PathTestCase.cpp\
	SlicedVectorTestCase.cpp\
	StringTokenizerTestCase.cpp\
	TextFileTestCase.cpp\
	XMLDocumentWrapperTestCase.cpp\
	LooseBoundedPoolTestCase.cpp\
	ConcurrentLinkedQueueTestCase.cpp

EXTRA_DIST = \
	BinaryFileTestCase.h\
	DirectoryIteratorTestCase.h\
	FileTestCase.h\
	HashMapTestCase.h\
	HashTestCase.h\
	KeyValueParserTestCase.h\
	MMapFileTestCase.h\
	NumberParserTestCase.h\
	PathTestCase.h\
	SlicedVectorTestCase.h\
	StringTokenizerTestCase.h\
	TextFileTestCase.h\
	LooseBoundedPoolTestCase.h

utility_test_LDADD= $(top_builddir)/extension/src/.libs/libfirtexext.la\
	$(top_builddir)/src/.libs/libfirtex.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	$(top_builddir)/contrib/snowball/src/libfirtex_snowball.la\
	$(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la\
	 -lpthread -ldl

if WITH_BUNDLED_LIBEVENT
utility_test_LDADD += $(external_srcdir)/$(LIBEVENT)/libevent.la
utility_test_LDADD += $(external_srcdir)/$(LIBEVENT)/libevent_pthreads.la
else
utility_test_LDADD += -llibevent
utility_test_LDADD += -llibevent_pthreads
endif


if WITH_TCMALLOC
utility_test_LDADD += -llibtcmalloc
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)

