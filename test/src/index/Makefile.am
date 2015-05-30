AM_CPPFLAGS = -I $(top_srcdir)/include\
	-I $(top_srcdir)/extension/include\
	-I $(top_srcdir)/test/cppunit/include\
	-I $(top_srcdir)/test/src

index_test_sources = \
	IndexBuildHelper.cpp\
	BarrelTermPositionIteratorTestCase.cpp\
	BarrelTermPostingIteratorTestCase.cpp\
	ByteSliceInputStreamTestCase.cpp\
	ByteSliceOutputStreamTestCase.cpp\
	DocPostingDecoderTestCase.cpp\
	DocPostingMergerTestCase.cpp\
	DocSkipListReaderTestCase.cpp\
	ForwardIndexTestCase.cpp\
	S16CodingTestCase.cpp\
	OptPFDPostingCodingTestCase.cpp\
	OptPForDeltaCodingTestCase.cpp\
	PosPostingDecoderTestCase.cpp\
	PosPostingMergerTestCase.cpp\
	PosSkipListReaderTestCase.cpp\
	DBTMergePolicyTestCase.cpp\
	IndexMergerTestCase.cpp\
	IndexTestCase.cpp\
	PrimaryKeyIndexTestCase.cpp\
	KeywordIndexTestCase.cpp\
	DateTimeIndexTestCase.cpp\
	TextIndexMergerTestCase.cpp\
	TextTermReaderTestCase.cpp\
	TypedDictTestCase.cpp\
	UnpackTestCase.cpp


EXTRA_DIST = \
	BarrelTermPositionIteratorTestCase.h\
	BarrelTermPostingIteratorTestCase.h\
	DateTimeIndexTestCase.h\
	ForwardIndexTestCase.h\
	IndexContentTestCase.h\
	IndexTestCase.h\
	KeywordIndexTestCase.h\
	NumberIndexTestCase.h\
	PrimaryKeyIndexTestCase.h

check_PROGRAMS  = index_test

index_test_SOURCES = $(index_test_sources)

index_test_LDADD= \
	$(top_builddir)/src/libfirtex.la\
	$(top_builddir)/test/src/libfirtextest_main.la\
	$(top_builddir)/test/cppunit/libcppunit.la\
	@FX_LIBEVENT_HOME@/libevent.la\
	@FX_LIBEVENT_HOME@/libevent_pthreads.la\
	 -lpthread -ldl

if HAVE_SNOWBALL
index_test_LDADD += $(top_builddir)/contrib/snowball/src/libfirtex_snowball.la
endif 

if HAVE_HIGHLIGHTER
index_test_LDADD += $(top_builddir)/contrib/highlighter/src/libfirtex_highlighter.la
endif 

index_test_LDFLAGS = $(LIBADD_DL)

if HAVE_TCMALLOC
#index_test_LDADD += @FX_TCMALLOC_HOME@/libtcmalloc.la
endif

TESTS_ENVIRONMENT=LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64 \
	HEAPCHECK=normal

TESTS = $(check_PROGRAMS)

