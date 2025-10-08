GDB = /usr/bin/gdb
GDBFLAGS = --batch --quiet
CFLAGS = -O0 -g
TESTS = 1 2
PROG = prog
TRASH = *.o
TEST_MARKER = "@@@"

.PHONY: all clean test

all: $(PROG)

test: $(addprefix test, $(TESTS))

test%: test%.gdb reference%.txt $(PROG)
	$(GDB) $(GDBFLAGS) -x $< $(PROG) | grep $(TEST_MARKER) | cmp $(word 2, $^)

clean:
	rm -f $(TRASH) $(PROG)
