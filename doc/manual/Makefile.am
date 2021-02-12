EXTRA_DIST = README.md

MARKDOWN=$(builddir)/markdown/index.md $(builddir)/markdown/overview_c.md $(builddir)/markdown/overview_cxx.md $(builddir)/markdown/build.md $(builddir)/markdown/build_in_conda.md $(builddir)/markdown/install_in_conda.md

html: $(builddir)/mkdocs.yml $(builddir)/markdown/pyeantic $(MARKDOWN)
	@# Create inventory of pyeantic (using the test-env scripts which set all
	@# paths so that we can 'import pyeantic' from the build tree.)
	source $(builddir)/../../pyeantic/test/test-env.sh && \
	python $(SPHINXBUILD) -M json $(srcdir)/pyeantic $(builddir)/json/pyeantic
	@# Build markdown files and inventory from e-antic C/C++ headers
	mkdir -p $(builddir)/markdown/libeantic
	$(STANDARDESE) -I $(srcdir)/../../libeantic/e-antic -I $(builddir)/../../libeantic/e-antic -I $(includedir) -I $(srcdir)/../../libeantic/test/external/cereal/include --output.prefix $(builddir)/markdown/libeantic/ --compilation.standard=c++17 `find $(srcdir)/../../libeantic/e-antic/ -name '*.h' -not -path '*external*'`
	@# Create html documentation for pyeantic
	source $(builddir)/../../pyeantic/test/test-env.sh && \
	python $(SPHINXBUILD) -M html $(srcdir)/pyeantic $(builddir)/html/pyeantic
	@# Create html documentation from markdown files
	$(MKDOCS) build --dirty --config-file $(builddir)/mkdocs.yml --site-dir $(builddir)/html/

serve: html
	python -m http.server 8080 --bind 127.0.0.1 --directory html

BUILT_SOURCES = mkdocs.yml markdown/index.md markdown/overview_c.md markdown/overview_cxx.md markdown/build.md markdown/build_in_conda.md markdown/install_in_conda.md

$(builddir)/mkdocs.yml: $(srcdir)/mkdocs.yml.in Makefile
	sed -e 's,[@]srcdir[@],$(srcdir),g' -e 's,[@]builddir[@],$(builddir),g' < $< > $@

$(builddir)/markdown/index.md: $(srcdir)/index.md Makefile
$(builddir)/markdown/overview_c.md: $(srcdir)/overview_c.md Makefile
$(builddir)/markdown/overview_cxx.md: $(srcdir)/overview_cxx.md Makefile
$(builddir)/markdown/build.md: $(srcdir)/build.md Makefile
$(builddir)/markdown/build_in_conda.md: $(srcdir)/build_in_conda.md Makefile
$(builddir)/markdown/install_in_conda.md: $(srcdir)/install_in_conda.md Makefile




$(builddir)/markdown/pyeantic: Makefile
	@mkdir -p $(builddir)/markdown
	@rm -f $(builddir)/markdown/pyeantic
	ln -s ../html/pyeantic $(builddir)/markdown/pyeantic

$(MARKDOWN):
	@mkdir -p $(builddir)/markdown
	cp $< $@

.PHONY: serve
