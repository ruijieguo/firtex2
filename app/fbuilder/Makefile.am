include $(top_srcdir)/external/versions.am
AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/contrib/highlighter/include
AUTOMAKE_OPTIONS = subdir-objects

appsrcdir = $(top_srcdir)/app/fbuilder

bindir = ${exec_prefix}/firtex2/bin

bin_PROGRAMS = fbuilder

fbuilder_SOURCES = main.cpp \
	BuilderAppRunner.cpp

LDADD = $(top_builddir)/extension/src/libfirtexext.la \
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/contrib/snowball/src/libfirtex_snowball.la\
	$(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la

external_srcdir = $(top_srcdir)/external

if WITH_BUNDLED_LIBEVENT
AM_CPPFLAGS += -I$(external_srcdir)/$(LIBEVENT)\
	-I$(external_srcdir)/$(LIBEVENT)/include

LDADD += $(external_srcdir)/$(LIBEVENT)/libevent.la
LDADD += $(external_srcdir)/$(LIBEVENT)/libevent_pthreads.la
else
LDADD += -llibevent
LDADD += -llibevent_pthreads
endif

fbuilder_LDFLAGS = $(LINKER_OPTIONS)


