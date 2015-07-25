AM_CPPFLAGS = -I $(top_srcdir)/include \
	-I $(top_srcdir)/test/src\
	-I $(top_srcdir)/extension/include \
	-I $(top_srcdir)/test/cppunit/include

processor_test_sources = \
	DocumentTemplateTestCase.cpp\
	StandardDocumentProcessorTestCase.cpp\
	TrecDocumentProcessorTestCase.cpp

EXTRA_DIST = \
	DocumentTemplateTestCase.h\
	StandardDocumentProcessorTestCase.h

check_PROGRAMS  = ext_processor_test

ext_processor_test_SOURCES = \
	$(processor_test_sources)

ext_processor_test_LDADD= \
	$(top_builddir)/extension/src/processor/libfirtexext_processor.la \
	$(top_builddir)/extension/src/plugin/libfirtexext_plugin.la \
	$(top_builddir)/extension/src/com/libfirtexext_com.la \
	$(top_builddir)/test/src/libfirtextest_main.la\
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	$(top_builddir)/contrib/snowball/src/libfirtex_snowball.la\
	$(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la\
	 -lpthread -ldl

if WITH_TCMALLOC
ext_processor_test_LDADD += -llibtcmalloc
endif


TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)
