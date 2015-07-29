AC_PREREQ([2.69])
AC_INIT([firtex-contrib],[0.5.1])
#AM_INIT_AUTOMAKE([subdir-objects])

AC_CANONICAL_HOST
AC_CANONICAL_BUILD
AC_CANONICAL_TARGET
AC_DISABLE_STATIC

AC_LANG([C++])

AC_CONFIG_MACRO_DIR([m4])

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

# Default to gcc toolchain (we rely on some atomic builtins for now,
# that are also present with clang).
AC_PROG_CXX([g++])
AC_PROG_CC([gcc])

# Check if we should enable debugging, optimization. Note we only
# update CFLAGS and CXXFLAGS if none are provided.
AM_CONDITIONAL([DEBUG], [test x"$enable_debug" = "xyes"])
AM_CONDITIONAL([OPTIMIZE], [test x"$enable_optimize" = "xyes"])


debug_flags="-g1"
if test "x$enable_debug" = "xyes"; then
  debug_flags="-g"
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
