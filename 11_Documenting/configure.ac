#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.72])
AC_INIT([Guesser], [1.0], [sportloto@mail.ru], [guesser])
AM_INIT_AUTOMAKE([foreign subdir-objects])
DX_INIT_DOXYGEN([guesser])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_REQUIRE_VERSION(0.23)

AC_CONFIG_SRCDIR([src/guesser.c])
AC_CONFIG_HEADERS([config.h])

ALL_LINGUAS="ru"

# Checks for programs.
AC_PROG_CC
AC_CHECK_PROG(HELP2MAN_CHECK, help2man, yes)
AS_IF([test x"$HELP2MAN_CHECK" != x"yes"],
      [AC_MSG_ERROR([help2man not found, please install it.])])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([libintl.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([setlocale])

AC_CONFIG_FILES([Makefile po/Makefile.in src/Makefile Doxyfile])
AC_OUTPUT
