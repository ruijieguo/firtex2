include $(top_srcdir)/external/versions.am

AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/contrib/highlighter/include -Ithrift/gen-cpp

appsrcdir = $(top_srcdir)/app/watchdog
common_incdir = $(top_srcdir)/app/common

bindir = ${exec_prefix}/firtex2/bin

bin_PROGRAMS = fwatchdog

fwatchdog_SOURCES = \
	HTTPWatchdogServer.cpp\
	HTTPWatchdogService.cpp\
	WatchdogServiceBase.cpp\
	WatchdogAppRunner.cpp\
	WatchdogMain.cpp\
	RPCWatchdogServer.cpp\
	RPCWatchdogService.cpp

fwatchdog_LDADD = $(top_builddir)/extension/src/libfirtexext.la \
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/app/common/libappcommon.la\
	$(top_builddir)/contrib/snowball/src/libfirtex_snowball.la\
	$(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la\
	$(top_builddir)/app/thrift/libthriftgen.la

external_srcdir = $(top_srcdir)/external

if WITH_BUNDLED_BOOST
AM_CPPFLAGS += -I$(external_srcdir)/$(BOOST)
endif

if WITH_BUNDLED_PCRE2
AM_CPPFLAGS += -I$(external_srcdir)/$(PCRE2)/src
endif


if WITH_BUNDLED_LIBEVENT
AM_CPPFLAGS += -I$(external_srcdir)/$(LIBEVENT)\
	-I$(external_srcdir)/$(LIBEVENT)/include

fwatchdog_LDADD += $(external_srcdir)/$(LIBEVENT)/libevent.la
fwatchdog_LDADD += $(external_srcdir)/$(LIBEVENT)/libevent_pthreads.la
else
fwatchdog_LDADD += -llibevent
fwatchdog_LDADD += -llibevent_pthreads
endif

if WITH_BUNDLED_THRIFT
fwatchdog_LDADD += $(external_srcdir)/$(THRIFT)/lib/cpp/libthriftnb.la
fwatchdog_LDADD += $(external_srcdir)/$(THRIFT)/lib/cpp/libthrift.la
AM_CPPFLAGS += -I$(external_srcdir)/$(THRIFT)/lib/cpp/src
else
fwatchdog_LDADD += -llibthrift
fwatchdog_LDADD += -llibthriftnb
endif


