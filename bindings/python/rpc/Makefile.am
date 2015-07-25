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
	$(THRIFT_BIN) -r --gen py:cob_style $(THRIFT_SRC_PATH)SearchService.thrift
	rm -rf thrift firtex __init__.py
	mkdir thrift firtex
	cp gen-py/__init__.py ./
	cp -r gen-py/firtex ./
#TODO:
	cp $(external_srcdir)/$(THRIFT)/lib/py/src/Thrift.py ./thrift/
	cp $(external_srcdir)/$(THRIFT)/lib/py/src/__init__.py ./thrift/
	cp -r $(external_srcdir)/$(THRIFT)/lib/py/src/protocol/ ./thrift/
	cp -r $(external_srcdir)/$(THRIFT)/lib/py/src/transport/ ./thrift/

distclean-local:
	rm -rf gen-py firtex thrift __init__.py
