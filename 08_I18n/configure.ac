#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.72])
AC_INIT([Guesser], [1.0], [sportloto@mail.ru])
AM_INIT_AUTOMAKE([foreign subdir-objects])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_REQUIRE_VERSION(0.23)

AC_CONFIG_SRCDIR([src/guesser.c])
AC_CONFIG_HEADERS([config.h])

ALL_LINGUAS="ru"

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([libintl.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([setlocale])

AC_CONFIG_FILES([Makefile po/Makefile.in src/Makefile])
AC_OUTPUT
