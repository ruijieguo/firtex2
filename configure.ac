sAC_PREREQ([2.69])
AC_INIT([firtex],[0.5.1])

AC_CANONICAL_HOST
AC_CANONICAL_BUILD
AC_CANONICAL_TARGET
AC_DISABLE_STATIC

AC_LANG([C++])

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_HEADERS(include/firtex/config.h)

# Initialize automake.
# -Wno-portability, since we require GNU Make for % patterns
# Removal of -Werror due warnings introduced by automake 1.14.
AM_INIT_AUTOMAKE([-Wall -Wno-portability foreign])

# This is required for linking non-POSIX libs.
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

LT_PREREQ([2.2])
LT_INIT
LT_LANG([C++])
LT_OUTPUT

AC_PROG_YACC
AC_PROG_LEX
AM_PROG_LEX

dnl Checks for library functions.
AC_FUNC_VPRINTF

CONFIGURE_ARGS="$ac_configure_args"
AC_SUBST(CONFIGURE_ARGS)


AC_ARG_ENABLE([debug],
              AS_HELP_STRING([--enable-debug],
                             [enable debugging. If CFLAGS/CXXFLAGS are set, this
                              option won't change them default: no]),
              [enable_debug=yes], [])

AC_ARG_ENABLE([optimize],
              AS_HELP_STRING([--enable-optimize],
                             [enable optimizations. If CFLAGS/CXXFLAGS are set,
                              this option won't change them default: no]),
              [enable_optimize=yes], [])

AC_ARG_ENABLE([bundled],
              AS_HELP_STRING([--disable-bundled],
                             [build against preinstalled dependencies instead
                              of bundled libraries]),
              [enable_bundled=no], [enable_bundled=yes])

AC_ARG_WITH([zookeeper],
            AS_HELP_STRING([--with-zookeeper@<:@=DIR@:>@],
                           [excludes building and using the bundled ZooKeeper
                            package in lieu of an installed version at a
                            location prefixed by the given path]),
            [without_bundled_zookeeper=yes], [])

AC_ARG_WITH([libevent],
            AS_HELP_STRING([--with-libevent@<:@=DIR@:>@],
                           [excludes building and using the bundled libevent
                            package in lieu of an installed version at a
                            location prefixed by the given path]),
            [without_bundled_libevent=yes], [])

AC_ARG_WITH([boost],
            AS_HELP_STRING([--with-boost@<:@=DIR@:>@],
                           [excludes building and using the bundled Boost
                           package in lieu of an installed version at a
                           location prefixed by the given path]),
            [without_bundled_boost=yes], [])

AC_ARG_WITH([thrift],
            AS_HELP_STRING([--with-thrift@<:@=DIR@:>@],
                           [excludes building and using the bundled Thrift
                           package in lieu of an installed version at a
                           location prefixed by the given path]),
            [without_bundled_thrift=yes], [])

AC_ARG_WITH([pcre2],
            AS_HELP_STRING([--with-pcre2@<:@=DIR@:>@],
                           [excludes building and using the bundled pcre2
                           package in lieu of an installed version at a
                           location prefixed by the given path]),
            [without_bundled_pcre2=yes], [])

AC_ARG_WITH([profile], 
	AS_HELP_STRING([--with-profile],[compile profile version (default is disabled)]),
	[], [])

AC_ARG_WITH([tcmalloc], 
	AS_HELP_STRING([--with-tcmalloc],[compile tcmalloc version (default is disabled)]),
	[], [])


AC_MSG_NOTICE([Setting up build environment for ${target_cpu} ${target_os}])
# Determine the current OS (TODO(benh): Does autotools do this for us?).
case "${target_os}" in
  linux*)
    OS_NAME=linux
    LIBS="$LIBS -lrt -ldl"
    ;;
  darwin*)
    OS_NAME=darwin
    ;;
  *)
    AC_MSG_ERROR("FirteX2 is currently unsupported on your platform.")
esac

# Used for conditionally including source files.
AM_CONDITIONAL([OS_LINUX], [test "x$OS_NAME" = "xlinux"])


# Default to gcc toolchain (we rely on some atomic builtins for now,
# that are also present with clang).
AC_PROG_CXX([g++])
AC_PROG_CC([gcc])

AX_COMPILER_VERSION
AX_COMPILER_VENDOR

AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])

