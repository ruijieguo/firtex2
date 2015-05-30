AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include

noinst_LTLIBRARIES = libfirtexext_plugin.la libfirtexext_plugin_firtexcom.la

libfirtexext_plugin_la_CPPFLAGS = -Wno-invalid-offsetof

plugin_sources = \
	ComObject.cpp\
	ComDocumentTemplate.cpp\
	ComDocumentSource.cpp\
	DocumentProcessorPlugins.cpp\
	ComRawDocument.cpp\
	DocumentProcessorPlugin.cpp\
	ComFieldTag.cpp\
	PluginFactory.cpp\
	AnalyzerPlugins.cpp\
	Plugin.cpp\
	Plugins.cpp\
	ComDocument.cpp\
	ComTokenView.cpp\
	ComTokenSource.cpp\
	ComDocumentSchema.cpp\
	SharedLibrary.cpp\
	FirteXCOM.cpp

libfirtexext_plugin_firtexcom_la_SOURCES = FirteXCOM.cpp

EXTRA_DIST = \
	SharedLibrary_WIN32.cpp\
	SharedLibrary_POSIX.cpp

libfirtexext_plugin_la_SOURCES = $(plugin_sources)
