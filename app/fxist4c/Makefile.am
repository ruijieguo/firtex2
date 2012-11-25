INCLUDES = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/contrib/highlighter/include

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
	 @FX_LIBEVENT_HOME@/libevent.la\
	 @FX_LIBEVENT_HOME@/libevent_pthreads.la

if HAVE_SNOWBALL
LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 

fxist4c_LDFLAGS = $(LINKER_OPTIONS)


