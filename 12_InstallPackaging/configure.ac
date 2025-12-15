#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.72])
AC_INIT([Matrix operations runner], [1.0.0], [sportloto@mail.ru], [matrix_ops_runner])
AM_INIT_AUTOMAKE([foreign subdir-objects])
LT_INIT([])
DX_INIT_DOXYGEN([guesser])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_REQUIRE_VERSION(0.23)

AC_CONFIG_SRCDIR([src/matrix_ops_runner.c])
AC_CONFIG_HEADERS([config.h])

ALL_LINGUAS="ru"

# Checks for programs.
AC_PROG_CC
AC_CHECK_PROG(HELP2MAN_CHECK, help2man, yes)
AS_IF([test x"$HELP2MAN_CHECK" != x"yes"],
      [AC_MSG_ERROR([help2man not found, please install it.])])

# Checks for libraries.
PKG_CHECK_MODULES([CHECK], [check])
AC_SUBST(CK_VERBOSITY, verbose) # default
AC_ARG_VAR(CK_VERBOSITY, [Default: "verbose". Can also be "silent", "minimal" or "normal"])

# Checks for header files.
AC_CHECK_HEADERS([libintl.h stdint.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([setlocale strcasecmp strerror])

AC_CONFIG_FILES([Makefile
                 po/Makefile.in
                 src/Makefile
                 tests/Makefile
                 Doxyfile])
AC_OUTPUT
