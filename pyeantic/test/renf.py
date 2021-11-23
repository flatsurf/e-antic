#!/usr/bin/env python
# -*- coding: utf-8 -*-

######################################################################
#  This file is part of e-antic.
#
#        Copyright (C) 2019 Vincent Delecroix
#        Copyright (C) 2019 Julian Rüth
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
import pytest

from pyeantic import eantic

def test_repr():
    K = eantic.renf("x^2 - 3", "x", "1.73 +/- 0.1")
    assert str(K) == repr(K)
    assert str(K) == "NumberField(x^2 - 3, [1.7320508075688772935274463415058723669 +/- 5.08e-38])"

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
