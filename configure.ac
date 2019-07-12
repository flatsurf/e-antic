#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([e-antic], [1.0.0], [vincent.delecroix@math.cnrs.fr])

dnl Version number of the library c:r:a.
dnl Set to c+1:0:0 on a non-backwards-compatible change.
dnl Set to c+1:0:a+1 on a backwards-compatible change.
dnl Set to c:r+1:a if the interface is unchanged.
AC_SUBST([libeantic_version_info], [1:0:0])
AC_SUBST([libeanticxx_version_info], [1:0:0])

AM_INIT_AUTOMAKE([-Wall -Werror])
AM_SILENT_RULES([yes]) # non-verbose make; override with ./configure --disable-silent-rules
AM_PROG_AR
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_LIBTOOL
AC_PROG_CXX

## Test for valgrind test suite
AX_VALGRIND_DFLT([sgcheck], [off])
AX_VALGRIND_DFLT([drd], [off])
AX_VALGRIND_DFLT([helgrind], [off])
AX_VALGRIND_CHECK

## Flint development
AC_ARG_ENABLE([flint-devel],
[  --enable-flint-devel    Compile with flint development version],
[case "${enableval}" in
  yes) flintdevel=true ;;
  no)  flintdevel=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-flint-devel]) ;;
esac],[flintdevel=false])
AM_CONDITIONAL([FLINT252], [test x$flintdevel = xfalse])

dnl e-antic is thread-safe when pthread is available.
AC_ARG_WITH([pthread], AS_HELP_STRING([--without-pythread], [Do not ensure thread-safety (with pthread)]))
AS_IF([test "x$with_pthread" != "xno" && test "x$have_pthread" = "xyes"],
      [
       with_pthread=yes
       AC_CHECK_LIB(pthread, pthread_mutex_init, [], AC_MSG_ERROR([pthread for thread-safety not found; run --without-pthread to disable thread-safety]))
      ], [])
AM_CONDITIONAL([HAVE_PTHREAD], [test "x$with_pthread" = "xyes"])

## Check for headers and libraries
AC_CHECK_HEADER(gmp.h, , [AC_MSG_ERROR([gmp header not found])])
AC_CHECK_LIB(gmp, __gmpz_add, [], [AC_MSG_ERROR([libgmp not found])])

AC_CHECK_HEADERS(flint/flint.h flint/fmpz.h flint/fmpq.h, , [AC_MSG_ERROR([flint header not found])])
AC_CHECK_LIB(flint, fmpz_init, [], [AC_MSG_ERROR([libflint not found])], [-lmpfr])

AC_CHECK_HEADERS(arb.h arf.h, , [AC_MSG_ERROR([arb header not found])])
AC_SEARCH_LIBS(arb_init, [arb flint-arb],[], [AC_MSG_ERROR([libarb not found])], [-lmpfr])

AC_LANG_PUSH([C++])
AX_CXX_COMPILE_STDCXX(17)
AC_CHECK_HEADER(gmpxx.h, , [AC_MSG_ERROR([gmpxx header not found (gmp needs to be compiled with c++ support (--enable-cxx))])])
AC_CHECK_HEADERS(boost/lexical_cast.hpp, , [AC_MSG_ERROR([boost headers not found])])
AC_LANG_POP([C++])

AC_OUTPUT
