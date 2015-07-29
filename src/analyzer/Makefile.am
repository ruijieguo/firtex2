AM_CPPFLAGS = -I $(top_srcdir)/include \
	-I $(top_srcdir)/contrib \
	-I $(top_srcdir)/src \
	-I $(top_srcdir)/contrib/snowball/include \
	-I $(top_srcdir)/contrib/highlighter/include 

# this output override is needed because the c++ scanner is outputted into
# lex.yy.cc and the automake ylwrap script only checks for lex.yy.c
AM_LFLAGS=-olex.yy.c
AM_YFLAGS=-r state -v -g

noinst_LTLIBRARIES = libanalyzer.la

libanalyzer_la_SOURCES = \
	DateTimeBisonParser.yy\
	DateTimeLexer.ll\
	GBKTokenLexer.ll\
	UTF8TokenLexer.ll\
	y.tab.h\
	location.hh\
	position.hh\
	stack.hh\
	Analyzer.cpp\
	AnalyzerFactory.cpp\
	DateTimeAnalyzer.cpp\
	NumericAnalyzer.cpp\
	LowercaseFilter.cpp\
	StandardAnalyzer.cpp\
	StandardStopFilter.cpp\
	SingleWordAnalyzer.cpp\
	Token.cpp\
	TokenView.cpp\
	TokenSource.cpp\
	TokenFilter.cpp\
	AnalyzerMapper.cpp\
	KeywordAnalyzer.cpp\
	WhitespaceAnalyzer.cpp\
	SynonymFilter.cpp\
	SynonymMap.cpp\
	FilterFactory.cpp\
	DoubleArrayTrie.cpp\
	MaxForwardSegmenter.cpp\
	UnigramSegmenter.cpp

extraclean:
	$(RM) -f DateTimeBisonParser.h DateTimeBisonParser.cc DateTimeLexer.cc GBKTokenLexer.cc UTF8TokenLexer.cc DateTimeBisonParser.hh  location.hh  position.hh  stack.hh

rebuild: clean extraclean all
