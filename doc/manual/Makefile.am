PORT ?= 8080
EXTRA_STANDARDESE_FLAGS ?=

STANDARDESE_FLAGS = --warn-as-error --free-file-comments -X
# Configure search paths for #include statements
STANDARDESE_FLAGS += -I $(srcdir)/../../libeantic/e-antic -I $(builddir)/../../libeantic/e-antic -I $(includedir) -I $(srcdir)/../../libeantic/test/external/cereal/include
# Link against FLINT's Sphinx documentation.
STANDARDESE_FLAGS += --external sphinx:flint:flint.inv=https://flintlib.org/sphinx/
# Link against Arb's Sphinx documentation.
STANDARDESE_FLAGS += --external sphinx:arb:arb.inv=https://arblib.org/
# Allow === Output Section === Syntax
STANDARDESE_FLAGS += -p output_section='=== (.*) ==='
# Allow ==* Group *== Syntax
STANDARDESE_FLAGS += -p group='==\* ((?:.(?!\*==))+)() \*=='
# Allow :param:`name` syntax like in Sphinx.
STANDARDESE_FLAGS += -p param=':param:`([^`]*)`'
# We will serve the C/C++ documentation from the e-antic/libeantic/ subdirectory
STANDARDESE_FLAGS += --vpath 'e-antic/libeantic/doc_{{ sanitize_basename(name) }}'
# Shorten type names in the documentation that reference common namespaces or use complicated aliases.
STANDARDESE_FLAGS += --format type='{% set formatted = format(option("default:type_format")) %}{% set formatted = replace(formatted, "std::|eantic::", "") %}{% set formatted = replace(formatted, "mp_limb_signed_t", "slong") %}{% set formatted = replace(formatted, "mp_bitcnt_t", "ulong") %}{{ formatted }}'

STANDARDESE_FLAGS += ${EXTRA_STANDARDESE_FLAGS}

# Process all header files with standardese.
STANDARDESE_INPUTS = `find $(srcdir)/../../libeantic/e-antic/ \( -name '*.h' -o -name '*.hpp' \) -not -path '*external*'`
# Progress all MarkDown files with standardese.
STANDARDESE_INPUTS += `find $(srcdir)/libeantic/ -name '*.md'`

$(builddir)/generated/pyeantic/source:
	@# Create a copy of the source dir with some generated assets since Sphinx cannot read from more than one directory.
	mkdir -p $(builddir)/generated/pyeantic
	cp -RT $(srcdir)/pyeantic/ $(builddir)/generated/pyeantic/source

$(builddir)/generated/pyeantic/inventory: $(builddir)/generated/pyeantic/source
	@# Create an inventory of pyeantic so standardese can link against it.
	source $(builddir)/../../pyeantic/test/test-env.sh && \
	python $(SPHINXBUILD) -b json $(builddir)/generated/pyeantic/source $(builddir)/build/pyeantic/inventory -Q

$(builddir)/generated/pyeantic/html: $(builddir)/generated/libeantic/inventory $(builddir)/generated/pyeantic/source
	@# Create HTML pages for pyeantic with Sphinx
	source $(builddir)/../../pyeantic/test/test-env.sh && \
	python $(SPHINXBUILD) -b html -n $(builddir)/generated/pyeantic/source $(builddir)/generated/pyeantic/html --color -a -E -W

$(builddir)/generated/libeantic/inventory:
	@# Create an inventory of libeantic so Sphinx can link against it. (This also creates the entire documentation since there is no flag in standardese yet to only generate the inventory.)
	mkdir -p $(builddir)/generated/libeantic/inventory
	$(STANDARDESE) --outdir $(builddir)/generated/libeantic/inventory/ $(STANDARDESE_FLAGS) $(STANDARDESE_INPUTS)

$(builddir)/generated/libeantic/markdown: $(builddir)/generated/pyeantic/inventory $(builddir)/arb.inv $(builddir)/flint.inv
	@# Build markdown files from e-antic C/C++ headers
	mkdir -p $(builddir)/generated/libeantic/
	cp -RT $(srcdir)/libeantic/ $(builddir)/generated/libeantic/markdown
	$(STANDARDESE) --outdir $(builddir)/generated/libeantic/markdown/ $(STANDARDESE_FLAGS) $(STANDARDESE_INPUTS)

$(builddir)/generated/libeantic/html: $(builddir)/generated/libeantic/markdown $(builddir)/generated/libeantic/markdown/mkdocs.yml
	@# Create html documentation from markdown files
	cd $(builddir)/generated/libeantic/markdown &&\
	$(MKDOCS) build --config-file mkdocs.yml --site-dir $(abs_builddir)/generated/libeantic/html/

$(builddir)/generated/libeantic/markdown/mkdocs.yml: $(srcdir)/libeantic/mkdocs.yml.in Makefile
	mkdir -p $(builddir)/generated/libeantic/markdown
	sed -e 's,[@]srcdir[@],$(srcdir),g' -e 's,[@]builddir[@],$(builddir),g' < $< > $@

$(builddir)/arb.inv:
	wget -O $@ https://arblib.org/objects.inv

CLEANFILES = arb.inv

$(builddir)/flint.inv:
	wget -O $@ https://flintlib.org/sphinx/objects.inv

CLEANFILES += flint.inv

html: $(builddir)/generated/libeantic/html $(builddir)/generated/pyeantic/html
	mkdir -p $(builddir)/generated/html/e-antic
	cp -RT $(builddir)/generated/libeantic/html/ $(builddir)/generated/html/e-antic/libeantic
	cp -RT $(builddir)/generated/pyeantic/html/ $(builddir)/generated/html/e-antic/pyeantic

serve: html
	python -m http.server ${PORT} --bind 127.0.0.1 --directory $(builddir)/generated/html

all: html

.PHONY: serve $(builddir)/generated/libeantic/html $(builddir)/generated/libeantic/markdown $(builddir)/generated/libeantic/inventory $(builddir)/generated/pyeantic/html $(builddir)/generated/pyeantic/inventory $(builddir)/generated/pyeantic/source

EXTRA_DIST = README.md libeantic pyeantic

mostlyclean-local:
	[[ ! -d generated ]] || (chmod -R u+w generated && rm -rf generated)
	[[ ! -d build ]] || (chmod -R u+w build && rm -rf build)
