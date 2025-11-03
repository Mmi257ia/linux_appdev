AM_CFLAGS = -Wall

bin_PROGRAMS=rhasher

rhasher_SOURCES=src/rhasher.c

# Use this to clean all the generates made by configure
clean-autotools-generates:
	rm -rf autom4te.cache src/.deps src/.dirstamp aclocal.m4 compile config.h config.h.in config.log config.status configure depcomp install-sh Makefile Makefile.in missing stamp-h1

clean-all: clean clean-autotools-generates