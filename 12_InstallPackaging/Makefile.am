SUBDIRS = src po tests

@DX_RULES@

matrix_ops_runner.1:
	$(MAKE) -C src matrix_ops_runner
	LC_ALL=C help2man -s 1 ./src/matrix_ops_runner > $@

man1_MANS = matrix_ops_runner.1

all-local: doxygen-doc

install-data-local:
	mkdir -p $(DESTDIR)/$(docdir)
	cp -a doxygen-doc/html $(DESTDIR)/$(docdir)

uninstall-local:
	rm -rf $(DESTDIR)/$(docdir)

http: doxygen-doc
	python3 -m http.server --directory $</html

check-tests-logs:
	cat tests/*.log

# Use this to clean all, including the generates made by autoconf
clean-all: clean
	git clean -xdf .