lib_LTLIBRARIES = libmatrixops.la
libmatrixops_la_LDFLAGS = -version-info 1:0:0

bin_PROGRAMS = matrix_ops_runner
matrix_ops_runner_SOURCES = matrix_ops_runner.c
matrix_ops_runner_LDADD = libmatrixops.la

include_HEADERS = matrixops.h

matrix_ops_runner.c: matrixops.h
libmatrixops.c: matrixops.h

AM_CFLAGS=-D'LOCALE_PATH="$(localedir)"'