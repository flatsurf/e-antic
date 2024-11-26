html: $(builddir)/generated/doxygen $(builddir)/arb.inv $(builddir)/flint.inv $(builddir)/generated/doxygen
	source $(builddir)/../../pyeantic/test/test-env.sh && \
	python $(SPHINXBUILD) -b html $(srcdir) $(builddir)/generated/html

$(builddir)/generated/doxygen:
	mkdir -p $@
	$(DOXYGEN)

$(builddir)/flint.inv:
	wget -O $@ https://flintlib.org/doc/objects.inv

CLEANFILES = flint.inv

all: html

.PHONY: html $(builddir)/generated/doxygen

EXTRA_DIST = README.md robots.txt libeantic pyeantic

mostlyclean-local:
	[[ ! -d generated ]] || (chmod -R u+w generated && rm -rf generated)
