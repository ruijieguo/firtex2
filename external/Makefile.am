SUBDIRS = . 

CONFIGURE_ARGS = @CONFIGURE_ARGS@ --enable-shared=no --with-pic --srcdir=.

include versions.am

EXTRA_DIST =		\
  $(PCRE2).tar.gz	\
  $(LIBEVENT).tar.gz	\
  $(ZOOKEEPER).tar.gz	\
  $(BOOST).tar.gz	\
  $(THRIFT).tar.gz


CLEAN_EXTRACTED =	\
  $(PCRE2)		\
  $(LIBEVENT)		\
  $(ZOOKEEPER)		\
  $(BOOST)		\
  $(THRIFT)

%-stamp: %.tar.gz
	gzip -d -c $^ | tar xf -
	test ! -e $(srcdir)/$*.patch || patch -d $* -p1 <$(srcdir)/$*.patch
	touch $@

thrift_cppflags = 
thrift_ldflags = 
thrift_withflags =
thrift_ld_lib_path =
external_srcdir = $(abs_top_srcdir)/external

# Dependencies for all-local.
ALL_LOCAL = # Initialize ALL_LOCAL so we can add to it.

if WITH_BUNDLED_PCRE2
  $(PCRE2)/libpcre2-posix.la: $(PCRE2)-stamp
	cd $(PCRE2) && ./configure $(CONFIGURE_ARGS) CFLAGS=-Dregcomp=PCRE2regcomp && \
	  $(MAKE) $(AM_MAKEFLAGS)

  ALL_LOCAL += $(PCRE2)/libpcre2-posix.la
  ALL_LOCAL += $(PCRE2)-stamp

endif

if WITH_BUNDLED_LIBEVENT
  $(LIBEVENT)/libevent.la: $(LIBEVENT)-stamp
	cd $(LIBEVENT) && ./configure $(CONFIGURE_ARGS) && \
	  $(MAKE) $(AM_MAKEFLAGS)

  ALL_LOCAL += $(LIBEVENT)/libevent.la
  ALL_LOCAL += $(LIBEVENT)-stamp

  thrift_cppflags += -I$(external_srcdir)/$(LIBEVENT)\
	-I$(external_srcdir)/$(LIBEVENT)/include
  thrift_ldflags += -L$(external_srcdir)/$(LIBEVENT)/.libs
  thrift_withflags += --with-libevent=$(external_srcdir)/$(LIBEVENT) --enable-tests=no
  thrift_ld_lib_path += $(external_srcdir)/$(LIBEVENT)/.libs
endif

if WITH_BUNDLED_ZOOKEEPER
  $(ZOOKEEPER)/src/c/libzookeeper_mt.la: $(ZOOKEEPER)-stamp
	cd $(ZOOKEEPER)/src/c && ./configure $(CONFIGURE_ARGS) && \
	  $(MAKE) $(AM_MAKEFLAGS)

  ALL_LOCAL += $(ZOOKEEPER)/src/c/libzookeeper_mt.la
  ALL_LOCAL += $(ZOOKEEPER)-stamp
endif

if WITH_BUNDLED_BOOST
  ALL_LOCAL += $(BOOST)-stamp
  thrift_cppflags += -I$(external_srcdir)/$(BOOST)
  thrift_withflags += --with-boost=$(external_srcdir)/$(BOOST)
endif

if WITH_BUNDLED_THRIFT
  $(THRIFT)/lib/cpp/libthrift.la $(THRIFT)/lib/cpp/libthriftnb.la: $(THRIFT)-stamp
	cd $(THRIFT) && ./configure $(CONFIGURE_ARGS) $(thrift_withflags) CPPFLAGS='$(thrift_cppflags)' LDFLAGS='$(thrift_ldflags)' LD_LIBRARY_PATH='$(thrift_ld_lib_path)' && \
	  $(MAKE) $(AM_MAKEFLAGS)

  ALL_LOCAL += $(THRIFT)/lib/cpp/libthrift.la
  ALL_LOCAL += $(THRIFT)/lib/cpp/libthriftnb.la
  ALL_LOCAL += $(THRIFT)-stamp
endif

all-local: $(ALL_LOCAL)

clean-local:
	rm -r -f $(CLEAN_EXTRACTED)
	rm -f *-stamp
