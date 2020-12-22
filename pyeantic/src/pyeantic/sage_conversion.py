# -*- coding: utf-8 -*-
r"""
Legacy conversions of SageMath Number Fields and their Elements to the real
embedded variants provided by e-antic.
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

def sage_nf_to_eantic(K):
    r"""
    An alias for ``pyeantic.RealEmbeddedNumberField(K).renf``.

    EXAMPLES::

        sage: from pyeantic.sage_conversion import sage_nf_to_eantic
        sage: x = polygen(QQ)
        sage: K = NumberField(x**3 - 3, 'a', embedding=AA(3)**Rational((1,3)))
        sage: L = sage_nf_to_eantic(K)
        sage: L
        NumberField(a^3 - 3, [1.442249570307408382321638310780 +/- 5.49e-31])
        sage: L.gen()
        (a ~ 1.4422496)

    """
    from .real_embedded_number_field import RealEmbeddedNumberField
    return RealEmbeddedNumberField(K).renf

def sage_nf_elem_to_eantic(K, elem):
    r"""
    An alias for ``pyeantic.RealEmbeddedNumberField(K)(elem).renf_elem``

    INPUT:

    - ``K`` (eantic number field): the number field to which the conversion is
      performed
    - ``elem`` (Sage number field element): the element to convert

    EXAMPLES::

        sage: from pyeantic.sage_conversion import sage_nf_to_eantic, sage_nf_elem_to_eantic
        sage: x = polygen(QQ)
        sage: K = NumberField(x**3 - 3, 'a', embedding=AA(3)**Rational((1,3)))
        sage: L = sage_nf_to_eantic(K)
        sage: sage_nf_elem_to_eantic(L, Integer(1))
        1
        sage: sage_nf_elem_to_eantic(L, Rational((2,3)))
        (2/3 ~ 0.66666667)
        sage: sage_nf_elem_to_eantic(L, 'a')
        (a ~ 1.4422496)
        sage: sage_nf_elem_to_eantic(L, K.gen())
        (a ~ 1.4422496)

    """
    from .real_embedded_number_field import RealEmbeddedNumberField
    return RealEmbeddedNumberField(K)(elem).renf_elem
