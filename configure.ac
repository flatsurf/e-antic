#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT(e-antic, 0.1)
AM_INIT_AUTOMAKE(foreign)
AM_SILENT_RULES([yes]) # non-verbose make; override with ./configure --disable-silent-rules
AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_LIBTOOL

AC_ARG_WITH([flint],
   AS_HELP_STRING([--with-flint=DIR],
      [provide location of FLINT (with arb, antic) installation prefix]),
   [ LDFLAGS="-L$with_flint/lib $LDFLAGS"
     CPPFLAGS="-I $with_flint/include $CPPFLAGS"
   ])

AC_OUTPUT
