INCLUDES = -I $(top_srcdir)/include -I@FX_BOOST_HOME@ -I@FX_THRIFT_HOME@/lib/cpp/src -I $(top_srcdir)/external -Ithrift/gen-cpp -I@FX_LIBEVENT_HOME@ -I@FX_LIBEVENT_HOME@/include

THRIFT_BIN = @FX_THRIFT_HOME@/compiler/cpp/thrift

EXTRA_DIST = \
	ThriftSearchServiceHelper.h\
	SearchService.thrift\
	WatchdogService.thrift

gen-cpp/ThriftSearchService.cpp: 
	$(THRIFT_BIN) -r --gen cpp:cob_style SearchService.thrift

gen-cpp/ThriftWatchdogService.cpp gen-cpp/WatchdogService_types.cpp gen-cpp/WatchdogService_constants.cpp: 
	$(THRIFT_BIN) -r --gen cpp:cob_style WatchdogService.thrift

noinst_LTLIBRARIES = libthriftgen.la

libthriftgen_la_SOURCES = \
	ThriftSearchServiceHelper.cpp\
	gen-cpp/ThriftSearchService.cpp\
	gen-cpp/ThriftWatchdogService.cpp\
	gen-cpp/SearchService_types.cpp\
	gen-cpp/SearchService_constants.cpp\
	gen-cpp/WatchdogService_types.cpp\
	gen-cpp/WatchdogService_constants.cpp

distclean:
	$(RM) gen-cpp -rf
