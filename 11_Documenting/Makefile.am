SUBDIRS = src po

@DX_RULES@

all-local: doxygen-doc

http: doxygen-doc
	python -m http.server --directory $</html

# Use this to clean all, including the generates made by autoconf
clean-all: clean
	git clean -xdf .
