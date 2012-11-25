INCLUDES = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/contrib/highlighter/include -I@FX_BOOST_HOME@ -I@FX_THRIFT_HOME@/lib/cpp/src -Ithrift/gen-cpp -I@FX_LIBEVENT_HOME@ -I@FX_LIBEVENT_HOME@/include

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

if HAVE_THRIFT
fproxy_SOURCES += \
	RPCBlenderService.cpp\
	RPCProxyServer.cpp\
	RPCProxyService.cpp\
	RPCSearchCluster.cpp\
	RPCSearchClusters.cpp\
	RPCSearchReplica.cpp\
	RPCSearchShard.cpp\
	RPCSearchShards.cpp

endif

LDADD = $(top_builddir)/extension/src/libfirtexext.la \
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/app/common/libappcommon.la

if HAVE_SNOWBALL
LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 

if HAVE_THRIFT
LDADD += $(top_builddir)/app/thrift/libthriftgen.la
LDADD += @FX_THRIFT_HOME@/lib/cpp/.libs/libthrift.la
LDADD += @FX_THRIFT_HOME@/lib/cpp/.libs/libthriftnb.la
endif

LDADD += @FX_LIBEVENT_HOME@/libevent.la
LDADD += @FX_LIBEVENT_HOME@/libevent_pthreads.la

fproxy_LDFLAGS = $(LINKER_OPTIONS)


