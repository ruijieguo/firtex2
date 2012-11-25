INCLUDES = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/contrib/highlighter/include -I@FX_BOOST_HOME@ -I@FX_THRIFT_HOME@/lib/cpp/src -Ithrift/gen-cpp -I@FX_LIBEVENT_HOME@ -I@FX_LIBEVENT_HOME@/include -I@FX_ZOOKEEPER_C_HOME@/include -I@FX_ZOOKEEPER_C_HOME@/generated

noinst_LTLIBRARIES = libappcommon.la

libappcommon_la_SOURCES = \
	SnippetGenerator.cpp\
	ServerBase.cpp\
	HTTPPingService.cpp

if HAVE_ZOOKEEPER
libappcommon_la_SOURCES += \
	ZkClientWrapper.cpp
endif

if HAVE_THRIFT
libappcommon_la_SOURCES += \
	FramedMemoryTransport.cpp\
	AsyncClientChannel.cpp\
	RPCServer.cpp
endif

