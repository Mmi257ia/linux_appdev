bin_PROGRAMS = matrix_ops_runner
matrix_ops_runner_SOURCES = matrix_ops_runner.c matrixops.c

matrix_ops_runner.c: matrixops.h
matrixops.c: matrixops.h

AM_CFLAGS=-D'LOCALE_PATH="$(localedir)"'