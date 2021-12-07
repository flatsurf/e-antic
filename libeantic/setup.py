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
from subprocess import check_call
from contextlib import contextmanager


@contextmanager
def cwd(path):
    pwd = os.getcwd()
    os.chdir(path)
    try:
        yield path
    finally:
        os.chdir(pwd)


def abs_builddir(distribution):
    builddir = distribution.command_obj["build"].build_base
    if not os.path.isabs(builddir):
        builddir = os.path.join(abs_srcdir, builddir)
    os.makedirs(builddir, exist_ok=True)
    return builddir


def prefix(distribution):
    if "install" in distribution.command_obj:
        return os.path.join(distribution.command_obj["install"].install_lib, "libeantic")
    elif "bdist_wheel" in distribution.command_obj:
        return os.path.join(distribution.command_obj["bdist_wheel"].bdist_dir, "libeantic")
    else:
        raise NotImplementedError("Cannot determine installation prefix in this distribution.")


def destdir(distribution):
    return os.path.join(abs_builddir(distribution))


# The @abs_srcdir@ containing this setup.py file.
abs_srcdir = os.path.abspath(os.path.dirname(__file__) or ".")


MAKE = os.getenv("MAKE", "make")


class ConfigureMake(build):
    def run(self):
        super().run()

        with cwd(abs_builddir(self.distribution)):
            check_call([os.path.join(abs_srcdir, "configure"), "--prefix=/install", "--without-benchmark", "--without-byexample"])

            check_call([MAKE, f"DESTDIR={destdir(self.distribution)}", "install"])


class MakeInstall(install):
    def run(self):
        super().run()

        self.copy_tree(os.path.join(destdir(self.distribution), "install"), prefix(self.distribution))


setup(
    name='libeantic',
    long_description=open('../README.md').read(),
    version='1.0.3',
    license='GPL 3.0+',
    setup_requires=["wheel"],
    cmdclass={
        'build': ConfigureMake,
        'install': MakeInstall,
    },
)
