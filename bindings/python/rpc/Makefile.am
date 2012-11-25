THRIFT_SRC_PATH = $(top_srcdir)/app/thrift/
THRIFT_BIN = @FX_THRIFT_HOME@/compiler/cpp/thrift

all:
	$(THRIFT_BIN) -r --gen py:cob_style $(THRIFT_SRC_PATH)SearchService.thrift
	rm -rf thrift firtex __init__.py
	mkdir thrift firtex
	cp gen-py/__init__.py ./
	cp -r gen-py/firtex ./
	cp @FX_THRIFT_HOME@/lib/py/src/Thrift.py ./thrift/
	cp @FX_THRIFT_HOME@/lib/py/src/__init__.py ./thrift/
	cp -r @FX_THRIFT_HOME@/lib/py/src/protocol/ ./thrift/
	cp -r @FX_THRIFT_HOME@/lib/py/src/transport/ ./thrift/

distclean:
	rm -rf gen-py firtex thrift __init__.py
