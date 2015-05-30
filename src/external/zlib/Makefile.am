AM_CPPFLAGS = -I $(top_srcdir)/include -I $(top_srcdir)/src/external

noinst_LTLIBRARIES = libexternal_zlib.la

libexternal_zlib_la_SOURCES =\
	deflate.c\
	inflate.c\
	uncompr.c\
	adler32.c\
	infback.c\
	inftrees.c\
	compress.c\
	inffast.c\
	crc32.c\
	trees.c\
	gvmat32.asm\
	gvmat32c.c\
	gzio.c\
	inffas32.asm\
	ioapi.c\
	unzip.c\
	zip.c\
	zutil.c

zlib_headers = crc32.h\
	crypt.h\
	deflate.h\
	inffast.h\
	inffixed.h\
	inflate.h\
	inftrees.h\
	ioapi.h\
	iowin32.h\
	trees.h\
	unzip.h\
	zconf.h\
	zip.h\
	zlib.h\
	zutil.h\
	gvmat32.asm\
	inffas32.asm

EXTRA_DIST = iowin32.c\
	$(zlib_headers)
