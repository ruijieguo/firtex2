INCLUDES= -I $(top_srcdir)/include \
	-I $(top_srcdir)/test/src\
	-I $(top_srcdir)/extension/include \
	-I $(top_srcdir)/test/cppunit/include

test_srcdir = $(top_srcdir)/test/extension

processor_test_sources = \
	DocumentTemplateTestCase.cpp\
	StandardDocumentProcessorTestCase.cpp\
	TrecDocumentProcessorTestCase.cpp

EXTRA_DIST = \
	DocumentTemplateTestCase.h\
	StandardDocumentProcessorTestCase.h

check_PROGRAMS  = ext_processor_test

ext_processor_test_SOURCES = \
	$(processor_test_sources)\
	$(top_srcdir)/test/src/main.cpp\
	$(top_srcdir)/test/src/TestHelper.cpp

ext_processor_test_LDADD= \
	$(top_builddir)/extension/src/processor/libfirtexext_processor.la \
	$(top_builddir)/extension/src/plugin/libfirtexext_plugin.la \
	$(top_builddir)/extension/src/com/libfirtexext_com.la \
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	 -lpthread -ldl

if HAVE_SNOWBALL
ext_processor_test_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
ext_processor_test_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 


ext_processor_test_LDFLAGS = $(LIBADD_DL)

if HAVE_TCMALLOC
ext_processor_test_LDADD += @FX_TCMALLOC_HOME@/libtcmalloc.la
endif


TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)
