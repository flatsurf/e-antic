r"""
Setup instructions for pyeantic.
"""
# ####################################################################
#  This file is part of e-antic.
#
#        Copyright (C)      2019 Vincent Delecroix
#                      2019-2022 Julian Rüth
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
from setuptools.command.sdist import sdist
from setuptools.command.egg_info import egg_info
from distutils.core import setup
from subprocess import check_call


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


class EggInfoVPath(egg_info, AutotoolsCommand):
    r"""
    A VPATH aware egg_info for VPATH builds with Automake, inspired by
    https://blog.kevin-brown.com/programming/2014/09/24/combining-autotools-and-setuptools.html

    Builds the .egg-info in the --build-base path that was given to "build".

    For this to work, that path must be the path build/ relative to the build
    directory; see AutotoolsCommand.builddir.
    """

    def finalize_options(self):
        if "build" in self.distribution.command_obj:
            self.egg_base = self.builddir

        super().finalize_options()


class MakeDist(sdist, AutotoolsCommand):
    r"""
    Creates a source distribution for PyPI for an autoconfiscated project.
    """

    def run(self):
        if os.path.normpath(os.getcwd()) != os.path.normpath(self.abs_srcdir):
            raise NotImplementedError("A source distribution can only be created from the directory where the setup.py file resides, currently.")

        check_call([os.path.join(self.abs_srcdir, "configure"), "--without-pytest"])
        check_call([self.MAKE, "distdir"])
        super().run()


setup(
    name='pyeantic',
    author='the e-antic authors',
    url='https://flatsurf.github.io/e-antic/pyeantic/',
    version='1.1.0',
    packages=['pyeantic'],
    license='LGPL 3.0+',
    license_files=('COPYING', 'COPYING.LESSER'),
    install_requires=[
        # We cannot encode our dependency on libeantic easily here since
        # libeantic is usually not installed as a Python package.
        # However, pyeantic won't work at all without libeantic of course.
        'cppyy',
    ],
    long_description=inspect.cleandoc(r"""
        pyeantic is a Python interface to the e-antic C/C++ library which offers exact computation with real embedded algebraic numbers.

        We do not recommend to install pyeantic from PyPI as it has lots of
        dependencies that are not available on PyPI. If you want to install
        pyeantic, please have a look at our installation guide:
        https://flatsurf.github.io/e-antic/libeantic/#installation

        Please consult pyeantic's home page for further details: https://flatsurf.github.io/e-antic/pyeantic/
        """),
    include_package_data=True,
    cmdclass={
        'egg_info': EggInfoVPath,
        'sdist': MakeDist,
    },
    package_dir={
        # In VPATH builds, search pyeantic relative to this setup.py file.
        '': os.path.join(os.path.relpath(os.path.dirname(__file__) or "."), 'src'),
    },
)
