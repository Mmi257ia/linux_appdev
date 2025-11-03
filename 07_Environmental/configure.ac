#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
# $ autoreconf -vi

AC_PREREQ([2.72])
AC_INIT([rhasher], [1.0], [sportloto@mail.ru])
AM_INIT_AUTOMAKE([foreign subdir-objects])

AC_CONFIG_SRCDIR([src/rhasher.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for ./configure flags.
DEFAULT_WITH_READLINE_VALUE=dflt_yes
AC_ARG_WITH([readline],
            [AS_HELP_STRING([--with-readline], [use libreadline to read input])],
            [],
            [with_readline=$DEFAULT_WITH_READLINE_VALUE])

# Checks for libraries.
AC_CHECK_LIB([rhash], [rhash_library_init], [], [AC_MSG_ERROR([librhash is required])])

AS_IF([test "$with_readline" != no],  # if default or explicitly stated "with readline"...
      AC_CHECK_LIB([readline], [readline], [],  # check for libreadline
                   [AS_IF([test "$with_readline" = "$DEFAULT_WITH_READLINE_VALUE"],  # if failed and value is default
                          [],  # just skip it, source code will fall back to getline()
                          # but if the value was explicitly "yes" -- complain
                          [AC_MSG_FAILURE([readline test failed (use --without-readline to disable)])])],
                   [])
)

# Checks for header files.
AC_CHECK_HEADER([rhash.h])
AC_CHECK_HEADER([readline/readline.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([strerror])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
