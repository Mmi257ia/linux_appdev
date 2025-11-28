#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.72])
AC_INIT([Growable memory buffers for C], [1.0.0], [sportloto@mail.ru])
AM_INIT_AUTOMAKE([foreign subdir-objects])
LT_INIT([])

# AC_CONFIG_SRCDIR([tests.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gettimeofday strtoull])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
