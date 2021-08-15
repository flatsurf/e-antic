dnl Process this file with autoconf to produce a configure script.

dnl We need at least autoconf 2.69 for this configure.ac to work.
AC_PREREQ([2.69])
AC_INIT([e-antic], [1.0.3], [vincent.delecroix@math.cnrs.fr])

AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.14.1 subdir-objects tar-pax foreign])

AC_CONFIG_FILES([Makefile])

AC_CONFIG_SUBDIRS([libeantic])

AC_ARG_WITH([pyeantic], AS_HELP_STRING([--without-pyeantic], [Do not build the Python interface pyeantic]))
AS_IF([test "x$with_pyeantic" != "xno"], [AC_CONFIG_SUBDIRS([pyeantic])])

AC_ARG_WITH([doc], AS_HELP_STRING([--without-doc], [Do not build the e-antic documentation]))
AS_IF([test "x$with_doc" != "xno"], [AC_CONFIG_SUBDIRS([doc])])

AC_OUTPUT
