SUBDIRS = win32 snowball highlighter

EXTRA_DIST =\
	autogen.sh

upload:
	make dist
	ncftpput -v upload.sourceforge.net /incoming @PACKAGE@-@VERSION@.tar.gz

doxygen:
	rm -fdr ./doc/html
	doxygen Doxygen
	cp ./doc/firtex_logo.png ./doc/html/firtex_logo.png
