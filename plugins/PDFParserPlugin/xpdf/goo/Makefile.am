goo_srcdir = $(xpdf_srcdir)/goo

goo_sources = $(goo_srcdir)/FixedPoint.cc\
	$(goo_srcdir)/GHash.cc\
	$(goo_srcdir)/GList.cc\
	$(goo_srcdir)/GString.cc\
	$(goo_srcdir)/gfile.cc\
	$(goo_srcdir)/gmem.cc\
	$(goo_srcdir)/gmempp.cc

EXTRA_DIST += $(goo_srcdir)/FixedPoint.h\
	$(goo_srcdir)/GHash.h\
	$(goo_srcdir)/GList.h\
	$(goo_srcdir)/GMutex.h\
	$(goo_srcdir)/GString.h\
	$(goo_srcdir)/gfile.h\
	$(goo_srcdir)/gmem.h\
	$(goo_srcdir)/gtypes.h


pdfparserplugin_so_SOURCES += $(goo_sources)