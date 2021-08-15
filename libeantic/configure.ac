dnl Process this file with autoconf to produce a configure script.

dnl We need at least autoconf 2.69 for this configure.ac to work.
AC_PREREQ([2.69])
AC_INIT([libeantic], [1.0.3], [vincent.delecroix@math.cnrs.fr])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([.])
AM_INIT_AUTOMAKE([1.14.1 subdir-objects tar-pax foreign])

dnl Version number of the library c:r:a.
dnl Set to c+1:0:0 on a non-backwards-compatible change.
dnl Set to c+1:0:a+1 on a backwards-compatible change.
dnl Set to c:r+1:a if the interface is unchanged.
AC_SUBST([libeantic_version_info], [1:2:0])
AC_SUBST([libeanticxx_version_info], [1:2:0])

AM_SILENT_RULES([yes]) # non-verbose make; override with ./configure --disable-silent-rules

AC_CANONICAL_HOST

dnl Find C++ Compiler
AC_PROG_CXX

dnl Set INSTALL or use install-sh.
AC_PROG_INSTALL

dnl We build our library with libtool.
LT_INIT

AC_CONFIG_LIBOBJ_DIR([upstream/patched])

## Test for valgrind test suite
AX_VALGRIND_DFLT([sgcheck], [off])
AX_VALGRIND_DFLT([drd], [off])
AX_VALGRIND_DFLT([helgrind], [off])
AX_VALGRIND_CHECK

## Check for headers and libraries
AC_CHECK_HEADER(gmp.h, , [AC_MSG_ERROR([GMP header not found])])
AC_CHECK_LIB(gmp, __gmpz_add, [], [AC_MSG_ERROR([libgmp not found])])

AC_CHECK_LIB(mpfr, mpfr_init, [], [AC_MSG_WARN([libmpfr not found which FLINT needs])])

AC_CHECK_HEADERS(flint/flint.h flint/fmpz.h flint/fmpq.h, , [AC_MSG_ERROR([FLINT headers not found])])
AC_CHECK_LIB(flint, fmpz_init, [], [AC_MSG_ERROR([libflint not found])])

AC_COMPILE_IFELSE([AC_LANG_PROGRAM([
#include <flint/flint.h>
#if __FLINT_RELEASE < 20600
#error FLINT must be at least 2.6.0
#endif
])], [], [AC_MSG_ERROR([libflint version must be at least 2.6.0])])

AC_CHECK_HEADERS(antic/nf.h antic/nf_elem.h, , [AC_MSG_ERROR([antic headers not found])])
AC_CHECK_LIB(antic, nf_init, [], [AC_MSG_ERROR([libantic not found])])

# Check if we need to provide our own nf_init_randtest which was introduced in https://github.com/wbhart/antic/pull/47
AC_REPLACE_FUNCS(nf_init_randtest)
# Unconditionally patch nf_elem_scalar_div_fmpq which has aliasing issues in ANTIC, https://github.com/wbhart/antic/pull/48
AC_LIBOBJ(EANTIC_nf_elem_scalar_div_fmpq)
# Unconditionally patch fmpq_poly_add_fmpq which was fixed in https://github.com/wbhart/flint2/commit/17d26d4c957828f3c98c15ddec85108f841d3438
AC_LIBOBJ(EANTIC_fmpq_poly_add_fmpq)
# Unconditionally patch nf_elem_add_fmpq so it uses our patched fmpq_poly_add_fmpq
AC_LIBOBJ(EANTIC_nf_elem_add_fmpq)

AC_CHECK_HEADERS(arb.h arf.h, , [AC_MSG_ERROR([Arb header not found])])
AC_SEARCH_LIBS(arb_init, [arb flint-arb],[], [AC_MSG_ERROR([libarb not found])])

AC_LANG_PUSH([C++])
AX_CXX_COMPILE_STDCXX(14)
AC_CHECK_HEADER(gmpxx.h, , [AC_MSG_ERROR([gmpxx header not found (GMP needs to be compiled with c++ support (--enable-cxx))])])
AC_CHECK_HEADERS(boost/lexical_cast.hpp, , [AC_MSG_ERROR([boost headers not found])])

dnl Our benchmarks use Google's C++ benchmark library.
dnl We fail if they cannot be found but let the user disable it explicitly.
AC_ARG_WITH([benchmark], AS_HELP_STRING([--without-benchmark], [Do not build C++ benchmarks that require google/benchmark]))
AS_IF([test "x$with_benchmark" != "xno"],
      [
       with_benchmark=yes
       AC_CHECK_HEADERS([benchmark/benchmark.h], , AC_MSG_ERROR([benchmark headers not found; run --without-benchmark to disable building of benchmark/]))
      ], [])
AM_CONDITIONAL([HAVE_BENCHMARK], [test "x$with_benchmark" = "xyes"])

dnl We test C/C++ code snippets with byexample which requires cppyy.
dnl We fail if byexample cannot be found but let the user disable it explicitly.
AC_ARG_WITH([byexample], AS_HELP_STRING([--without-byexample], [Do not test C/C++ code snippets with byexample]))
AS_IF([test "x$with_byexample" != "xno"],
      [
       with_byexample=yes
       AC_MSG_CHECKING([for byexample])
       AS_IF([byexample --version 2> /dev/null], , AC_MSG_ERROR([byexample for make check not found; run --without-byexample to disable byexample tests in make check]))
       AC_MSG_RESULT([yes])
      ], [])
AM_CONDITIONAL([HAVE_BYEXAMPLE], [test "x$with_byexample" = "xyes"])

AC_LANG_POP([C++])

AC_ARG_WITH([version-script], AS_HELP_STRING([--with-version-script], [Only export a curated list of symbols in the shared library.]))
AM_CONDITIONAL([HAVE_VERSION_SCRIPT], [test "x$with_version_script" = "xyes"])

AC_CONFIG_HEADERS([e-antic/config.h])
AC_CONFIG_FILES([Makefile src/Makefile srcxx/Makefile test/Makefile benchmark/Makefile test/byexample/Makefile])

AC_OUTPUT
