#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([e-antic], [0.1], [vincent.delecroix@math.cnrs.fr])
AM_INIT_AUTOMAKE([-Wall -Werror])
AM_SILENT_RULES([yes]) # non-verbose make; override with ./configure --disable-silent-rules
AM_PROG_AR
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile e-antic/test/Makefile])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_LIBTOOL
AC_PROG_CXX

AC_CHECK_HEADER(gmp.h, , [AC_MSG_ERROR([gmp header not found])])
AC_CHECK_LIB(gmp, __gmpz_add, [], [AC_MSG_ERROR([libgmp not found])])

AC_CHECK_HEADERS(flint/flint.h flint/fmpz.h flint/fmpq.h, , [AC_MSG_ERROR([flint header not found])])
AC_CHECK_LIB(flint, fmpz_init, [], [AC_MSG_ERROR([libflint not found])])

AC_CHECK_HEADERS(arb.h arf.h, , [AC_MSG_ERROR([arb header not found])])
AC_CHECK_LIB(arb, arb_init, [], [AC_MSG_ERROR([libarb not found])])

AC_CHECK_HEADERS(antic/nf.h antic/nf_elem.h, , [AC_MSG_ERROR([antic header not found])])
AC_CHECK_LIB(antic, nf_init, [], [AC_MSG_ERROR([libantic not found])])

AC_LANG_PUSH([C++])
AC_CHECK_HEADER(gmpxx.h, , [AC_MSG_ERROR([gmpxx header not found (gmp needs to be compiled with c++ support (--enable-cxx))])])
AC_LANG_POP([C++])

AC_OUTPUT
