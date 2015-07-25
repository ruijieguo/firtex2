include $(top_srcdir)/external/versions.am

AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/contrib/highlighter/include

noinst_LTLIBRARIES = libappcommon.la

libappcommon_la_SOURCES = \
	SnippetGenerator.cpp\
	ServerBase.cpp\
	ServiceConf.cpp\
	HTTPPingService.cpp\
	ZkClientWrapper.cpp\
	FramedMemoryTransport.cpp\
	AsyncClientChannel.cpp\
	RPCServer.cpp

external_srcdir = $(top_srcdir)/external

if WITH_BUNDLED_BOOST
AM_CPPFLAGS += -I$(external_srcdir)/$(BOOST)
endif

if WITH_BUNDLED_LIBEVENT
AM_CPPFLAGS += -I$(external_srcdir)/$(LIBEVENT)/include\
	-I$(external_srcdir)/$(LIBEVENT)
endif

if WITH_BUNDLED_THRIFT
AM_CPPFLAGS += -I$(external_srcdir)/$(THRIFT)/lib/cpp/src
endif

if WITH_BUNDLED_ZOOKEEPER
AM_CPPFLAGS += -I$(external_srcdir)/$(ZOOKEEPER)/src/c/include
AM_CPPFLAGS += -I$(external_srcdir)/$(ZOOKEEPER)/src/c/generated
endif

