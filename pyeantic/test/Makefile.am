if HAVE_SAGE
  MAYBE_SAGE = eantic_sage.py sage-doctest.sh
endif
if HAVE_REALALG
  MAYBE_REALALG = realalg-doctest.sh
endif
TESTS = renf.py renf_elem.py python-doctest.sh ${MAYBE_SAGE} ${MAYBE_REALALG}
EXTRA_DIST = $(TESTS) eantic_sage.py

AM_TESTS_ENVIRONMENT = . $(builddir)/test-env.sh;

renf.py: test-env.sh
renf_elem.py: test-env.sh
eantic_sage.py: test-env.sh
python-doctest.sh: test-env.sh
sage-doctest.sh: test-env.sh

@VALGRIND_CHECK_RULES@

BUILT_SOURCES = test-env.sh python-doctest.sh sage-doctest.sh
EXTRA_DIST += test-env.sh.in python-doctest.sh.in sage-doctest.sh.in
CLEANFILES = test-env.sh python-doctest.sh sage-doctest.sh
$(builddir)/test-env.sh: $(srcdir)/test-env.sh.in Makefile
	sed -e 's,[@]abs_srcdir[@],$(abs_srcdir),g' -e 's,[@]abs_builddir[@],$(abs_builddir),g' -e 's,[@]pythondir[@],$(pythondir),g' < $< > $@
$(builddir)/python-doctest.sh: $(srcdir)/python-doctest.sh.in Makefile
	sed -e 's,[@]abs_srcdir[@],$(abs_srcdir),g' -e 's,[@]abs_builddir[@],$(abs_builddir),g' < $< > $@
	chmod +x $@
$(builddir)/sage-doctest.sh: $(srcdir)/sage-doctest.sh.in Makefile
	sed -e 's,[@]abs_srcdir[@],$(abs_srcdir),g' -e 's,[@]abs_builddir[@],$(abs_builddir),g' < $< > $@
	chmod +x $@
