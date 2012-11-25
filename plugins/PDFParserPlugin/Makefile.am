SUBDIRS = .
pdfparserplugin_srcdir = $(top_srcdir)/plugins/PDFParserPlugin
xpdf_srcdir = $(top_srcdir)/plugins/PDFParserPlugin/xpdf

INCLUDES = -I $(top_srcdir)/include\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/plugins/PDFParserPlugin/xpdf\
	-I $(top_srcdir)/plugins/PDFParserPlugin/xpdf/xpdf\
	-I $(top_srcdir)/plugins/PDFParserPlugin/xpdf/fofi\
	-I $(top_srcdir)/plugins/PDFParserPlugin/xpdf/splash\
	-I $(top_srcdir)/plugins/PDFParserPlugin/xpdf/goo

bindir = ${exec_prefix}/firtex2/plugins

bin_PROGRAMS = pdfparserplugin.so


pdfparserplugin_so_SOURCES = $(pdfparserplugin_srcdir)/PDFDocReader.cpp\
	$(pdfparserplugin_srcdir)/PDFParserPlugin.cpp\
	$(pdfparserplugin_srcdir)/PDFParserPluginFactory.cpp\
	$(top_srcdir)/extension/src/plugin/FirteXCOM.cpp\
	$(pdfparserplugin_srcdir)/main.cpp

EXTRA_DIST = \
	PDFParserPlugin_8.0.vcproj\
	PDFParserPlugin_9.0.vcproj\
	PDFParserPlugin.def\
	PDFDocReader.h\
	PDFParserPlugin.h

include xpdf/fofi/Makefile.am
include xpdf/xpdf/Makefile.am
include xpdf/goo/Makefile.am

pdfparserplugin_so_LDADD   = $(top_builddir)/extension/src/com/.libs/libfirtexext_com.la -ldl -lpthread
pdfparserplugin_so_LDFLAGS = $(LINKER_OPTIONS) -fPIC -shared -rdynamic
pdfparserplugin_so_CXXFLAGS = @CXXFLAGS@ -fPIC -w
pdfparserplugin_so_CFLAGS = @CFLAGS@ -fPIC -w
