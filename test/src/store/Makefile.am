INCLUDES = -I $(top_srcdir)/include\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/test/cppunit/include\
	-I $(top_srcdir)/test/src

test_srcdir = $(top_srcdir)/test/src/
store_test_srcdir = $(test_srcdir)/store

store_test_sources = \
	$(store_test_srcdir)/BlockFileSystemTest.cpp\
	$(store_test_srcdir)/BlockFileInputStreamTestCase.cpp\
	$(store_test_srcdir)/BlockFileOutputStreamTestCase.cpp

EXTRA_DIST = \
	$(store_test_srcdir)/BlockFileSystemTest.h\
	$(store_test_srcdir)/BlockFileInputStreamTestCase.h\
	$(store_test_srcdir)/BlockFileOutputStreamTestCase.h

check_PROGRAMS  = store_test

store_test_SOURCES = \
	$(store_test_sources)\
	$(test_srcdir)/main.cpp\
	$(test_srcdir)/TestHelper.cpp

store_test_LDADD= $(top_builddir)/extension/src/libfirtexext.la\
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	@FX_LIBEVENT_HOME@/libevent.la\
	@FX_LIBEVENT_HOME@/libevent_pthreads.la\
	 -lpthread -ldl


if HAVE_SNOWBALL
store_test_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
store_test_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 

store_test_LDFLAGS = $(LIBADD_DL)

if HAVE_TCMALLOC
store_test_LDADD += @FX_TCMALLOC_HOME@/libtcmalloc.la
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)

