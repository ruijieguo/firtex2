AM_CPPFLAGS = -I../../../include -I../include

lib_LTLIBRARIES	= libfirtex_highlighter.la

libfirtex_highlighter_la_SOURCES  = \
	GradientFormatter.cpp\
	SimpleHTMLEncoder.cpp\
	Highlighter.cpp\
	SimpleHTMLFormatter.cpp\
	QueryScorer.cpp \
	TextFragment.cpp\
	QueryTermExtractor.cpp\
	TokenGroup.cpp\
	SimpleFragmenter.cpp


libfirtex_highlighter_la_CXXFLAGS = @CXXFLAGS@ -fPIC -shared
libfirtex_highlighter_la_CFLAGS = @CFLAGS@ -fPIC -shared


