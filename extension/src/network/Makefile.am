
AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/contrib/highlighter/include -I@FX_BOOST_HOME@ -I@FX_LIBEVENT_HOME@/include

noinst_LTLIBRARIES = libfirtexext_network.la

network_sources = \
	EchoService.cpp\
	EvHttpRequestHandler.cpp\
	EvHttpServiceFactory.cpp\
	EvHttpServer.cpp\
	EvHttpAsyncClient.cpp\
	EvHttpSyncClient.cpp\
	EvThreadInitializer.cpp\
	EvHttpRequestContext.cpp\
	HTTPTypes.cpp

libfirtexext_network_la_SOURCES = $(network_sources)

