AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/contrib/highlighter/include -I@FX_BOOST_HOME@ -I@FX_THRIFT_HOME@/lib/cpp/src -Ithrift/gen-cpp -I@FX_LIBEVENT_HOME@ -I@FX_LIBEVENT_HOME@/include

appsrcdir = $(top_srcdir)/app/watchdog
common_incdir = $(top_srcdir)/app/common

bindir = ${exec_prefix}/firtex2/bin

bin_PROGRAMS = fwatchdog

fwatchdog_SOURCES = \
	HTTPWatchdogServer.cpp\
	HTTPWatchdogService.cpp\
	WatchdogServiceBase.cpp\
	WatchdogAppRunner.cpp\
	WatchdogMain.cpp

if HAVE_THRIFT
fwatchdog_SOURCES += \
	RPCWatchdogServer.cpp\
	RPCWatchdogService.cpp
endif

fwatchdog_LDADD = $(top_builddir)/extension/src/libfirtexext.la \
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/app/common/libappcommon.la

if HAVE_SNOWBALL
fwatchdog_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
fwatchdog_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 

fwatchdog_LDADD += @FX_LIBEVENT_HOME@/libevent.la
fwatchdog_LDADD += @FX_LIBEVENT_HOME@/libevent_pthreads.la

if HAVE_THRIFT
fwatchdog_LDADD += $(top_builddir)/app/thrift/libthriftgen.la
fwatchdog_LDADD += @FX_THRIFT_HOME@/lib/cpp/.libs/libthrift.la
fwatchdog_LDADD += @FX_THRIFT_HOME@/lib/cpp/.libs/libthriftnb.la
endif

fwatchdog_LDFLAGS = $(LINKER_OPTIONS)


