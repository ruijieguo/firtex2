INCLUDES = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/contrib/highlighter/include
appsrcdir = $(top_srcdir)/app/fbuilder

bindir = ${exec_prefix}/firtex2/bin

bin_PROGRAMS = fbuilder

fbuilder_SOURCES = $(appsrcdir)/main.cpp \
	$(appsrcdir)/BuilderAppRunner.cpp

LDADD = $(top_builddir)/extension/src/libfirtexext.la \
	$(top_builddir)/src/libfirtex.la\
	 @FX_LIBEVENT_HOME@/libevent.la\
	 @FX_LIBEVENT_HOME@/libevent_pthreads.la

if HAVE_SNOWBALL
LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 

fbuilder_LDFLAGS = $(LINKER_OPTIONS)


