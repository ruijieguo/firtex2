SUBDIRS = .
msofficeparserplugin_srcdir = $(top_srcdir)/plugins/MSOfficeParserPlugin

AM_CPPFLAGS = -I $(top_srcdir)/include\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/plugins/MSOfficeParserPlugin

bindir = ${exec_prefix}/firtex2/plugins

bin_PROGRAMS = msofficeparserplugin.so


msofficeparserplugin_so_SOURCES = \
	Analyze.cpp\
	CharGetter.cpp\
	Charsets.cpp\
	ConfUtil.cpp\
	FileUtil.cpp\
	MSOfficeParserPlugin.cpp\
	MSOfficeParserPluginFactory.cpp\
	MSOfficeTextStripper.cpp\
	NumUtils.cpp\
	Ole.cpp\
	PPTParse.cpp\
	RTFRead.cpp\
	Reader.cpp\
	Substmap.cpp\
	Writer.cpp\
	XLSParse.cpp\
	main.cpp

EXTRA_DIST = \
	Analyze.h\
	CharGetter.h\
	Charsets.h\
	ConfUtil.h\
	FileUtil.h\
	MSOfficeParserPlugin.h\
	MSOfficeTextStripper.h\
	NumUtils.h\
	Ole.h\
	PPTParse.h\
	RTFRead.h\
	Reader.h\
	Substmap.h\
	Writer.h\
	XLSParse.h\
	ppttypes.h\
	xltypes.h\
	MSOfficeParserPlugin.def\
	win32/MSOfficeParserPlugin_10.0.vcxproj\
	win32/MSOfficeParserPlugin_8.0.vcproj\
	win32/MSOfficeParserPlugin_9.0.vcproj\
	win32/acconfig.h\
	win32/getopt.c\
	win32/getopt.h\
	win32/rint.cpp\
	win32/rint.h\
	win32/win32.c

msofficeparserplugin_so_LDADD   = $(top_builddir)/extension/src/com/.libs/libfirtexext_com.la \
	$(top_builddir)/extension/src/plugin/.libs/libfirtexext_plugin_firtexcom.la -ldl -lpthread
msofficeparserplugin_so_LDFLAGS = $(LINKER_OPTIONS) -fPIC -shared -rdynamic
msofficeparserplugin_so_CXXFLAGS = @CXXFLAGS@ -fPIC
msofficeparserplugin_so_CFLAGS = @CFLAGS@ -fPIC
