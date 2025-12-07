SUBDIRS = src po

@DX_RULES@

main_page_for_doxygen.c:
	$(MAKE) -C src guesser
	echo -ne '/** \mainpage\n * ' > $@
	LC_ALL=C ./src/guesser --help | sed -E 's/Usage: (.*)/Usage: `\1`/g' | tr '\n' '\r' | sed -E 's/-[^ \t,]*/`&`/g' | sed 's/\r/  \n * /g' >> $@
	echo '*/' >> $@

$(DX_DOCDIR)/$(PACKAGE).tag: main_page_for_doxygen.c

.INTERMEDIATE: main_page_for_doxygen.c

guesser.1:
	$(MAKE) -C src guesser
	LC_ALL=C help2man -s 1 --version-string="$(VERSION)" ./src/guesser > $@

man1_MANS = guesser.1

all-local: doxygen-doc

install-data-local:
	mkdir -p $(DESTDIR)/$(docdir)
	cp -a doxygen-doc/html $(DESTDIR)/$(docdir)

http: doxygen-doc
	python3 -m http.server --directory $</html

# Use this to clean all, including the generates made by autoconf
clean-all: clean
	git clean -xdf .
