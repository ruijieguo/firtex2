AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/contrib/highlighter/include -I@FX_BOOST_HOME@ -I@FX_THRIFT_HOME@/lib/cpp/src -Ithrift/gen-cpp -I@FX_LIBEVENT_HOME@ -I@FX_LIBEVENT_HOME@/include

appsrcdir = $(top_srcdir)/app/fbench
common_incdir = $(top_srcdir)/app/common

bindir = ${exec_prefix}/firtex2/bin

bin_PROGRAMS = fbench

fbench_SOURCES = \
	BenchmarkAppRunner.cpp\
	BenchmarkMain.cpp

fbench_LDADD = $(top_builddir)/extension/src/libfirtexext.la \
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/app/common/libappcommon.la

if HAVE_SNOWBALL
fbench_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
fbench_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 

fbench_LDADD += @FX_LIBEVENT_HOME@/libevent.la
fbench_LDADD += @FX_LIBEVENT_HOME@/libevent_pthreads.la

if HAVE_THRIFT
fbench_LDADD += $(top_builddir)/app/thrift/libthriftgen.la
fbench_LDADD += @FX_THRIFT_HOME@/lib/cpp/.libs/libthrift.la
fbench_LDADD += @FX_THRIFT_HOME@/lib/cpp/.libs/libthriftnb.la
endif

fbench_LDFLAGS = $(LINKER_OPTIONS)


