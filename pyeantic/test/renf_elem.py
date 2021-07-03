#!/usr/bin/env python
# -*- coding: utf-8 -*-

######################################################################
#  This file is part of intervalxt.
#
#        Copyright (C) 2019 Vincent Delecroix
#        Copyright (C) 2019 Julian Rüth
#
#  intervalxt is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or (at your
#  option) any later version.
#
#  intervalxt is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with intervalxt. If not, see <https://www.gnu.org/licenses/>.
#####################################################################

import sys
import pytest

from pyeantic import eantic

def test_repr():
    K = eantic.renf("x^2 - 3", "x", "1.73 +/- 0.1")
    x = K.gen()
    assert repr(x) == "(x ~ 1.7320508)"

def test_construct():
    K = eantic.renf("A^3 - 3", "A", "1.44 +/- 0.1")
    zero = K.zero()
    one = K.one()
    A = K.gen()
    assert eantic.renf_elem(K) == zero
    assert eantic.renf_elem(K, "A") == A
    assert eantic.renf_elem(K, "A - 3") == A - 3
    assert eantic.renf_elem(K, "1") == one
    assert eantic.renf_elem(A) == A
    assert eantic.renf_elem(K, 1) == one
    assert eantic.renf_elem(K, [0,0,0]) == zero
    assert eantic.renf_elem(K, [1,0,0]) == one
    assert eantic.renf_elem(K, [0,1,0]) == A

def test_construct_no_parent():
    assert eantic.renf_elem(1) == 1

def test_arithmetic():
    K = eantic.renf("x^2 - 3", "x", "1.73 +/- 0.1")
    x = K.gen()

    assert x + x == 2*x
    assert not (x - x)
    assert (x + 1) * (x - 1) == x*x - 1
    assert x**2 == 3
    assert -x != x

def test_delete_parent_before_binop():
    K = eantic.renf("x^3 - 3", "x", "1.44 +/- 0.1")
    x = K.gen()
    y = K.gen() + 1
    del K
    z = x + y

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
