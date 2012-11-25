SUBDIRS = common utility thread external config document analyzer store index queryparser search .

# this output override is needed because the c++ scanner is outputted into
# lex.yy.cc and the automake ylwrap script only checks for lex.yy.c
AM_LFLAGS=-olex.yy.c
AM_YFLAGS=-r state -v -g

EXTRA_DIST = $(top_srcdir)/include/firtex/FirteX.h

lib_LTLIBRARIES = libfirtex.la 
libfirtex_la_SOURCES = 
libfirtex_la_LIBADD = \
	common/libcommon.la\
	utility/libutility.la\
	thread/libthread.la\
	external/zlib/libexternal_zlib.la\
	external/iconv/libexternal_iconv.la\
	config/libconfig.la\
	document/libdocument.la\
	analyzer/libanalyzer.la\
	store/libstore.la\
	index/libindex.la\
	queryparser/libqueryparser.la\
	search/libsearch.la
