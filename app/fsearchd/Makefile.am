include $(top_srcdir)/external/versions.am

AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/contrib/highlighter/include -Ithrift/gen-cpp 

appsrcdir = $(top_srcdir)/app/fsearchd
common_incdir = $(top_srcdir)/app/common

bindir = ${exec_prefix}/firtex2/bin

bin_PROGRAMS = fsearchd frpcclient

fsearchd_SOURCES = \
	HTTPSearchServer.cpp\
	SearchServerAppRunner.cpp\
	SearchServerMain.cpp\
	SearchResource.cpp\
	SearchExecutor.cpp\
	SearchServerBase.cpp\
	HTTPSearchService.cpp
#	RPCSearchServer.cpp\
#	RPCSearchService.cpp

frpcclient_SOURCES = \
	RPCSearchClientAppRunner.cpp\
	SearchClientMain.cpp

frpcclient_LDADD = $(top_builddir)/extension/src/libfirtexext.la \
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/app/common/libappcommon.la\
	$(top_builddir)/app/thrift/libthriftgen.la

fsearchd_LDADD = $(top_builddir)/extension/src/libfirtexext.la \
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/app/common/libappcommon.la\
	$(top_builddir)/contrib/snowball/src/libfirtex_snowball.la\
	$(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la\
	$(top_builddir)/app/thrift/libthriftgen.la

external_srcdir = $(top_srcdir)/external

if WITH_BUNDLED_PCRE2
AM_CPPFLAGS += -I$(external_srcdir)/$(PCRE2)/src
endif


if WITH_BUNDLED_BOOST
AM_CPPFLAGS += -I$(external_srcdir)/$(BOOST)
fsearchd_LDADD += $(external_srcdir)/$(PCRE2)/libpcre2-8.la
endif

if WITH_BUNDLED_LIBEVENT
AM_CPPFLAGS += -I$(external_srcdir)/$(LIBEVENT)\
	-I$(external_srcdir)/$(LIBEVENT)/include

fsearchd_LDADD += $(external_srcdir)/$(LIBEVENT)/libevent.la
fsearchd_LDADD += $(external_srcdir)/$(LIBEVENT)/libevent_pthreads.la

frpcclient_LDADD += $(external_srcdir)/$(LIBEVENT)/libevent.la
frpcclient_LDADD += $(external_srcdir)/$(LIBEVENT)/libevent_pthreads.la
else
fsearchd_LDADD += -llibevent
fsearchd_LDADD += -llibevent_pthreads
frpcclient_LDADD += -llibevent
frpcclient_LDADD += -llibevent_pthreads
endif

if WITH_BUNDLED_THRIFT
frpcclient_LDADD += $(external_srcdir)/$(THRIFT)/lib/cpp/libthrift.la
frpcclient_LDADD += $(external_srcdir)/$(THRIFT)/lib/cpp/libthriftnb.la

fsearchd_LDADD += $(external_srcdir)/$(THRIFT)/lib/cpp/libthrift.la
fsearchd_LDADD += $(external_srcdir)/$(THRIFT)/lib/cpp/libthriftnb.la

AM_CPPFLAGS += -I$(external_srcdir)/$(THRIFT)/lib/cpp/src
else
frpcclient_LDADD += -llibthrift
frpcclient_LDADD += -llibthriftnb

fsearchd_LDADD += -llibthrift
fsearchd_LDADD += -llibthriftnb
endif

