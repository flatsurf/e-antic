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
from distutils.core import setup
from setuptools.command.egg_info import egg_info


# The @srcdir@ containing this setup.py file.
srcdir = os.path.relpath(os.path.dirname(__file__) or ".")


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
    license='GPL 3.0+',
    install_requires=[
        'cppyy',
    ],
    long_description=open(os.path.join(srcdir, '..', '..', 'README.md')).read(),
    include_package_data=True,
    cmdclass={'egg_info': vpath_egg_info},
    package_dir={
        # In VPATH builds, search pyeantic relative to this setup.py file.
        '': srcdir,
    },
)