# Check if we should enable debugging, optimization. Note we only
# update CFLAGS and CXXFLAGS if none are provided.
AM_CONDITIONAL([DEBUG], [test x"$enable_debug" = "xyes"])
AM_CONDITIONAL([OPTIMIZE], [test x"$enable_optimize" = "xyes"])


debug_flags="-g1"
if test "x$enable_debug" = "xyes"; then
  debug_flags="-g -D_DEBUG"
elif test "x$enable_optimize" = "xyes"; then
  debug_flags=""
fi

if test "x$enable_optimize" = "xyes"; then
  optimize_flags="-O2"
else
  optimize_flags="-O0"
fi

AS_IF([test "x${ac_cv_env_CFLAGS_set}" = "x"],
      [CFLAGS="$debug_flags $optimize_flags"])
AS_IF([test "x${ac_cv_env_CXXFLAGS_set}" = "x"],
      [CXXFLAGS="$CXXFLAGS $debug_flags $optimize_flags"])

# Check zookeeper
if test -n "`echo $with_zookeeper`"; then
  CPPFLAGS="$CPPFLAGS -I${with_zookeeper}/include/zookeeper"
  LDFLAGS="$LDFLAGS -L${with_zookeeper}/lib"
elif test "x$enable_bundled" = "xno"; then
  CPPFLAGS="$CPPFLAGS -I/usr/include/zookeeper"
fi

if test "x$without_bundled_zookeeper" = "xyes" || \
   test "x$enable_bundled" != "xyes"; then
  # Check if headers and library were located.
  AC_CHECK_HEADERS([zookeeper.h],
                   [AC_CHECK_LIB([zookeeper_mt],
                                 [zookeeper_init],
                                 [found_zookeeper=yes])])

  if test "x$found_zookeeper" = "xyes"; then
    with_bundled_zookeeper=no
  else
    AC_MSG_ERROR([cannot find ZooKeeper
-------------------------------------------------------------------
You have requested the use of a non-bundled ZooKeeper but no suitable
ZooKeeper could be found.

You may want specify the location of ZooKeeper by providing a prefix
path via --with-zookeeper=DIR, or check that the path you provided is
correct if you're already doing this
-------------------------------------------------------------------
])
  fi
else
  with_bundled_zookeeper=yes
fi

AM_CONDITIONAL([WITH_BUNDLED_ZOOKEEPER],
               [test "x$with_bundled_zookeeper" = "xyes"])

# Check boost
if test -n "`echo $with_boost`"; then
  CPPFLAGS="$CPPFLAGS -I${with_boost}/include"
fi

if test "x$without_bundled_boost" = "xyes" || \
   test "x$enable_bundled" != "xyes"; then
  # Check if headers and library were located.
  AC_CHECK_HEADERS([boost/version.hpp], [found_boost=yes])

  if test "x$found_boost" = "xyes"; then
    with_bundled_boost=no
  else
    AC_MSG_ERROR([cannot find Boost
-------------------------------------------------------------------
You have requested the use of a non-bundled Boost but no suitable
Boost could be found.

You may want specify the location of Boost by providing a prefix
path via --with-boost=DIR, or check that the path you provided is
correct if you're already doing this.
-------------------------------------------------------------------
])
  fi
else
  with_bundled_boost=yes
fi

AM_CONDITIONAL([WITH_BUNDLED_BOOST], [test "x$with_bundled_boost" = "xyes"])


if test "x$without_bundled_libevent" = "xyes" || \
   test "x$enable_bundled" != "xyes"; then
   AC_CHECK_HEADERS([event2/event.h],
		    [AC_CHECK_LIB([event],
                                  [event_base_new],
                                  [found_libevent=yes])])
   
  if test "x$found_libevent" = "xyes"; then
    with_bundled_libevent=no
  else
    AC_MSG_ERROR([cannot find Libevent
-------------------------------------------------------------------
You have requested the use of a non-bundled Libevent but no suitable
Libevent could be found.

You may want specify the location of Libevent by providing a prefix
path via --with-libevent=DIR, or check that the path you provided is
correct if you're already doing this
-------------------------------------------------------------------
])
  fi
