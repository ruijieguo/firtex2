include $(top_srcdir)/external/versions.am
SUBDIRS = processor collection com network plugin util .

lib_LTLIBRARIES = libfirtexext.la

libfirtexext_la_SOURCES = 
libfirtexext_la_LIBADD = \
	collection/libfirtexext_collection.la\
	processor/libfirtexext_processor.la\
	com/libfirtexext_com.la\
	network/libfirtexext_network.la\
	plugin/libfirtexext_plugin.la\
	util/libfirtexext_util.la

external_srcdir = $(top_srcdir)/external

if WITH_BUNDLED_PCRE2
libfirtexext_la_LIBADD += $(external_srcdir)/$(PCRE2)/libpcre2-8.la
endif

if WITH_BUNDLED_LIBEVENT
libfirtexext_la_LIBADD += $(external_srcdir)/$(LIBEVENT)/libevent_pthreads.la
libfirtexext_la_LIBADD += $(external_srcdir)/$(LIBEVENT)/libevent.la
endif

