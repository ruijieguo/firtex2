include $(top_srcdir)/external/versions.am
AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/contrib/highlighter/include

bindir = ${exec_prefix}/firtex2/bin

bin_PROGRAMS = fxist4c

fxist4c_SOURCES = \
	main.cpp \
	DocumentOverviewAppRunner.cpp \
	DocumentOverviewAppRunner.h \
	FileIndexerAppRunner.cpp \
	FileIndexerAppRunner.h \
	IndexBuilderAppRunner.cpp \
	IndexBuilderAppRunner.h \
	IndexMergerAppRunner.cpp \
	IndexMergerAppRunner.h \
	IndexOverviewAppRunner.cpp \
	IndexOverviewAppRunner.h \
	IndexPrinterAppRunner.cpp \
	IndexPrinterAppRunner.h \
	IndexSearcherAppRunner.cpp \
	IndexSearcherAppRunner.h

LDADD = $(top_builddir)/extension/src/libfirtexext.la \
	$(top_builddir)/src/libfirtex.la \
	$(top_builddir)/app/common/libappcommon.la\
	$(top_builddir)/contrib/snowball/src/libfirtex_snowball.la\
	$(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la

external_srcdir = $(top_srcdir)/external

if WITH_BUNDLED_LIBEVENT
LDADD += $(external_srcdir)/$(LIBEVENT)/libevent.la
LDADD += $(external_srcdir)/$(LIBEVENT)/libevent_pthreads.la
else
LDADD += -llibevent
LDADD += -llibevent_pthreads
endif
