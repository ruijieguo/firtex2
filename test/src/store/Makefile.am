include $(top_srcdir)/external/versions.am

AM_CPPFLAGS = -I $(top_srcdir)/include\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/test/cppunit/include\
	-I $(top_srcdir)/test/src

test_srcdir = $(top_srcdir)/test/src/
store_test_srcdir = $(test_srcdir)/store

store_test_sources = \
	BlockFileSystemTest.cpp\
	BlockFileInputStreamTestCase.cpp\
	BlockFileOutputStreamTestCase.cpp

EXTRA_DIST = \
	BlockFileSystemTest.h\
	BlockFileInputStreamTestCase.h\
	BlockFileOutputStreamTestCase.h

check_PROGRAMS  = store_test

store_test_SOURCES = $(store_test_sources)

store_test_LDADD= $(top_builddir)/extension/src/libfirtexext.la\
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	 -lpthread -ldl

store_test_LDFLAGS = $(LIBADD_DL)

if WITH_TCMALLOC
store_test_LDADD += -llibtcmalloc
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)

