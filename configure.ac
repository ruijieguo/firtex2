
AC_INIT(firtex, [0.5.1])
AC_CANONICAL_TARGET
AM_CONFIG_HEADER(include/firtex/config.h)
AM_INIT_AUTOMAKE([subdir-objects]) 

# gruard against pre defined config.h
AH_TOP([
#ifndef __CONFIG_H
#define __CONFIG_H
])
AH_BOTTOM([
#endif
])


AC_CANONICAL_HOST
AC_CANONICAL_BUILD

AC_LANG([C++])

AC_CONFIG_MACRO_DIR([m4])

LT_PREREQ([2.2])
LT_INIT
LT_LANG([C++])
LT_OUTPUT


AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
_LT_SET_OPTION([LT_INIT],[dlopen])
AC_DIAGNOSE([obsolete],[_LT_SET_OPTION([LT_INIT],[dlopen])
AC_DIAGNOSE([obsolete],[AC_LIBTOOL_DLOPEN: Remove this warning and the call to _LT_SET_OPTION when you
put the `dlopen' option into LT_INIT's first parameter.])
: Remove this warning and the call to _LT_SET_OPTION when you
put the `dlopen' option into LT_INIT's first parameter.])

LT_INIT
AC_PROG_YACC
AC_PROG_LEX
AM_PROG_LEX
AC_LANG([C++]) 

dnl Checks for library functions.
AC_FUNC_VPRINTF

ac_cv_use_rtlib=yes

dnl Setup compiler/linker options based on the target type
case "$target" in
*-*-mingw32*)
    dnl MingW32 should use native win32 threads
    ;;
*-apple-darwin*)
    dnl Use the Mac multiprocessing library
    LINKER_OPTIONS="-framework CoreServices"
    ac_cv_use_rtlib=no
    ;;
*)
    dnl Assume pthreads
    if test x"$PTHREAD_CFLAGS" != x; then
      COMPILER_OPTIONS="$PTHREAD_CFLAGS"
    fi
    if test x"$PTHREAD_LIBS" != x; then
      LINKER_OPTIONS="$PTHREAD_LIBS"
    fi
    ;;
esac

dnl Set debug compilation flags
AC_ARG_WITH([debug], 
	AS_HELP_STRING([--with-debug],[compile debug version (default is disabled)]),
	[ac_cv_use_debug=$withval], [ac_cv_use_debug=no])

AC_MSG_CHECKING([whether to compile debug version])
		
if test x$ac_cv_use_debug != xno; then
   COMPILER_OPTIONS="$COMPILER_OPTIONS -Wall -g -D_DEBUG -D_FILE_OFFSET_BITS=64 -fno-strict-aliasing -Wno-unused-local-typedefs -Wno-parentheses -msse2"
   REQUIRED_OPTIONS="-D_DEBUG -D_FILE_OFFSET_BITS=64"
   AC_MSG_RESULT([yes])
else
   COMPILER_OPTIONS="$COMPILER_OPTIONS -Wall -g -D_FILE_OFFSET_BITS=64 -O3 -DNDEBUG -fno-strict-aliasing -Wno-unused-local-typedefs -Wno-parentheses -msse2"
   REQUIRED_OPTIONS="-DNDEBUG -D_FILE_OFFSET_BITS=64"
   AC_MSG_RESULT([no])
fi

dnl Set profile compilation flags
AC_ARG_WITH([profile], 
	AS_HELP_STRING([--with-profile],[compile profile version (default is disabled)]),
	[ac_cv_use_profile=$withval], [ac_cv_use_profile=no])

AC_MSG_CHECKING([whether to compile profile version])
		
if test x$ac_cv_use_profile != xno; then
   COMPILER_OPTIONS="$COMPILER_OPTIONS -pg"
   AC_MSG_RESULT([yes])
else
   AC_MSG_RESULT([no])
fi

dnl Set tcmalloc compilation flags
FX_TCMALLOC_HOME=/usr/lib
AC_ARG_WITH([tcmalloc], 
	AS_HELP_STRING([--with-tcmalloc],[compile tcmalloc version (default is disabled)]),
	[ac_cv_use_tcmalloc=$withval], [ac_cv_use_tcmalloc=no])

