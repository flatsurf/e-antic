r"""
Setup instructions for pyeantic.
"""
# ####################################################################
#  This file is part of e-antic.
#
#        Copyright (C)      2019 Vincent Delecroix
#                      2019-2021 Julian Rüth
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
from distutils.core import setup
from setuptools.command.egg_info import egg_info


class vpath_egg_info(egg_info):
    r"""
    A VPATH aware egg_info for VPATH builds with Automake, see
    https://blog.kevin-brown.com/programming/2014/09/24/combining-autotools-and-setuptools.html
    """

    def run(self):
        r"""
        Run egg_info.run() but build the .egg-info in the --build-base path
        that was given to "build".
        """
        if "build" in self.distribution.command_obj:
            build_command = self.distribution.command_obj["build"]
            self.egg_base = build_command.build_base
            self.egg_info = os.path.join(self.egg_base, os.path.basename(self.egg_info))
        egg_info.run(self)


setup(
    name='pyeantic',
    version='1.0.3',
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

        Please consult pyeantic's home page for further details: https://flatsurf.github.io/e-antic/pyeantic/
        """),
    include_package_data=True,
    cmdclass={'egg_info': vpath_egg_info},
    package_dir={
        # In VPATH builds, search pyeantic relative to this setup.py file.
        '': os.path.join(os.path.relpath(os.path.dirname(__file__) or "."), 'src'),
    },
)
