MOVE = move
PRESERVER = preserver
TESTS_DIR = tests
TESTS = $(shell ls $(TESTS_DIR))
TRASH = *.o

.PHONY: all clean test $(TESTS_DIR)/%

all: $(MOVE) $(PRESERVER).so

$(PRESERVER).so: $(PRESERVER).c
	$(CC) $(CFLAGS) -shared $^ -o $@

test: $(addprefix $(TESTS_DIR)/, $(TESTS))

# Tests are just shell scripts because it would be very inconvenient to do them here
$(TESTS_DIR)/%: $(MOVE) $(PRESERVER).so
	$@ $(abspath $(MOVE)) $(abspath $(PRESERVER).so)

clean:
	rm -rf $(TRASH) $(MOVE) $(PRESERVER).so
