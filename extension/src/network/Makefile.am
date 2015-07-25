include $(top_srcdir)/external/versions.am

AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/contrib/highlighter/include 

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

LDADD = 

external_srcdir = $(top_srcdir)/external

if WITH_BUNDLED_LIBEVENT
AM_CPPFLAGS += -I$(external_srcdir)/$(LIBEVENT)\
	-I$(external_srcdir)/$(LIBEVENT)/include

LDADD += $(external_srcdir)/$(LIBEVENT)/libevent.la
LDADD += $(external_srcdir)/$(LIBEVENT)/libevent_pthreads.la
else
LDADD += -llibevent
LDADD += -llibevent_pthreads
endif

if WITH_BUNDLED_PCRE2
AM_CPPFLAGS += -I$(external_srcdir)/$(PCRE2)/src
endif


libfirtexext_network_la_SOURCES = $(network_sources)

