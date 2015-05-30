AM_CPPFLAGS = -I../../../include -I../include
# -I../src/stemmer

lib_LTLIBRARIES = libfirtex_snowball.la

libfirtex_snowball_la_SOURCES  =\
	SnowballFilter.cpp\
	SnowballWrapper.cpp\
	stemmer/include/libstemmer.h\
	stemmer/libstemmer/modules.h\
	stemmer/libstemmer/libstemmer.c\
	stemmer/runtime/api.h\
	stemmer/runtime/header.h\
	stemmer/runtime/api.c\
	stemmer/runtime/utilities.c\
	stemmer/src_c/stem_ISO_8859_1_danish.c\
	stemmer/src_c/stem_ISO_8859_1_porter.h\
	stemmer/src_c/stem_UTF_8_german.c\
	stemmer/src_c/stem_ISO_8859_1_danish.h\
	stemmer/src_c/stem_ISO_8859_1_portuguese.c\
	stemmer/src_c/stem_UTF_8_german.h\
	stemmer/src_c/stem_ISO_8859_1_dutch.c\
	stemmer/src_c/stem_ISO_8859_1_portuguese.h\
	stemmer/src_c/stem_UTF_8_hungarian.c\
	stemmer/src_c/stem_ISO_8859_1_dutch.h\
	stemmer/src_c/stem_ISO_8859_1_spanish.c\
	stemmer/src_c/stem_UTF_8_hungarian.h\
	stemmer/src_c/stem_ISO_8859_1_english.c\
	stemmer/src_c/stem_ISO_8859_1_spanish.h\
	stemmer/src_c/stem_UTF_8_italian.c\
	stemmer/src_c/stem_ISO_8859_1_english.h\
	stemmer/src_c/stem_ISO_8859_1_swedish.c\
	stemmer/src_c/stem_UTF_8_italian.h\
	stemmer/src_c/stem_ISO_8859_1_finnish.c\
	stemmer/src_c/stem_ISO_8859_1_swedish.h\
	stemmer/src_c/stem_UTF_8_norwegian.c\
	stemmer/src_c/stem_ISO_8859_1_finnish.h\
	stemmer/src_c/stem_KOI8_R_russian.c\
	stemmer/src_c/stem_UTF_8_norwegian.h\
	stemmer/src_c/stem_ISO_8859_1_french.c\
	stemmer/src_c/stem_KOI8_R_russian.h\
	stemmer/src_c/stem_UTF_8_porter.c\
	stemmer/src_c/stem_ISO_8859_1_french.h\
	stemmer/src_c/stem_UTF_8_danish.c\
	stemmer/src_c/stem_UTF_8_porter.h\
	stemmer/src_c/stem_ISO_8859_1_german.c\
	stemmer/src_c/stem_UTF_8_danish.h\
	stemmer/src_c/stem_UTF_8_portuguese.c\
	stemmer/src_c/stem_ISO_8859_1_german.h\
	stemmer/src_c/stem_UTF_8_dutch.c\
	stemmer/src_c/stem_UTF_8_portuguese.h\
	stemmer/src_c/stem_ISO_8859_1_hungarian.c\
	stemmer/src_c/stem_UTF_8_dutch.h\
	stemmer/src_c/stem_UTF_8_russian.c\
	stemmer/src_c/stem_ISO_8859_1_hungarian.h\
	stemmer/src_c/stem_UTF_8_english.c\
	stemmer/src_c/stem_UTF_8_russian.h\
	stemmer/src_c/stem_ISO_8859_1_italian.c\
	stemmer/src_c/stem_UTF_8_english.h\
	stemmer/src_c/stem_UTF_8_spanish.c\
	stemmer/src_c/stem_ISO_8859_1_italian.h\
	stemmer/src_c/stem_UTF_8_finnish.c\
	stemmer/src_c/stem_UTF_8_spanish.h\
	stemmer/src_c/stem_ISO_8859_1_norwegian.c\
	stemmer/src_c/stem_UTF_8_finnish.h\
	stemmer/src_c/stem_UTF_8_swedish.c\
	stemmer/src_c/stem_ISO_8859_1_norwegian.h\
	stemmer/src_c/stem_UTF_8_french.c\
	stemmer/src_c/stem_UTF_8_swedish.h\
	stemmer/src_c/stem_ISO_8859_1_porter.c\
	stemmer/src_c/stem_UTF_8_french.h

EXTRA_DIST = \
	stemmer/libstemmer/*.txt

libfirtex_snowball_a_CXXFLAGS = @CXXFLAGS@ -fPIC -shared
libfirtex_snowball_a_CFLAGS = @CFLAGS@ -fPIC -shared


