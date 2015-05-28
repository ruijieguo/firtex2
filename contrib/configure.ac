AC_INIT([firtex-contrib], [0.5.1])
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE(firtex-contrib, [0.5.1])
AM_INIT_AUTOMAKE([foreign subdir-objects])

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_LANG_CPLUSPLUS
AC_PROG_CXX
AC_PROG_CC
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AM_PROG_CC_C_O

dnl Set debug compilation flags
AC_ARG_WITH([debug], 
	AC_HELP_STRING([--with-debug], [compile debug version (default is disabled)]),
	[ac_cv_use_debug=$withval], [ac_cv_use_debug=no])

AC_MSG_CHECKING([whether to compile debug version])
		
if test x$ac_cv_use_debug != xno; then
   COMPILER_OPTIONS="$COMPILER_OPTIONS -Wall -g -D_DEBUG -D_FILE_OFFSET_BITS=64 -fno-strict-aliasing -msse2"
   REQUIRED_OPTIONS="-D_DEBUG -D_FILE_OFFSET_BITS=64"
   AC_MSG_RESULT([yes])
else
   COMPILER_OPTIONS="$COMPILER_OPTIONS -Wall -g -D_FILE_OFFSET_BITS=64 -O3 -DNDEBUG -fno-strict-aliasing -msse2"
   REQUIRED_OPTIONS="-DNDEBUG -D_FILE_OFFSET_BITS=64"
   AC_MSG_RESULT([no])
fi

dnl Set profile compilation flags
AC_ARG_WITH([profile], 
	AC_HELP_STRING([--with-profile], [compile profile version (default is disabled)]),
	[ac_cv_use_profile=$withval], [ac_cv_use_profile=no])

AC_MSG_CHECKING([whether to compile profile version])
		
if test x$ac_cv_use_profile != xno; then
   COMPILER_OPTIONS="$COMPILER_OPTIONS -pg"
   AC_MSG_RESULT([yes])
else
   AC_MSG_RESULT([no])
fi

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
AC_CHECK_LIB(dl, dlopen, [LIBS="$LIBS -ldl"], exit 1)

dnl Check for pthread
AC_CHECK_LIB(pthread, pthread_create, [LIBS="$LIBS -lpthread"], exit 1)

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

AC_CONFIG_FILES([Makefile
                 highlighter/Makefile
                 highlighter/include/Makefile
                 highlighter/src/Makefile
                 snowball/Makefile
                 snowball/include/Makefile
                 snowball/src/Makefile
                 win32/Makefile])
AC_OUTPUT