AC_MSG_CHECKING([whether to compile with tcmalloc for heap checking])
		
if test x$ac_cv_use_tcmalloc != xno; then
   if test x$ac_cv_use_tcmalloc != xyes; then
      FX_TCMALLOC_HOME=$ac_cv_use_tcmalloc
   fi
   AC_MSG_RESULT([yes])
else
   AC_MSG_RESULT([no])
fi

AC_SUBST(FX_TCMALLOC_HOME)
AM_CONDITIONAL([HAVE_TCMALLOC], [test x$ac_cv_use_tcmalloc != xno])

dnl Set snowball compilation flags
AC_ARG_WITH([snowball], 
	AS_HELP_STRING([--with-snowball],[compile version with snowball stemmer (default is disabled)]),
	[ac_cv_use_snowball=yes], [ac_cv_use_snowball=no])

AC_MSG_CHECKING([whether to compile snowball version])
		
if test x$ac_cv_use_snowball != xno; then
   COMPILER_OPTIONS="$COMPILER_OPTIONS -DHAVE_SNOWBALL"
   REQUIRED_OPTIONS="$REQUIRED_OPTIONS -DHAVE_SNOWBALL"
   AC_MSG_RESULT([yes])
else
   AC_MSG_RESULT([no])
fi

AM_CONDITIONAL([HAVE_SNOWBALL], [test x$ac_cv_use_snowball != xno])

dnl Set highlighter compilation flags
AC_ARG_WITH([highlighter], 
	AS_HELP_STRING([--without-highlighter],[compile version without highlighter (default is enable)]),
	[ac_cv_use_highlighter=no], [ac_cv_use_highlighter=yes])

AC_MSG_CHECKING([whether to compile highlighter version])
		
if test x$ac_cv_use_highlighter == xno; then
   AC_MSG_RESULT([no])
else
   COMPILER_OPTIONS="$COMPILER_OPTIONS -DHAVE_HIGHLIGHTER"
   REQUIRED_OPTIONS="-DHAVE_HIGHLIGHTER"
   AC_MSG_RESULT([yes])
fi

AM_CONDITIONAL([HAVE_HIGHLIGHTER], [test x$ac_cv_use_highlighter != xno])

dnl Set user specified boost path
FX_BOOST_HOME=`pwd`/external
AC_ARG_WITH([boost], 
	AS_HELP_STRING([--with-boost],[specify boost home path (default using the built-in mini boost)]),
	[ac_cv_use_boost=$withval], [ac_cv_use_boost=no])

AC_MSG_CHECKING([whether to compile with user specified boost])
		
if test x$ac_cv_use_boost != xno; then
   FX_BOOST_HOME=$ac_cv_use_boost
   AC_MSG_RESULT([yes])
else
   AC_MSG_RESULT([no])
fi

AC_SUBST(FX_BOOST_HOME)

dnl Set user specified libevent path
FX_LIBEVENT_HOME=`pwd`/external/libevent
AC_ARG_WITH([libevent], 
	AS_HELP_STRING([--with-libevent],[specify libevent home path (default using the built-in libevent)]),
	[ac_cv_use_libevent=$withval], [ac_cv_use_libevent=yes])

AC_MSG_CHECKING([whether to compile with libevent])

FX_LIBEVENT_DIR=
if test x$ac_cv_use_libevent = xyes; then
   FX_LIBEVENT_DIR=libevent
   AC_MSG_RESULT([built-in])
elif test x$ac_cv_use_libevent != xno; then
   FX_LIBEVENT_HOME=$ac_cv_use_libevent
   AC_MSG_RESULT([yes])
else
   AC_MSG_RESULT([no])
fi

AM_CONDITIONAL([HAVE_LIBEVENT], [test x$ac_cv_use_libevent != xno])
AC_SUBST(FX_LIBEVENT_HOME)
AC_SUBST(FX_LIBEVENT_DIR)

