fofi_srcdir = $(xpdf_srcdir)/fofi

fofi_sources = xpdf/fofi/FoFiBase.cc\
	xpdf/fofi/FoFiEncodings.cc\
	xpdf/fofi/FoFiTrueType.cc\
	xpdf/fofi/FoFiType1.cc\
	xpdf/fofi/FoFiType1C.cc

EXTRA_DIST += $(fofi_srcdir)/FoFiBase.h\
	$(fofi_srcdir)/FoFiEncodings.h\
	$(fofi_srcdir)/FoFiTrueType.h\
	$(fofi_srcdir)/FoFiType1.h\
	$(fofi_srcdir)/FoFiType1C.h

pdfparserplugin_so_SOURCES += $(fofi_sources)
