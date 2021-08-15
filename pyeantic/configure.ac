dnl Process this file with autoconf to produce a configure script.

dnl We need at least autoconf 2.69 for this configure.ac to work.
AC_PREREQ([2.69])
AC_INIT([pyeantic], [1.0.3], [julian.rueth@fsfe.org])

AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.14.1 tar-pax foreign])

dnl Set INSTALL or use install-sh.
AC_PROG_INSTALL
dnl We build our library with libtool.
LT_INIT

AC_CANONICAL_HOST

dnl We run the valgrind test-suite in make check-valgrind (if valgrind is installed)
AX_VALGRIND_DFLT([sgcheck], [off])
AX_VALGRIND_DFLT([drd], [off])
AX_VALGRIND_DFLT([helgrind], [off])
AX_VALGRIND_CHECK()

dnl We want to build a Python module powered by cppyy
AM_PATH_PYTHON([3.6], [have_python=yes], [have_python=no])

dnl Our Python library relies on cppyy. It can be built without since it is a
dnl pure Python package but we cannot test it if cppyy is not present.
AC_ARG_WITH([pytest], AS_HELP_STRING([--without-pytest], [Do not run Python tests]))
AS_IF([test "x$with_pytest" != "xno" && test "x$have_python" = "xyes"],
      [
       with_pytest=yes
       AC_MSG_CHECKING([for pytest module])
       AS_IF([$PYTHON -c 'import pytest'], , AC_MSG_ERROR([pytest for make check not found; run --without-pytest to disable Python tests in make check]))
       AC_MSG_RESULT([yes])
       AC_MSG_CHECKING([for cppyy])
       AS_IF([$PYTHON -c 'import cppyy'], , AC_MSG_ERROR([cppyy for make check not found; run --without-pytest to disable Python tests in make check]))
       AC_MSG_RESULT([yes])
       AC_MSG_CHECKING([for cppyythonizations])
       AS_IF([$PYTHON -c 'import cppyythonizations'], , AC_MSG_ERROR([cppyythonizations for make check not found; run --without-pytest to disable Python tests in make check]))
       AC_MSG_RESULT([yes])
      ], [])
AM_CONDITIONAL([HAVE_PYTEST], [test "x$with_pytest" = "xyes"])

dnl Some bits require SageMath to be installed. We can build without it since it is a pure Python package but we cannot test it then.
AC_ARG_WITH([sage], AS_HELP_STRING([--without-sage], [Do not run SageMath tests]))
AS_IF([test "x$with_sage" != "xno" && test "x$with_pytest" = "xyes"],
      [
       with_sage=yes
       AC_MSG_CHECKING([for sage module])
       AS_IF([$PYTHON -c 'import sage'], , AC_MSG_ERROR([SageMath for make check not found; run --without-sage to disable SageMath tests in make check]))
       AC_MSG_RESULT([yes])
      ], [])
AM_CONDITIONAL([HAVE_SAGE], [test "x$with_sage" = "xyes"])

dnl We do not need realalg to build a converter for realalg but we need it to test.
AC_ARG_WITH([realalg], AS_HELP_STRING([--without-realalg], [Do not run realalg tests]))
AS_IF([test "x$with_realalg" != "xno" && test "$x$with_pytest" = "$xyes"],
      [
       with_realalg=yes
       AC_MSG_CHECKING([for realalg module])
       AS_IF([$PYTHON -c 'import realalg'], , AC_MSG_ERROR([realalg for make check not found; run --without-realalg to disable realalg tests in make check]))
       AC_MSG_RESULT([yes])
      ], [])
AM_CONDITIONAL([HAVE_REALALG], [test "x$with_realalg" = "xyes"])

AC_CONFIG_FILES([Makefile src/Makefile test/Makefile])

AC_OUTPUT
