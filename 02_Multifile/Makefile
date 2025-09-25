TESTS_DIR = tests
GENERATES = prog prog-a prog-so README liboutput.so *.o *~ o.* *.a $(TESTS_DIR)
CFLAGS = -Wall -fPIC

.PHONY: all clean test

all:    prog prog-a prog-so README test

prog:	prog.o fun.o const.o

prog-a:	prog.o liboutput_static.a
	$(CC) $^ -o $@

prog-so:	prog.o liboutput.so
	$(CC) -L. $^ -o $@

# This leads to rearchivation the library on every "make" invoke for me =(
# liboutput_static.a:	liboutput_static.a(fun.o const.o)

liboutput_static.a:	fun.o const.o
	$(AR) -rcs $@ $^

liboutput.so:	fun.o const.o
	$(CC) -shared $^ -o $@

fun.o prog.o:  outlib.h

README: prog
	./$< 2> $@

test:	$(TESTS_DIR) $(TESTS_DIR)/noparam.check $(TESTS_DIR)/oneparam.check $(TESTS_DIR)/threeparams.check

$(TESTS_DIR):
	mkdir -p $(TESTS_DIR)

%.check: %.test
	cmp $< $<-a
	cmp $< $<-so

$(TESTS_DIR)/noparam.test:	prog prog-a prog-so
	./prog > $@ 2>&1
	./prog-a > $@-a 2>&1
	LD_LIBRARY_PATH=. ./prog-so > $@-so 2>&1

$(TESTS_DIR)/oneparam.test:	prog prog-a prog-so
	./prog one-param-text > $@ 2>&1
	./prog-a one-param-text > $@-a 2>&1
	LD_LIBRARY_PATH=. ./prog-so one-param-text > $@-so 2>&1

$(TESTS_DIR)/threeparams.test:	prog prog-a prog-so
	./prog three params text > $@ 2>&1
	./prog-a three params text > $@-a 2>&1
	LD_LIBRARY_PATH=. ./prog-so three params text > $@-so 2>&1

clean:
	rm -rf $(GENERATES)
