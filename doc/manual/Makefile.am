export ABS_SRCDIR=$(abs_srcdir)
export ABS_BUILDDIR=$(abs_builddir)

generated/html/index.html: generated/doxygen/xml/index.xml flint.inv $(srcdir)/*.rst $(srcdir)/conf.py $(srcdir)/_static/extra.css
	$(MAKE) -C ../../pyeantic/test test-env.sh bin/python
	$(MAKE) -C ../../libeantic/src
	$(MAKE) -C ../../libeantic/srcxx
	source ../../pyeantic/test/test-env.sh && \
	python $(SPHINXBUILD) -b html -W $(srcdir) $(builddir)/generated/html
	cp $(srcdir)/robots.txt generated/html/

generated/doxygen/xml/index.xml: $(srcdir)/Doxyfile $(srcdir)/../../libeantic/e-antic/*.h $(srcdir)/../../libeantic/e-antic/*.hpp
	mkdir -p generated/doxygen
	$(DOXYGEN) $(srcdir)/Doxyfile

flint.inv:
	wget -O $@ https://flintlib.org/doc/objects.inv

CLEANFILES = flint.inv

all: generated/html/index.html

EXTRA_DIST = README.md
EXTRA_DIST += robots.txt _static/extra.css
EXTRA_DIST += Doxyfile
EXTRA_DIST += conf.py index.rst
EXTRA_DIST += c_fmpq_poly_extra.rst c_fmpz_poly_extra.rst c_overview.rst c_renf_elem.rst c_renf.rst
EXTRA_DIST += cxx_overview.rst cxx_renf_class.rst cxx_renf_elem_class.rst
EXTRA_DIST += python_overview.rst python_realalg.rst python_sage.rst

mostlyclean-local:
	[[ ! -d generated ]] || (chmod -R u+w generated && rm -rf generated)
