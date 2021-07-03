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

def test_nf():
    from sage.all import NumberField, QQ, AA, Rational, polygen
    from pyeantic.sage_conversion import sage_nf_to_eantic

    x = polygen(QQ)

    # linear
    K = NumberField(x - 3, 'x', embedding=QQ(3))
    L = sage_nf_to_eantic(K)

    # quadratic
    K = NumberField(2*x**2 - 3, 'A', embedding=AA(Rational((3,2)))**Rational((1,2)))
    L = sage_nf_to_eantic(K)

    # cubic
    p = x**3 - x**2 - x - 1
    s = p.roots(AA, False)[0]
    K = NumberField(x**3 - x**2 - x - 1, 's', embedding=s)
    L = sage_nf_to_eantic(K)

def test_element():
    from pyeantic import eantic
    from sage.all import ZZ, QQ
    K = eantic.renf('x^2 - 2', 'x', '[1.4142 +/- 0.0001]')

    assert str(eantic.renf_elem(K, [ZZ(2), ZZ(3)])) == "(3*x+2 ~ 6.2426407)"
    assert str(eantic.renf_elem(K, [1/ZZ(2), 1/ZZ(3)])) == "(1/3*x+1/2 ~ 0.97140452)"

def test_arithmetic():
    from pyeantic import eantic
    from sage.all import ZZ, QQ
    K = eantic.renf('x^2 - 2', 'x', '[1.4142 +/- 0.0001]')
    a = K.gen()

    assert (a + ZZ(1)) * (a - ZZ(1)) == (a*a - ZZ(1))
    assert (a + 1/ZZ(2)) * (a - 1/ZZ(2)) == (a*a - 1/ZZ(4)) 

def test_cmp():
    from pyeantic import eantic
    from sage.all import ZZ, QQ
    K = eantic.renf('x^2 - 2', 'x', '[1.4142 +/- 0.0001]')
    a = K.gen()

    assert a == a
    assert a <= a
    assert a >= a
    assert a > 0
    assert 0 < a
    assert a != 0

if __name__ == '__main__':
    sys.exit(pytest.main(sys.argv))
