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
#        Copyright (C) 2021 Julian Rüth
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
from setuptools import setup
from setuptools.command.install import install
from distutils.command.build import build
from setuptools.command.sdist import sdist
from subprocess import check_call
from contextlib import contextmanager
import tempfile


@contextmanager
def cwd(path):
    pwd = os.getcwd()
    os.chdir(path)
    try:
        yield path
    finally:
        os.chdir(pwd)


class AutotoolsCommand:
    @property
    def abs_builddir(self):
        builddir = self.distribution.command_obj["build"].build_base
        if not os.path.isabs(builddir):
            builddir = os.path.join(self.abs_srcdir, builddir)
        os.makedirs(builddir, exist_ok=True)
        return builddir

    @property
    def destdir(self):
        if "install" in self.distribution.command_obj:
            return os.path.join(self.distribution.command_obj["install"].install_lib, "libeantic")
        else:
            raise NotImplementedError("Cannot determine installation prefix in this build. We need to know the eventual location of the install so hard-coded library paths are set correctly.")

    @property
    def abs_srcdir(self):
        # The @abs_srcdir@ containing this setup.py file.
        return os.path.abspath(os.path.dirname(__file__) or ".")

    @property
    def MAKE(self):
        return os.getenv("MAKE", "make")


class NoBinary(build):
    def run(self):
        raise NotImplementedError("No binary wheels can be built for libeantic currently because the installation prefix is hard-coded in some of its header files. To skip this step when using pip, run with --no-binary :all:")


class ConfigureMakeInstall(install, AutotoolsCommand):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.skip_build = True

    def run(self):
        super().run()

        with tempfile.TemporaryDirectory() as build:
            with cwd(build):
                check_call([os.path.join(self.abs_srcdir, "configure"), f"--prefix={self.destdir}", "--without-benchmark", "--without-byexample"])
                check_call([self.MAKE, "install"])

    def get_outputs(self):
        return super().get_outputs() + [self.destdir]


class MakeDist(sdist, AutotoolsCommand):
    def run(self):
        with cwd(self.abs_srcdir):
            check_call([os.path.join(self.abs_srcdir, "configure"), "--without-benchmark", "--without-byexample"])
            check_call([self.MAKE, "distdir"])
            super().run()


setup(
    name='libeantic',
    long_description=r"""
        TODO
    """,
    version='1.0.3',
    license='GPL 3.0+',
    setup_requires=["wheel"],
    cmdclass={
        'sdist': MakeDist,
        'build': NoBinary,
        'install': ConfigureMakeInstall,
    },
)
