AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include

noinst_LTLIBRARIES = libfirtexext_com.la

com_sources = \
	Com.cpp\
	ComUtil.cpp\
	DllServer.cpp

libfirtexext_com_la_SOURCES = $(com_sources)

EXTRA_DIST = \
	ComUtil_POSIX.cpp\
	ComUtil_WIN32.cpp\
	DllMain_WIN32.cpp


