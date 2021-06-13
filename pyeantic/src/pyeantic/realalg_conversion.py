# -*- coding: utf-8 -*-
r"""
realalg conversions

``realag`` is a pure Python library developed by Mark C. Bell that provides
similarly to ``pyeantic`` exact computations over real embedded number fields.
The source code is available at https://github.com/MarkCBell/realalg.
This module provides conversion from and to ``realalg``.
"""
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

import realalg

import numbers
from .cppyy_eantic import eantic

def poly_string(coeffs, var):
    s = str(coeffs[0])
    for i in range(1, len(coeffs)):
        if not coeffs[i]:
            continue
        s += ' + %s * %s^%s' % (coeffs[i], var, i)
    return s

def realalg_interval_to_arb(i):
    from realalg.interval import Interval
    if not isinstance(i, Interval):
        raise ValueError
    center = float(i.lower + i.upper) / 2.0 / 10.0 ** (i.precision)
    rad = float(i.upper - i.lower) / 10.0 ** (i.precision - 1)
    return '[' + str(center) + ' +/- ' + str(rad) + ']'

def realalg_nf_to_eantic(K, name='a'):
    r"""
    Convert a realalg real number field to eantic

    >>> import realalg
    >>> from pyeantic.realalg_conversion import realalg_nf_to_eantic
    >>> R = realalg.RealNumberField([-2,0,1])  # random output as it prints a deprecation warning in SageMath
    >>> realalg_nf_to_eantic(R)
    NumberField(a^2 - 2, [1.4142135...])
    >>> R = realalg.RealNumberField([-13,3,0,0,1])
    >>> realalg_nf_to_eantic(R)
    NumberField(a^4 + 3*a - 13, [1.679729...])
    """
    if not isinstance(K, realalg.RealNumberField):
        raise ValueError

    p = poly_string(K.coefficients, name)
    emb = realalg_interval_to_arb(K.intervals(15)[1])
    return eantic.renf(p, name, emb)

def realalg_nf_elem_to_eantic(K, elem):
    r"""
    Convert a realalg real number field element to eantic

    >>> import realalg
    >>> from pyeantic.realalg_conversion import realalg_nf_to_eantic, realalg_nf_elem_to_eantic
    >>> R = realalg.RealNumberField([-2,0,1])
    >>> K = realalg_nf_to_eantic(R)
    >>> realalg_nf_elem_to_eantic(K, R([1,-1]))
    (-a+1 ~ -0.41421356)
    """
    if isinstance(elem, numbers.Integral):
        data = int(elem)
    elif isinstance(elem, str):
        data = elem
    elif isinstance(elem, realalg.RealAlgebraic):
        data = poly_string(elem.coefficients, K.gen_name())
    else:
        data = str(elem)

    return eantic.renf_elem(K, data)
