include $(top_srcdir)/external/versions.am

AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/contrib/highlighter/include -Ithrift/gen-cpp

appsrcdir = $(top_srcdir)/app/fproxy
common_incdir = $(top_srcdir)/app/common

bindir = ${exec_prefix}/firtex2/bin

bin_PROGRAMS = fproxy

fproxy_SOURCES = \
	AppendResultCombiner.cpp\
	BlenderServerConf.cpp\
	SearchServiceClient.cpp\
	BlenderServiceBase.cpp\
	ClusterClause.cpp\
	HTTPBlenderService.cpp\
	HTTPProxyServer.cpp\
	HTTPProxyService.cpp\
	HTTPSearchCluster.cpp\
	HTTPSearchClusters.cpp\
	HTTPSearchReplica.cpp\
	HTTPSearchShard.cpp\
	HTTPSearchShards.cpp\
	ProxyServerAppRunner.cpp\
	ProxyServerConf.cpp\
	ProxyServerMain.cpp\
	ProxyServiceBase.cpp\
	SearchClusterBase.cpp\
	SearchClustersBase.cpp\
	SearchReplicaBase.cpp\
	SearchShardBase.cpp\
	SearchShardsBase.cpp\
	RoundRobinPolicy.cpp\
	LoadBallancePolicyFactory.cpp

fproxy_SOURCES += \
	RPCBlenderService.cpp\
	RPCProxyServer.cpp\
	RPCProxyService.cpp\
	RPCSearchCluster.cpp\
	RPCSearchClusters.cpp\
	RPCSearchReplica.cpp\
	RPCSearchShard.cpp\
	RPCSearchShards.cpp


LDADD = $(top_builddir)/extension/src/libfirtexext.la \
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/app/common/libappcommon.la\
	$(top_builddir)/contrib/snowball/src/libfirtex_snowball.la\
	$(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la

external_srcdir = $(top_srcdir)/external

if WITH_BUNDLED_BOOST
AM_CPPFLAGS += -I$(external_srcdir)/$(BOOST)
endif

if WITH_BUNDLED_LIBEVENT
AM_CPPFLAGS += -I$(external_srcdir)/$(LIBEVENT)\
	-I$(external_srcdir)/$(LIBEVENT)/include

LDADD += $(external_srcdir)/$(LIBEVENT)/libevent.la
LDADD += $(external_srcdir)/$(LIBEVENT)/libevent_pthreads.la
else
LDADD += -llibevent
LDADD += -llibevent_pthreads
endif

if WITH_BUNDLED_THRIFT
LDADD += $(top_builddir)/$(THRIFT)/lib/cpp/libthrift.la
LDADD += $(top_builddir)/$(THRIFT)/lib/cpp/libthriftnb.la
AM_CPPFLAGS += $(external_srcdir)/$(THRIFT)/lib/cpp/src
else
LDADD += -llibthrift
LDADD += -llibthriftnb
endif

fproxy_LDFLAGS = $(LINKER_OPTIONS)


