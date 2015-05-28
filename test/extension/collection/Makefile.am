INCLUDES= -I $(top_srcdir)/include \
	-I $(top_srcdir)/test/src\
	-I $(top_srcdir)/extension/include \
	-I $(top_srcdir)/test/cppunit/include


collection_test_sources = \
	CollectionConfigureTestCase.cpp\
	DirCollectionTestCase.cpp\
	StandardCollectionTestCase.cpp

EXTRA_DIST = 

check_PROGRAMS  = ext_collection_test

ext_collection_test_SOURCES = \
	$(collection_test_sources)

ext_collection_test_LDADD= \
	$(top_builddir)/extension/src/collection/libfirtexext_collection.la \
	$(top_builddir)/extension/src/processor/libfirtexext_processor.la \
	$(top_builddir)/extension/src/plugin/libfirtexext_plugin.la \
	$(top_builddir)/extension/src/com/libfirtexext_com.la \
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	 -lpthread -ldl

if HAVE_SNOWBALL
ext_collection_test_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
ext_collection_test_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 


ext_collection_test_LDFLAGS = $(LIBADD_DL)

if HAVE_TCMALLOC
ext_collection_test_LDADD += @FX_TCMALLOC_HOME@/libtcmalloc.la
endif


TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)
