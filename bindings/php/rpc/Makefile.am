THRIFT_SRC_PATH = $(top_srcdir)/app/thrift/
THRIFT_BIN = @FX_THRIFT_HOME@/compiler/cpp/thrift

all:
	$(THRIFT_BIN) -r --gen php:cob_style $(THRIFT_SRC_PATH)SearchService.thrift
	rm -rf packages
	mkdir packages
	cp -fr gen-php/SearchService ./packages/
	cp @FX_THRIFT_HOME@/lib/php/src/Thrift.php ./
	cp @FX_THRIFT_HOME@/lib/php/src/TStringUtils.php ./
	cp -fr @FX_THRIFT_HOME@/lib/php/lib/Thrift/Protocol/ ./
	cp -fr @FX_THRIFT_HOME@/lib/php/lib/Thrift/Transport/ ./

distclean:
	rm -rf gen-php packages protocol transport Thrift.php
