RU = ru/LC_MESSAGES

bin_PROGRAMS = guesser

guesser_SOURCES = src/guesser.c

pkgdata_DATA = $(RU)/$(PACKAGE).mo

$(RU)/$(PACKAGE).mo: ru.po
	mkdir -p `dirname $@`
	msgfmt $< -o $@

# Use this to clean all, including the generates made by autoconf
clean-all: clean
	git clean -xdf .
