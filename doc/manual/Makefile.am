EXTRA_DIST = README.md
PORT ?= 8080

# TODO: Set warn-as-error for most invocations.

$(builddir)/generated/pyeantic/source:
	@# Create a copy of the source dir with some generated assets since Sphinx cannot read from more than one directory.
	mkdir -p $(builddir)/generated/pyeantic
	cp -RT $(srcdir)/pyeantic/ $(builddir)/generated/pyeantic/source

$(builddir)/generated/pyeantic/inventory: $(builddir)/generated/pyeantic/source
	@# Create an inventory of pyeantic so standardese can link against it.
	source $(builddir)/../../pyeantic/test/test-env.sh && \
	python $(SPHINXBUILD) -b json $(builddir)/generated/pyeantic/source $(builddir)/build/pyeantic/inventory -Q

$(builddir)/generated/pyeantic/html: $(builddir)/generated/libeantic/inventory $(builddir)/generated/pyeantic/source
	@# Create HTML pages for pyeantic with sphinx
	source $(builddir)/../../pyeantic/test/test-env.sh && \
	python $(SPHINXBUILD) -b html -n $(builddir)/generated/pyeantic/source $(builddir)/generated/pyeantic/html --color -a -E -W

$(builddir)/generated/libeantic/inventory:
	@# Create an inventory of libeantic so sphinx can link against it.
	## TODO: Only generate inventory
	mkdir -p $(builddir)/generated/libeantic/inventory
	$(STANDARDESE) --free-file-comments -X -I $(srcdir)/../../libeantic/e-antic -I $(builddir)/../../libeantic/e-antic -I $(includedir) -I $(srcdir)/../../libeantic/test/external/cereal/include --external sphinx:flint:flint.inv=https://flintlib.org/sphinx/ --external sphinx:arb:arb.inv=https://arblib.org/ -p output_section='=== (.*) ===' -p group='==\* ((?:.(?!\*==))+)() \*==' -p param=':param:`([^`]*)`' --vpath 'libeantic/doc_{{ sanitize_basename(name) }}' --outdir $(builddir)/generated/libeantic/inventory/ `find $(srcdir)/../../libeantic/e-antic/ \( -name '*.h' -o -name '*.hpp' \) -not -path '*external*'`

$(builddir)/generated/libeantic/markdown: $(builddir)/generated/pyeantic/inventory $(builddir)/arb.inv $(builddir)/flint.inv
	@# Build markdown files from e-antic C/C++ headers
	##TODO: Have standardese parse CPPFLAGS instead.
	mkdir -p $(builddir)/generated/libeantic/
	cp -RT $(srcdir)/libeantic/ $(builddir)/generated/libeantic/markdown
	$(STANDARDESE) --warn-as-error --free-file-comments -X -I $(srcdir)/../../libeantic/e-antic -I $(builddir)/../../libeantic/e-antic -I $(includedir) -I $(srcdir)/../../libeantic/test/external/cereal/include --external sphinx:flint:flint.inv=https://flintlib.org/sphinx/ --external sphinx:arb:arb.inv=https://arblib.org/ -p output_section='=== (.*) ===' -p group='==\* ((?:.(?!\*==))+)() \*==' -p param=':param:`([^`]*)`' --format type='{% set formatted = format(option("default:type_format")) %}{% set formatted = replace(formatted, "std::|eantic::", "") %}{% set formatted = replace(formatted, "mp_limb_signed_t", "slong") %}{% set formatted = replace(formatted, "mp_bitcnt_t", "ulong") %}{{ formatted }}' --vpath 'libeantic/doc_{{ sanitize_basename(name) }}' --outdir $(builddir)/generated/libeantic/markdown/ `find $(srcdir)/../../libeantic/e-antic/ \( -name '*.h' -o -name '*.hpp' \) -not -path '*external*'` `find $(srcdir)/libeantic/ -name '*.md'`

$(builddir)/generated/libeantic/html: $(builddir)/generated/libeantic/markdown $(builddir)/generated/libeantic/markdown/mkdocs.yml
	@# Create html documentation from markdown files
	cd $(builddir)/generated/libeantic/markdown &&\
	$(MKDOCS) build --config-file mkdocs.yml --site-dir $(abs_builddir)/generated/libeantic/html/

$(builddir)/generated/libeantic/markdown/mkdocs.yml: $(srcdir)/libeantic/mkdocs.yml.in Makefile
	mkdir -p $(builddir)/generated/libeantic/markdown
	sed -e 's,[@]srcdir[@],$(srcdir),g' -e 's,[@]builddir[@],$(builddir),g' < $< > $@

$(builddir)/arb.inv:
	wget -O $@ https://arblib.org/objects.inv

$(builddir)/flint.inv:
	wget -O $@ https://flintlib.org/sphinx/objects.inv

html: $(builddir)/generated/libeantic/html $(builddir)/generated/pyeantic/html
	mkdir -p $(builddir)/generated/e-antic/html
	cp -RT $(builddir)/generated/libeantic/html/ $(builddir)/generated/e-antic/html/libeantic
	cp -RT $(builddir)/generated/pyeantic/html/ $(builddir)/generated/e-antic/html/pyeantic

serve: html
	python -m http.server ${PORT} --bind 127.0.0.1 --directory $(builddir)/generated/e-antic/html

all: html

.PHONY: serve $(builddir)/generated/libeantic/html $(builddir)/generated/libeantic/markdown $(builddir)/generated/libeantic/inventory $(builddir)/generated/pyeantic/html $(builddir)/generated/pyeantic/inventory $(builddir)/generated/pyeantic/source

EXTRA_DIST = README.md libeantic pyeantic
