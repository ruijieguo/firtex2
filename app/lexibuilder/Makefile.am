INCLUDES = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/app
appsrcdir = $(top_srcdir)/app/lexibuilder

bindir = ${exec_prefix}/firtex2/bin

bin_PROGRAMS = lexibuilder

lexibuilder_SOURCES = \
	$(appsrcdir)/main.cpp\
	$(appsrcdir)/LexiconBuilderAppRunner.cpp

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


lexibuilder_LDFLAGS = $(LINKER_OPTIONS)


