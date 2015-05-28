SUBDIRS = store thread common utility document analyzer config index queryparser search

INCLUDES = -I $(top_srcdir)/include\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/test/cppunit/include\
	-I $(top_srcdir)/test/src

noinst_LTLIBRARIES = libfirtextest_main.la

libfirtextest_main_la_SOURCES = \
	main.cpp\
	TestHelper.cpp

EXTRA_DIST = $(top_srcdir)/test/src/TestHelper.h
