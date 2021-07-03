TESTS = manual-doctest.sh

EXTRA_DIST = $(TESTS)

AM_TESTS_ENVIRONMENT = . $(builddir)/test-env.sh;

@VALGRIND_CHECK_RULES@

BUILT_SOURCES = test-env.sh bin/python manual-doctest.sh
EXTRA_DIST += test-env.sh.in bin/python.in manual-doctest.sh.in
CLEANFILES = test-env.sh bin/python manual-doctest.sh

$(builddir)/test-env.sh: $(srcdir)/test-env.sh.in Makefile
	sed -e 's,[@]abs_builddir[@],$(abs_builddir),g' < $< > $@

$(builddir)/bin/python: $(srcdir)/bin/python.in Makefile
	mkdir -p $(builddir)/bin
	sed -e 's,[@]abs_srcdir[@],$(abs_srcdir),g' -e 's,[@]abs_builddir[@],$(abs_builddir),g' < $< > $@
	chmod +x $@

$(builddir)/manual-doctest.sh: $(srcdir)/manual-doctest.sh.in Makefile
	sed -e 's,[@]abs_srcdir[@],$(abs_srcdir),g' < $< > $@
	chmod +x $@
