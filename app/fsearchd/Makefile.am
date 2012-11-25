INCLUDES = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/contrib/highlighter/include -I@FX_BOOST_HOME@ -I@FX_THRIFT_HOME@/lib/cpp/src -Ithrift/gen-cpp -I@FX_LIBEVENT_HOME@ -I@FX_LIBEVENT_HOME@/include

appsrcdir = $(top_srcdir)/app/fsearchd
common_incdir = $(top_srcdir)/app/common

bindir = ${exec_prefix}/firtex2/bin

bin_PROGRAMS = fsearchd
if HAVE_THRIFT
bin_PROGRAMS += frpcclient
endif

fsearchd_SOURCES = \
	SearchServerBase.cpp\
	HTTPSearchServer.cpp\
	SearchServerAppRunner.cpp\
	SearchServerConf.cpp\
	SearchServerMain.cpp\
	SearchResource.cpp\
	HTTPSearchService.cpp

if HAVE_THRIFT
fsearchd_SOURCES += \
	RPCSearchServer.cpp\
	RPCSearchService.cpp

frpcclient_SOURCES = \
	RPCSearchClientAppRunner.cpp\
	SearchClientMain.cpp

frpcclient_LDADD = $(top_builddir)/extension/src/libfirtexext.la \
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/app/common/libappcommon.la
endif

fsearchd_LDADD = $(top_builddir)/extension/src/libfirtexext.la \
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/app/common/libappcommon.la

if HAVE_SNOWBALL
fsearchd_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
fsearchd_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 

fsearchd_LDADD += @FX_LIBEVENT_HOME@/libevent.la
fsearchd_LDADD += @FX_LIBEVENT_HOME@/libevent_pthreads.la

if HAVE_THRIFT
fsearchd_LDADD += $(top_builddir)/app/thrift/libthriftgen.la
fsearchd_LDADD += @FX_THRIFT_HOME@/lib/cpp/.libs/libthrift.la
fsearchd_LDADD += @FX_THRIFT_HOME@/lib/cpp/.libs/libthriftnb.la

frpcclient_LDADD += $(top_builddir)/app/thrift/libthriftgen.la
frpcclient_LDADD += @FX_THRIFT_HOME@/lib/cpp/.libs/libthrift.la
frpcclient_LDADD += @FX_LIBEVENT_HOME@/libevent.la
frpcclient_LDADD += @FX_LIBEVENT_HOME@/libevent_pthreads.la
endif


fsearchd_LDFLAGS = $(LINKER_OPTIONS)


