fofi_srcdir = $(xpdf_srcdir)/fofi

fofi_sources = $(fofi_srcdir)/FoFiBase.cc\
	$(fofi_srcdir)/FoFiEncodings.cc\
	$(fofi_srcdir)/FoFiTrueType.cc\
	$(fofi_srcdir)/FoFiType1.cc\
	$(fofi_srcdir)/FoFiType1C.cc

EXTRA_DIST += $(fofi_srcdir)/FoFiBase.h\
	$(fofi_srcdir)/FoFiEncodings.h\
	$(fofi_srcdir)/FoFiTrueType.h\
	$(fofi_srcdir)/FoFiType1.h\
	$(fofi_srcdir)/FoFiType1C.h

pdfparserplugin_so_SOURCES += $(fofi_sources)