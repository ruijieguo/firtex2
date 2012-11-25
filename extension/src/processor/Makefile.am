INCLUDES = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/src/external

noinst_LTLIBRARIES = libfirtexext_processor.la

processor_sources = \
	HTMLParser.cpp\
	MultiTypeDocumentProcessor.cpp\
	PlainDocumentProcessor.cpp\
	HTMLDocumentProcessor.cpp\
	StandardDocumentProcessor.cpp\
	DocumentTemplate.cpp\
	TrecDocumentProcessor.cpp\
	DocumentProcessorFactory.cpp\
	PlainDocumentReader.cpp\
	GZipDocumentReader.cpp

libfirtexext_processor_la_SOURCES = $(processor_sources)