else
  with_bundled_libevent=yes
fi

AM_CONDITIONAL([WITH_BUNDLED_LIBEVENT],
               [test "x$with_bundled_libevent" = "xyes"])


# Check thrift
if test -n "`echo $with_thrift`"; then
  CPPFLAGS="-I${with_thrift}/include $CPPFLAGS"
  LDFLAGS="-L${with_thrift}/lib $LDFLAGS"
fi

if test "x$without_bundled_thrift" = "xyes" || \
   test "x$enable_bundled" != "xyes"; then
   AC_CHECK_HEADERS([thrift/Thrift.h],
		    [AC_CHECK_LIB([thrift],
                                  [exit],
                                  [found_thrift=yes])])
   
  if test "x$found_thrift" = "xyes"; then
    with_bundled_thrift=no
  else
    AC_MSG_ERROR([cannot find Thrift
-------------------------------------------------------------------
You have requested the use of a non-bundled Thrift but no suitable
Thrift could be found.

You may want specify the location of Thrift by providing a prefix
path via --with-thrift=DIR, or check that the path you provided is
correct if you're already doing this
-------------------------------------------------------------------
])
  fi
else
  with_bundled_thrift=yes
fi

AM_CONDITIONAL([WITH_BUNDLED_THRIFT],
               [test "x$with_bundled_thrift" = "xyes"]) 

# Check pcre2
if test -n "`echo $with_pcre2`"; then
  CPPFLAGS="-I${with_pcre2}/include $CPPFLAGS"
  LDFLAGS="-L${with_pcre2}/lib $LDFLAGS"
fi

if test "x$without_bundled_pcre2" = "xyes" || \
   test "x$enable_bundled" != "xyes"; then
   AC_CHECK_HEADERS([pcre2.h],
		    [AC_CHECK_LIB([pcre2],
                                  [exit],
                                  [found_pcre2=yes])])
   
  if test "x$found_pcre2" = "xyes"; then
    with_bundled_pcre2=no
  else
    AC_MSG_ERROR([cannot find Pcre2
-------------------------------------------------------------------
You have requested the use of a non-bundled Pcre2 but no suitable
Pcre2 could be found.

You may want specify the location of pcre2 by providing a prefix
path via --with-pcre2=DIR, or check that the path you provided is
correct if you're already doing this
-------------------------------------------------------------------
])
  fi
else
  with_bundled_pcre2=yes
fi

AM_CONDITIONAL([WITH_BUNDLED_PCRE2],
               [test "x$with_bundled_pcre2" = "xyes"]) 

       

# Check profile
AC_MSG_CHECKING([whether to compile profile version])
		
if test -n "`echo $with_profile`"; then
   CPPFLAGS="$CPPFLAGS -pg"
   AC_MSG_RESULT([yes])
else
   AC_MSG_RESULT([no])
fi

# Check tcmalloc
AC_MSG_CHECKING([whether to compile with tcmalloc for heap checking])
		
if test -n "`echo $with_tcmalloc`"; then
   LDFLAGS="$LDFLAGS -L${with_tcmalloc}/lib"
   with_tcmalloc=yes
   AC_MSG_RESULT([yes])
else
   with_tcmalloc=no
   AC_MSG_RESULT([no])
fi

AM_CONDITIONAL([WITH_TCMALLOC], [test "x$with_tcmalloc" = "xyes"]) 

CXXFLAGS="$CXXFLAGS -D_FILE_OFFSET_BITS=64 -fno-strict-aliasing -msse2"

ac_configure_args="$ac_configure_args CXXFLAGS='$CXXFLAGS'"
CONFIGURE_ARGS="$CONFIGURE_ARGS $ac_configure_args"

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


ac_configure_args_pre="$ac_configure_args"
ac_configure_args_post="$ac_configure_args --enable-shared=no --with-pic"
ac_configure_args="$ac_configure_args_post"

AC_CONFIG_COMMANDS_PRE([ac_configure_args="$ac_configure_args_pre"])
AC_CONFIG_COMMANDS_POST([ac_configure_args="$ac_configure_args_post"])

AC_CONFIG_SUBDIRS([contrib])

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
	        test/app/searcher/Makefile
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
