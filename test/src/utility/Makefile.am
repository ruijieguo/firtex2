INCLUDES = -I $(top_srcdir)/include\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/test/cppunit/include\
	-I $(top_srcdir)/test/src

test_srcdir = $(top_srcdir)/test/src/

#BoundedConcurrentHashMapTestCase.cpp

utility_test_sources = \
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

check_PROGRAMS  = utility_test

utility_test_SOURCES = \
	$(utility_test_sources)\
	$(test_srcdir)/main.cpp\
	$(test_srcdir)/TestHelper.cpp

utility_test_LDADD= $(top_builddir)/extension/src/.libs/libfirtexext.la\
	$(top_builddir)/src/.libs/libfirtex.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	@FX_LIBEVENT_HOME@/libevent.la\
	@FX_LIBEVENT_HOME@/libevent_pthreads.la\
	 -lpthread -ldl

if HAVE_SNOWBALL
utility_test_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
utility_test_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 

utility_test_LDFLAGS = $(LIBADD_DL)

if HAVE_TCMALLOC
utility_test_LDADD += @FX_TCMALLOC_HOME@/libtcmalloc.la
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)

