r"""
Pip-compatible setup instructions for libeantic.

Being a C/C++-library with lots of dependencies, libeantic cannot be sanely
installed with pip from binary builds. It should be installed through a proper
package manager. When this is not possible, it needs to be installed from
source with configure-make-make-install. However, in some scenarios, hiding
this build inside a pip-package is advantageous, e.g., when installing inside a
SageMath source build or when packages have to be installed with a
requirements.txt.
"""
# ####################################################################
#  This file is part of e-antic.
#
#        Copyright (C) 2021-2022 Julian Rüth
#
#  e-antic is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or (at your
#  option) any later version.
#
#  e-antic is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with e-antic. If not, see <https://www.gnu.org/licenses/>.
# ###################################################################
import os
import inspect
from setuptools import setup
from setuptools.command.install import install
from distutils.command.build import build
from setuptools.command.sdist import sdist
from subprocess import check_call
from contextlib import contextmanager


@contextmanager
def cwd(path):
    r"""
    Change the current working directory to `path` while inside this context.
    """
    pwd = os.getcwd()
    os.chdir(path)
    try:
        yield path
    finally:
        os.chdir(pwd)


class AutotoolsCommand:
    r"""
    Helpers that provide variables like the ones available to automake
    Makefiles.
    """
    @property
    def builddir(self):
        r"""
        Return the path of the build directory, equivalent to @builddir@ in automake.

        This property is only available when a build has been invoked as part of this setup.py run.
        In particular, this is the directory passed with --build-base.
        Note that "setup.py install" does not accept a --build-base so we have
        to make sure that this happens to be build/ relative to the current
        working directory during the install phase since otherwise the install
        step won't be able to find the assets that have been built by the build
        step.
        """
        if "build" not in self.distribution.command_obj:
            raise ValueError("There is no build directory since no build is being performed in this invocation of setup.py")

        return self.distribution.command_obj["build"].build_base

    @property
    def abs_builddir(self):
        r"""
        Return the absolute path of the build directory, equivalent to @abs_builddir@ in automake.

        The limitations of `builddir` apply to this property as well.
        """
        builddir = self.builddir
        if not os.path.isabs(builddir):
            builddir = os.path.join(self.abs_srcdir, builddir)

        return builddir

    @property
    def destdir(self):
        r"""
        Return the installation prefix for this package in site-packages (or the user directory.)

        This is the value that you want to pass to a configure's --prefix flag.
        Note that naturally this value is only available when setup was asked
        to install this package. In particular, this is not available when
        trying to build a wheel.

        As a consequence this value is also not available initially when
        invoking `pip install` since that tries to build a wheel first. You
        might want to invoke pip install with `--no-binary :all:` so that pip
        skips to a regular install where this value is available.
        """
        if "install" not in self.distribution.command_obj:
            raise ValueError("Cannot determine installation prefix in this build which does not install.")
        return os.path.join(self.distribution.command_obj["install"].install_lib, self.distribution.get_name())

    @property
    def distdir(self):
        return f"{self.distribution.get_name()}-{self.distribution.get_version()}"

    @property
    def abs_srcdir(self):
        r"""
        Return the absolute path of the source directory, i.e., the directory where this setup.py is.

        This is the equivalent to @abs_srcdir@ in automake.
        """
        return os.path.abspath(os.path.dirname(__file__) or ".")

    @property
    def MAKE(self):
        r"""
        Return the name of the make command which might have been overridden by
        the MAKE environment variable.
        """
        return os.getenv("MAKE", "make")


class NoBinaryBuild(build):
    r"""
    Disables building binary wheels for this package.

    Since such binary wheels are not relocatable because we hard code library
    paths into our hedaer files so cppyy known where things are located.
    """

    def run(self):
        raise NotImplementedError("No binary wheels can be built for libeantic currently because the installation prefix is hard-coded in some of its header files. To skip this step when using pip, run with --no-binary :all:")


class ConfigureMakeInstall(install, AutotoolsCommand):
    r"""
    Builds and installs libeantic by running configure and make install.
    """

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        # We do not perform a separate build since we don't know the prefix during such a build yet.
        self.skip_build = True

    def run(self):
        super().run()

        # Perform a VPATH build in a temporary directory and install into a
        # directory such as site-packages/libeantic.
        import tempfile
        with tempfile.TemporaryDirectory() as build:
            with cwd(build):
                import os
                env = os.environ.copy()

                # Check whether antic has been installed through pip
                import importlib
                antic = importlib.util.find_spec('antic')
                if antic:
                    # When antic has been pip-installed, we need to add its include/ and lib/ directory to the search path.
                    # Currently, this probably does not work on Windows.
                    import os.path
                    include = os.path.join(antic.submodule_search_locations[0], 'include')
                    # TODO: Need to escape spaces and such.
                    env['CPPFLAGS'] = f"-I{include} {env.get('CPPFLAGS', '')}"

                    lib = os.path.join(antic.submodule_search_locations[0], 'lib')
                    env['LDFLAGS'] = f"-L{lib} {env.get('LDFLAGS', '')}"

                check_call([os.path.join(self.abs_srcdir, "configure"), f"--prefix={self.destdir}", "--without-benchmark", "--without-byexample"], env=env)
                check_call([self.MAKE, "install"], env=env)

    def get_outputs(self):
        r"""
        Return the installed files/directories so we know how to uninstall libeantic again.
        """
        return super().get_outputs() + [self.destdir]


class MakeDist(sdist, AutotoolsCommand):
    r"""
    Create a source distribution for PyPI for an autoconfiscated project.
    """

    def make_release_tree(self, base_dir, files):
        import sys
        if sys.version_info < (3, 8):
            raise NotImplementedError("sdist requires at least Python 3.8")

        import os
        os.makedirs(base_dir, exist_ok=True)
        os.makedirs(self.distdir, exist_ok=True)

        if not os.path.samefile(base_dir, self.distdir):
            raise NotImplementedError(f"automake distdir {self.distdir} and setuptools staging dir {base_dir} must be the same")

        check_call([os.path.join(self.abs_srcdir, "configure"), "--without-benchmark", "--without-byexample"])
        check_call([self.MAKE, "distdir"])

        super().make_release_tree(base_dir, files)


setup(
    name='libeantic',
    author='the e-antic authors',
    url='https://flatsurf.github.io/e-antic/libeantic/',
    # We cannot encode all our dependencies since they are mostly not available
    # as Python packages. Also we want to build with system packages that are
    # detected by our configure script.
    install_requires=[],
    long_description=inspect.cleandoc(r"""
        e-antic is a C/C++/Python library to deal with real embedded number fields built on top of ANTIC. It aims to provide the fastest possible exact arithmetic operations and comparisons.

        We do not recommend to install libeantic from PyPI as it has lots of
        dependencies that are not available on PyPI. If you want to install libeantic, please have a look at our installation guide: https://flatsurf.github.io/e-antic/libeantic/#installation

        Please consult libeantic's home page for further details: https://flatsurf.github.io/e-antic/libeantic/
        """),
    version='1.1.0',
    license='LGPL 3.0+',
    license_files=('COPYING', 'COPYING.LESSER'),
    setup_requires=["wheel"],
    cmdclass={
        'sdist': MakeDist,
        'build': NoBinaryBuild,
        'install': ConfigureMakeInstall,
    },
)
