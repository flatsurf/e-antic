#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([e-antic], [0.1], [vincent.delecroix@math.cnrs.fr])
AM_INIT_AUTOMAKE([-Wall -Werror])
AM_SILENT_RULES([yes]) # non-verbose make; override with ./configure --disable-silent-rules
AM_PROG_AR
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_LIBTOOL
AC_PROG_CXX

## Test for OpenMP parallelization.
AC_ARG_ENABLE([openmp],
    [AS_HELP_STRING([--enable-openmp@<:@=ARG@:>@],
      [enable parallelization with OpenMP @<:@default=check@:>@])],
    [:],
    [enable_openmp=check])
AS_IF([test "x$enable_openmp" != xno],
  [AX_OPENMP([have_openmp=yes],
            [have_openmp=no])])
AS_IF([test x$have_openmp = xyes],
  [enable_openmp=yes],
  [AS_IF([test "x$enable_openmp" = xyes],
    [AC_MSG_ERROR([openmp not available])],
    [AS_IF([test "x$enable_openmp" != xno],
        [AC_MSG_NOTICE([Parallelization with OpenMP will not be available because no suitable compiler was found])])
     enable_openmp=no])])
AC_SUBST(OPENMP_CFLAGS)
AM_CONDITIONAL(ENABLE_OPENMP, [test x$enable_openmp = xyes])

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

## Check for headers and libraries
AC_CHECK_HEADER(gmp.h, , [AC_MSG_ERROR([gmp header not found])])
AC_CHECK_LIB(gmp, __gmpz_add, [], [AC_MSG_ERROR([libgmp not found])])

AC_CHECK_HEADERS(flint/flint.h flint/fmpz.h flint/fmpq.h, , [AC_MSG_ERROR([flint header not found])])
AC_CHECK_LIB(flint, fmpz_init, [], [AC_MSG_ERROR([libflint not found])])

AC_CHECK_HEADERS(arb.h arf.h, , [AC_MSG_ERROR([arb header not found])])
AC_CHECK_LIB(arb, arb_init, [], [AC_MSG_ERROR([libarb not found])])

AC_LANG_PUSH([C++])
AX_CXX_COMPILE_STDCXX(11)
AC_CHECK_HEADER(gmpxx.h, , [AC_MSG_ERROR([gmpxx header not found (gmp needs to be compiled with c++ support (--enable-cxx))])])
AC_LANG_POP([C++])

AC_OUTPUT
