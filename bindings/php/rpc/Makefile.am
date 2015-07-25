include $(top_srcdir)/external/versions.am

THRIFT_SRC_PATH = $(top_srcdir)/app/thrift/

THRIFT_BIN = 
external_srcdir = $(top_srcdir)/external

if WITH_BUNDLED_THRIFT
THRIFT_BIN += $(external_srcdir)/$(THRIFT)/compiler/cpp/thrift
else
THRIFT_BIN += /usr/bin/thrift
endif


all:
	$(THRIFT_BIN) -r --gen php:cob_style $(THRIFT_SRC_PATH)SearchService.thrift
	rm -rf packages
	mkdir packages
	cp -fr gen-php/firtex ./packages/
	cp -fr ../FirteXQueryResult.php ./packages/firtex/
	cp -fr ../FirteXException.php ./packages/firtex/
#TODO:
	cp -fr $(external_srcdir)/$(THRIFT)/lib/php/lib ./

distclean-local:
	rm -rf gen-php packages protocol transport Thrift.php
