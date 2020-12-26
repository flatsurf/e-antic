######################################################################
#  This file is part of e-antic.
#
#        Copyright (C) 2020 Julian Rüth
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
#####################################################################

import sys
from packaging import version

from rever.activity import activity

try:
  input("Are you sure you are on the master branch which is identical to origin/master and the only pending changes are a version_info bump in configure.ac? [ENTER]")
except KeyboardInterrupt:
  sys.exit(1)

@activity
def dist():
    r"""
    Run make dist and collect the resulting tarball.
    """
    from tempfile import TemporaryDirectory
    from xonsh.dirstack import DIRSTACK
    with TemporaryDirectory() as tmp:
        ./bootstrap
        pushd @(tmp)
        @(DIRSTACK[-1])/configure
        make dist
        mv *.tar.gz @(DIRSTACK[-1])
        popd
    return True

$PROJECT = 'e-antic'

$ACTIVITIES = [
    'version_bump',
    'changelog',
    'dist',
    'tag',
    'push_tag',
    'ghrelease',
]

MAJOR, MINOR, PATCH = version.parse($VERSION).release

$VERSION_BUMP_PATTERNS = [
    ('configure.ac', r'AC_INIT', r'AC_INIT([e-antic], [$VERSION], [vincent.delecroix@math.cnrs.fr])'),
    ('libeantic/configure.ac', r'AC_INIT', r'AC_INIT([libeantic], [$VERSION], [vincent.delecroix@math.cnrs.fr])'),
    ('libeantic/e-antic/e-antic.h.in', r'#define E_ANTIC_VERSION ', r'#define E_ANTIC_VERSION "$VERSION"'),
    ('libeantic/e-antic/e-antic.h.in', r'#define E_ANTIC_VERSION_MAJOR', rf'#define E_ANTIC_VERSION_MAJOR {MAJOR}'),
    ('libeantic/e-antic/e-antic.h.in', r'#define E_ANTIC_VERSION_MINOR', rf'#define E_ANTIC_VERSION_MINOR {MINOR}'),
    ('libeantic/e-antic/e-antic.h.in', r'#define E_ANTIC_VERSION_PATCHLEVEL', rf'#define E_ANTIC_VERSION_PATCHLEVEL {PATCH}'),
    ('libeantic/recipe/meta.yaml', r"\{% set version =", r"{% set version = '$VERSION' %}"),
    ('libeantic/recipe/meta.yaml', r"\{% set build_number =", r"{% set build_number = '0' %}"),
    ('pyeantic/recipe/meta.yaml', r"\{% set version =", r"{% set version = '$VERSION' %}"),
    ('pyeantic/recipe/meta.yaml', r"\{% set build_number =", r"{% set build_number = '0' %}"),
]

$CHANGELOG_FILENAME = 'ChangeLog'
$CHANGELOG_TEMPLATE = 'TEMPLATE.rst'
$CHANGELOG_NEWS = 'doc/news'
$CHANGELOG_CATEGORIES = ('Added', 'Changed', 'Deprecated', 'Removed', 'Fixed', 'Performance')
$PUSH_TAG_REMOTE = 'git@github.com:flatsurf/e-antic.git'

$GITHUB_ORG = 'flatsurf'
$GITHUB_REPO = 'e-antic'

$GHRELEASE_ASSETS = ['e-antic-' + $VERSION + '.tar.gz']
