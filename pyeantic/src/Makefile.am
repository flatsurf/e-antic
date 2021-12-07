all-local:
	mkdir -p $(builddir)/build
	cd $(srcdir) && $(PYTHON) setup.py build --verbose --build-base $(abs_top_builddir)/src/build

install-exec-local:
	$(PYTHON) setup.py install --prefix $(DESTDIR)$(prefix) --single-version-externally-managed --record $(DESTDIR)$(pythondir)/pyeantic/install_files.txt --verbose

uninstall-local:
	cat $(DESTDIR)$(pythondir)/pyeantic/install_files.txt | xargs rm -rf
	rm -rf $(DESTDIR)$(pythondir)/pyeantic

clean-local:
	-rm -rf pyeantic/__pycache__ pyeantic.egg-info build .pytest_cache

EXTRA_DIST = setup.py MANIFEST.in pyeantic/__init__.py pyeantic/cppyy_eantic.py pyeantic/realalg_conversion.py pyeantic/real_embedded_number_field.py pyeantic/sage_conversion.py
