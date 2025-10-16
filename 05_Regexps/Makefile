ESUB = esub
SED_RUNNER = run_sed.sh
TESTS = 01 02 03 04 05
TESTS_DIR = tests
TESTS_OUTPUT_DIR = tests_output
TRASH = *.o $(TESTS_OUTPUT_DIR)

.PHONY: all clean test

all: $(ESUB)

%/:
	mkdir -p $@

test: $(addprefix test, $(TESTS))

test%: $(TESTS_DIR)/%.test $(TESTS_OUTPUT_DIR)/%.out $(ESUB)
	xargs < $< $(CURDIR)/$(ESUB) | cmp $(word 2, $^)

$(TESTS_OUTPUT_DIR)/%.out: $(TESTS_DIR)/%.test $(TESTS_OUTPUT_DIR)/ $(SED_RUNNER)
	xargs < $< $(CURDIR)/$(SED_RUNNER) > $@

clean:
	rm -rf $(TRASH) $(ESUB)
