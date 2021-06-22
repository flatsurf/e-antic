if HAVE_SAGE
  MAYBE_SAGE = eantic_sage.py sage-doctest.sh
endif
if HAVE_REALALG
  MAYBE_REALALG = realalg-doctest.sh
endif
TESTS = renf.py renf_elem.py python-doctest.sh ${MAYBE_SAGE} ${MAYBE_REALALG}
EXTRA_DIST = $(TESTS) eantic_sage.py

AM_TESTS_ENVIRONMENT = . $(builddir)/test-env.sh;

renf.py: test-env.sh bin/python
renf_elem.py: test-env.sh bin/python
eantic_sage.py: test-env.sh bin/python
python-doctest.sh: test-env.sh bin/python
sage-doctest.sh: test-env.sh bin/python
realalg-doctest.sh: test-env.sh bin/python

@VALGRIND_CHECK_RULES@

BUILT_SOURCES = test-env.sh bin/python python-doctest.sh sage-doctest.sh realalg-doctest.sh
EXTRA_DIST += test-env.sh.in bin/python.in python-doctest.sh.in sage-doctest.sh.in realalg-doctest.sh.in
CLEANFILES = test-env.sh bin/python python-doctest.sh sage-doctest.sh realalg-doctest.sh

$(builddir)/test-env.sh: $(srcdir)/test-env.sh.in Makefile
	sed -e 's,[@]abs_srcdir[@],$(abs_srcdir),g' -e 's,[@]abs_builddir[@],$(abs_builddir),g' -e 's,[@]pythondir[@],$(pythondir),g' < $< > $@

$(builddir)/bin/python: $(srcdir)/bin/python.in Makefile
	mkdir -p $(builddir)/bin
	sed -e 's,[@]abs_srcdir[@],$(abs_srcdir),g' -e 's,[@]abs_builddir[@],$(abs_builddir),g' -e 's,[@]pythondir[@],$(pythondir),g' < $< > $@
	chmod +x $@

$(builddir)/python-doctest.sh: $(srcdir)/python-doctest.sh.in Makefile
	sed -e 's,[@]abs_srcdir[@],$(abs_srcdir),g' -e 's,[@]abs_builddir[@],$(abs_builddir),g' < $< > $@
	chmod +x $@

$(builddir)/realalg-doctest.sh: $(srcdir)/realalg-doctest.sh.in Makefile
	sed -e 's,[@]abs_srcdir[@],$(abs_srcdir),g' -e 's,[@]abs_builddir[@],$(abs_builddir),g' < $< > $@
	chmod +x $@

$(builddir)/sage-doctest.sh: $(srcdir)/sage-doctest.sh.in Makefile
	sed -e 's,[@]abs_srcdir[@],$(abs_srcdir),g' -e 's,[@]abs_builddir[@],$(abs_builddir),g' < $< > $@
	chmod +x $@
