include $(top_srcdir)/external/versions.am

AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include 

noinst_LTLIBRARIES = libfirtexext_util.la

libfirtexext_util_la_SOURCES = \
	Option.cpp\
	OptionParser.cpp\
	Application.cpp\
	ServerApplication.cpp\
	HelpDisplayAppRunner.cpp\
	SignalHandler.cpp\
	Process_POSIX.cpp\
	Regex.cpp

external_srcdir = $(top_srcdir)/external

if WITH_BUNDLED_PCRE2
AM_CPPFLAGS += -I$(external_srcdir)/$(PCRE2)/src
libfirtexext_util_la_LIBADD = $(external_srcdir)/$(PCRE2)/libpcre2-8.la
endif
