include $(top_srcdir)/external/versions.am

AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/contrib/highlighter/include -Ithrift/gen-cpp

appsrcdir = $(top_srcdir)/app/fbench
common_incdir = $(top_srcdir)/app/common

bindir = ${exec_prefix}/firtex2/bin

bin_PROGRAMS = fbench

fbench_SOURCES = \
	BenchmarkAppRunner.cpp\
	BenchmarkMain.cpp

fbench_LDADD = $(top_builddir)/extension/src/libfirtexext.la \
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/app/common/libappcommon.la\
	$(top_builddir)/contrib/snowball/src/libfirtex_snowball.la\
	$(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la\
	$(top_builddir)/app/thrift/libthriftgen.la

external_srcdir = $(top_srcdir)/external

if WITH_BUNDLED_BOOST
AM_CPPFLAGS += -I$(external_srcdir)/$(BOOST)
endif

if WITH_BUNDLED_LIBEVENT
AM_CPPFLAGS += -I$(external_srcdir)/$(LIBEVENT)\
	-I$(external_srcdir)/$(LIBEVENT)/include

fbench_LDADD += $(external_srcdir)/$(LIBEVENT)/libevent.la
fbench_LDADD += $(external_srcdir)/$(LIBEVENT)/libevent_pthreads.la
else
fbench_LDADD += -llibevent
fbench_LDADD += -llibevent_pthreads
endif

if WITH_BUNDLED_THRIFT
fbench_LDADD += $(external_srcdir)/$(THRIFT)/lib/cpp/libthrift.la
fbench_LDADD += $(external_srcdir)/$(THRIFT)/lib/cpp/libthriftnb.la
AM_CPPFLAGS += -I$(external_srcdir)/$(THRIFT)/lib/cpp/src
else
fbench_LDADD += -llibthrift
fbench_LDADD += -llibthriftnb
endif


