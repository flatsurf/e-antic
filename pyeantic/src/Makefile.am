all-local:
	mkdir -p $(abs_builddir)/build
	cd $(abs_srcdir) && $(PYTHON) $(abs_srcdir)/setup.py build --verbose --build-base $(abs_builddir)/build

install-exec-local:
	$(PYTHON) $(abs_srcdir)/setup.py install --prefix $(DESTDIR)$(prefix) --single-version-externally-managed --record $(DESTDIR)$(pythondir)/pyeantic/install_files.txt --verbose

uninstall-local:
	cat $(DESTDIR)$(pythondir)/pyeantic/install_files.txt | xargs rm -rf
	rm -rf $(DESTDIR)$(pythondir)/pyeantic

clean-local:
	-rm -rf pyeantic/__pycache__ pyeantic.egg-info build .pytest_cache

EXTRA_DIST = setup.py pyeantic/__init__.py pyeantic/cppyy_eantic.py pyeantic/realalg_conversion.py pyeantic/real_embedded_number_field.py pyeantic/sage_conversion.py
