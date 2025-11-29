SUBDIRS = src tests

check-tests-logs: check
	cat tests/*.log

gcov: check
	$(MAKE) $(MAKEFLAGS) -C src gcov

# Use this to clean all, including the generates made by autoconf
clean-all: clean
	git clean -xdf .