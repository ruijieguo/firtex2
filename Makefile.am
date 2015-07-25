SUBDIRS = include src external extension contrib app plugins test bin win32 documentation sysdata conf bindings 

EXTRA_DIST =\
	bootstrap.sh\
	Doxygen

upload:
	make dist
	ncftpput -v upload.sourceforge.net /incoming @PACKAGE@-@VERSION@.tar.gz

doxygen:
	rm -fdr ./documentation/api_doc/html
	doxygen Doxygen
	cp ./documentation/api_doc/firtex_logo.png ./documentation/api_doc/html/firtex_logo.png
