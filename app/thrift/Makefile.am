include $(top_srcdir)/external/versions.am

AUTOMAKE_OPTIONS = subdir-objects

gen-cpp/ThriftSearchService.cpp gen-cpp/ThriftSearchService.h: SearchService.thrift
	$(THRIFT_BIN) -r --gen cpp:cob_style $<

gen-cpp/ThriftWatchdogService.cpp gen-cpp/WatchdogService_types.cpp gen-cpp/WatchdogService_constants.cpp: WatchdogService.thrift
	$(THRIFT_BIN) -r --gen cpp:cob_style $<

noinst_LTLIBRARIES = libthriftgen.la

libthriftgen_la_SOURCES = \
	gen-cpp/ThriftSearchService.cpp\
	gen-cpp/ThriftWatchdogService.cpp\
	gen-cpp/SearchService_types.cpp\
	gen-cpp/SearchService_constants.cpp\
	gen-cpp/WatchdogService_types.cpp\
	gen-cpp/WatchdogService_constants.cpp\
	ThriftSearchServiceHelper.cpp

AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/external -Ithrift/gen-cpp

THRIFT_BIN = 
external_srcdir = $(top_srcdir)/external

if WITH_BUNDLED_BOOST
AM_CPPFLAGS += -I$(external_srcdir)/$(BOOST)
endif

if WITH_BUNDLED_THRIFT
AM_CPPFLAGS += -I$(external_srcdir)/$(THRIFT)/lib/cpp/src
THRIFT_BIN += $(external_srcdir)/$(THRIFT)/compiler/cpp/thrift
else
THRIFT_BIN += /usr/bin/thrift
endif

EXTRA_DIST = \
	ThriftSearchServiceHelper.h\
	SearchService.thrift\
	WatchdogService.thrift

distclean-local:
	$(RM) gen-cpp -rf
