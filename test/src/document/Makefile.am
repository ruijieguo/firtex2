INCLUDES = -I $(top_srcdir)/include\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/test/cppunit/include\
	-I $(top_srcdir)/test/src

test_srcdir = $(top_srcdir)/test/src/
document_test_srcdir = $(test_srcdir)/document

document_test_sources = \
	$(document_test_srcdir)/DocumentSchemaTestCase.cpp

EXTRA_DIST = \
	$(document_test_srcdir)/DocumentSchemaTestCase.h

check_PROGRAMS  = document_test

document_test_SOURCES = \
	$(document_test_sources)\
	$(test_srcdir)/main.cpp\
	$(test_srcdir)/TestHelper.cpp

document_test_LDADD= $(top_builddir)/extension/src/libfirtexext.la\
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	@FX_LIBEVENT_HOME@/libevent.la\
	@FX_LIBEVENT_HOME@/libevent_pthreads.la\
	 -lpthread -ldl

if HAVE_SNOWBALL
document_test_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
document_test_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 

document_test_LDFLAGS = $(LIBADD_DL)

if HAVE_TCMALLOC
document_test_LDADD += @FX_TCMALLOC_HOME@/libtcmalloc.la
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)

