######################################################################
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
#####################################################################

def test_nothing():
    r"""
    In SageMath 9.4 invocations of sage-runtests broke for us, see
    https://trac.sagemath.org/timeline?from=2021-09-20T16%3A08%3A13Z&precision=second

    Adding a trivial pytest test works around this problem and makes the
    SageMath tests pass again.
    """