dnl Set user specified libevent path
FX_ZOOKEEPER_C_HOME=`pwd`/external/zookeeper_c
AC_ARG_WITH([zookeeper], 
	AS_HELP_STRING([--with-zookeeper],[specify zookeeper home path (default using the built-in zookeeper)]),
	[ac_cv_use_zookeeper=$withval], [ac_cv_use_zookeeper=yes])

AC_MSG_CHECKING([whether to compile with zookeeper])

FX_ZOOKEEPER_C_DIR=
if test x$ac_cv_use_zookeeper = xyes; then
   FX_ZOOKEEPER_C_DIR=zookeeper_c
   AC_MSG_RESULT([built-in])
   COMPILER_OPTIONS="$COMPILER_OPTIONS -DHAVE_ZOOKEEPER"
elif test x$ac_cv_use_zookeeper != xno; then
   FX_ZOOKEEPER_C_HOME=$ac_cv_use_zookeeper
   AC_MSG_RESULT([yes])
   COMPILER_OPTIONS="$COMPILER_OPTIONS -DHAVE_ZOOKEEPER"
else
   AC_MSG_RESULT([no])
fi

AM_CONDITIONAL([HAVE_ZOOKEEPER], [test x$ac_cv_use_zookeeper != xno])
AC_SUBST(FX_ZOOKEEPER_C_HOME)
AC_SUBST(FX_ZOOKEEPER_C_DIR)

dnl Set user specified thrift path
FX_THRIFT_HOME=`pwd`/external/thrift
AC_ARG_WITH([thrift], 
	AS_HELP_STRING([--with-thrift],[specify thrift home path (default using the built-in thrift)]),
	[ac_cv_use_thrift=$withval], [ac_cv_use_thrift=yes])

AC_MSG_CHECKING([whether to compile with thrift])

FX_THRIFT_DIR=
if test x$ac_cv_use_thrift = xyes; then
   FX_THRIFT_DIR=thrift
   AC_MSG_RESULT([built-in])   
   COMPILER_OPTIONS="$COMPILER_OPTIONS -DHAVE_THRIFT"
elif test x$ac_cv_use_thrift != xno; then
   FX_THRIFT_HOME=$ac_cv_use_thrift
   AC_MSG_RESULT([yes])
   COMPILER_OPTIONS="$COMPILER_OPTIONS -DHAVE_THRIFT"
else
   AC_MSG_RESULT([no])
fi

AM_CONDITIONAL([HAVE_THRIFT], [test x$ac_cv_use_thrift != xno])
AC_SUBST(FX_THRIFT_HOME)
AC_SUBST(FX_THRIFT_DIR)

dnl set flags for C compiler if there are no user overrides
if test x$ac_env_CFLAGS_set != xset; then
   CFLAGS=$COMPILER_OPTIONS
else 
   CFLAGS="$CFLAGS $REQUIRED_OPTIONS"
fi

dnl set flags for C++ compiler if there are no user overrides
if test x$ac_env_CXXFLAGS_set != xset; then
   CXXFLAGS=$COMPILER_OPTIONS
else
   CXXFLAGS="$CXXFLAGS $REQUIRED_OPTIONS"
fi

dnl Check for dl
dnl AC_CHECK_LIB(dl, dlopen, [LIBS="$LIBS -ldl"], exit 1)


dnl Check for rt
if test x$ac_cv_use_rtlib != xno; then
   AC_CHECK_LIB(rt, shm_open, [LIBS="$LIBS -lrt"], exit 1)
else 
   AC_CHECK_LIB(c, shm_open, [LIBS="$LIBS -lc"], exit 1)
fi


dnl Check for pthread
dnl AC_CHECK_LIB(pthread, pthread_create, [LIBS="$LIBS -lpthread"], exit 1)

LIBS="$LIBS -ldl"

dnl #######################################################################
dnl # Doxygen Stuff
dnl #######################################################################
AC_CHECK_PROG(DOXYGEN, doxygen, true, false)
if test $DOXYGEN = false; then
   AC_MSG_WARN([*** doxygen not found, docs will not be available])
   enable_doxygen=no
