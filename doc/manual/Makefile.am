html: $(builddir)/generated/doxygen $(builddir)/flint.inv $(builddir)/flint.inv $(builddir)/generated/doxygen
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

EXTRA_DIST = README.md robots.txt Doxyfile c_fmpq_poly_extra.rst c_fmpz_poly_extra.rst c_overview.rst c_renf_elem.rst c_renf.rst cxx_overview.rst cxx_renf_class.rst cxx_renf_elem_class.rst index.rst python_overview.rst python_realalg.rst python_sage.rst

mostlyclean-local:
	[[ ! -d generated ]] || (chmod -R u+w generated && rm -rf generated)
