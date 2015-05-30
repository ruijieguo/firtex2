AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/extension/include -I $(top_srcdir)/src/external

noinst_LTLIBRARIES = libfirtexext_collection.la

collection_sources = \
	Collection.cpp\
	CollectionFactory.cpp\
	StandardCollection.cpp\
	TrecCollection.cpp\
	DirectoryFileFetcher.cpp\
	MonitorDirectoryFileFetcher.cpp\
	DefaultFileFilter.cpp\
	CollectionConfigure.cpp\
	DirCollection.cpp\
	ConsoleProgressObserver.cpp

#	DirCollection.cpp
#	FileListCollection.cpp
#	FirteXCollection.cpp
#	TrecCollection.cpp
#	GZTrecCollection.cpp
#	ZIPTrecCollection.cpp
#	TWCollection.cpp
#	GZTWCollection.cpp


libfirtexext_collection_la_SOURCES = $(collection_sources)