else
   enable_doxygen=yes
   AC_CHECK_PROG(DOT, dot, true, false)
   if test $DOT = false; then
      enable_dot=no
      AC_MSG_WARN([*** dot not found, graphs will not be available])
   else
      enable_dot=yes
   fi
fi

AC_SUBST(enable_doxygen)
AC_SUBST(enable_dot)

AC_CONFIG_SUBDIRS([contrib])

if test x$ac_cv_use_libevent != xno; then
   AC_CONFIG_SUBDIRS([external/libevent])
fi   

dnl if test x$ac_cv_use_zookeeper != xno; then
dnl   AC_CONFIG_SUBDIRS([external/zookeeper_c])
dnl fi   

if test x$ac_cv_use_thrift = xyes; then
   AC_CONFIG_SUBDIRS([external/thrift])

dnl hack solution
    thrift_cppflags="CPPFLAGS=-I$FX_LIBEVENT_HOME -I$FX_LIBEVENT_HOME/include -I$FX_LIBEVENT_HOME/include/event2 -I$FX_BOOST_HOME -I/usr/local"
   if test x$ac_cv_use_debug != xno; then
      thrift_cppflags="$thrift_cppflags -g -D_DEBUG"
   fi
   ac_configure_args="$ac_configure_args --enable-tests=no --with-boost=$FX_BOOST_HOME '$thrift_cppflags' 'LDFLAGS=-L$FX_LIBEVENT_HOME/.libs' 'LD_LIBRARY_PATH=$FX_LIBEVENT_HOME/.libs'"
fi  

AC_CONFIG_FILES([Makefile                
		include/Makefile
		src/common/Makefile
		src/utility/Makefile
		src/thread/Makefile
		src/external/Makefile
		src/external/zlib/Makefile
		src/external/iconv/Makefile
		src/external/iconv/configmake.h
		src/config/Makefile
		src/document/Makefile
		src/analyzer/Makefile
		src/store/Makefile
		src/index/Makefile
		src/queryparser/Makefile
		src/search/Makefile
                src/Makefile
                extension/src/processor/Makefile
                extension/src/collection/Makefile
                extension/src/com/Makefile
                extension/src/network/Makefile
                extension/src/plugin/Makefile
                extension/src/util/Makefile
                extension/Makefile
                extension/src/Makefile
                extension/include/Makefile
                extension/win32/Makefile
                external/Makefile
                app/Makefile
	        app/common/Makefile
	        app/fxist4c/Makefile
	        app/fbuilder/Makefile
	        app/fsearchd/Makefile
	        app/thrift/Makefile
	        app/lexibuilder/Makefile
	        app/fproxy/Makefile
	        app/fbench/Makefile
	        app/watchdog/Makefile
		bindings/Makefile
		bindings/php/Makefile
		bindings/php/http/Makefile
		bindings/php/rpc/Makefile
		bindings/python/Makefile
		bindings/python/rpc/Makefile
		plugins/plugins/Makefile
		plugins/PDFParserPlugin/Makefile
		plugins/MSOfficeParserPlugin/Makefile
		plugins/Makefile
	        win32/Makefile
	        test/Makefile
	        test/cppunit/Makefile
	        test/src/Makefile
	        test/src/document/Makefile
	        test/src/analyzer/Makefile
	        test/src/index/Makefile
	        test/src/queryparser/Makefile
	        test/src/search/Makefile
	        test/src/common/Makefile
	        test/src/thread/Makefile
	        test/src/utility/Makefile
	        test/src/config/Makefile
	        test/src/store/Makefile
	        test/extension/Makefile
	        test/extension/processor/Makefile
	        test/extension/network/Makefile
	        test/extension/util/Makefile
	        test/extension/collection/Makefile
	        test/app/Makefile
	        test/app/common/Makefile
	        test/app/fproxy/Makefile
	        test/app/watchdog/Makefile
	        bin/Makefile		        
	        sysdata/Makefile
	        conf/Makefile
	        documentation/Makefile
	        documentation/api_doc/Makefile
		documentation/docbook/Makefile
		documentation/fop/Makefile
		documentation/src/Makefile
		test/testdata/conf/test_config_posix.xml
		test/src/test.h
		])

AC_OUTPUT
