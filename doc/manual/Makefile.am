html: $(builddir)/generated/doxygen $(builddir)/flint.inv $(builddir)/flint.inv $(builddir)/generated/doxygen
	source $(builddir)/../../pyeantic/test/test-env.sh && \
	python $(SPHINXBUILD) -b html $(srcdir) $(builddir)/generated/html

$(builddir)/generated/doxygen:
	mkdir -p $@
	$(DOXYGEN) $(srcdir)/Doxyfile

$(builddir)/flint.inv:
	wget -O $@ https://flintlib.org/doc/objects.inv

CLEANFILES = flint.inv

all: html

.PHONY: html $(builddir)/generated/doxygen

EXTRA_DIST = README.md
EXTRA_DIST += robots.txt _static/extra.css
EXTRA_DIST += Doxyfile
EXTRA_DIST += conf.py index.rst
EXTRA_DIST += c_fmpq_poly_extra.rst c_fmpz_poly_extra.rst c_overview.rst c_renf_elem.rst c_renf.rst
EXTRA_DIST += cxx_overview.rst cxx_renf_class.rst cxx_renf_elem_class.rst
EXTRA_DIST += python_overview.rst python_realalg.rst python_sage.rst

mostlyclean-local:
	[[ ! -d generated ]] || (chmod -R u+w generated && rm -rf generated)
