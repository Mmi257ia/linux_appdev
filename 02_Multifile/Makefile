GENERATES = prog README libout.a
TRASH = *.o *~ o.*
CFLAGS = -Wall

all:    README prog

.SECONDARY:	fun.o const.o
.INTERMEDIATE:	libout.a(fun.o const.o)

libout.a:	libout.a(fun.o const.o)

prog:	prog.o libout.a

README: prog
	./$< 2> $@

fun.o prog.o:  outlib.h

clean:
	rm -f $(TRASH)

distclean:      clean
	rm -rf $(GENERATES)
