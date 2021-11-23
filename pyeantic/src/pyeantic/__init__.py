r"""
Python Wrappers for E-ANTIC

>>> import pyeantic
"""
######################################################################
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
#####################################################################

from .cppyy_eantic import eantic


def RealEmbeddedNumberField(*args, **kwargs):
    r"""
    A number field with an embedding into the reals.

    Unlike the usual ``NumberField`` in SageMath, this uses exact ball
    arithmetic under the hood which can lead to much better performance in
    typical applications as they arise in exact classical geometry.

    EXAMPLES:

    We can create an ``RealEmbeddedNumberField`` from a ``NumberField`` with an
    embedding::

        sage: from pyeantic import RealEmbeddedNumberField
        sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
        sage: RealEmbeddedNumberField(K)
        Real Embedded Number Field in a with defining polynomial x^2 - 2 with a = 1.414213562373095?

    From an actual embedding::

        sage: K = NumberField(x**2 - 2, 'a')
        sage: RealEmbeddedNumberField(K.embeddings(AA)[0])
        Real Embedded Number Field in a with defining polynomial x^2 - 2 with a = -1.414213562373095?

    From a ``renf_class``::

        sage: from pyeantic import eantic
        sage: K = eantic.renf("a^2 - 2", "a", "1.4 +/- .1")
        sage: RealEmbeddedNumberField(K)
        Real Embedded Number Field in a with defining polynomial x^2 - 2 with a = 1.414213562373095?

    """
    from .real_embedded_number_field import RealEmbeddedNumberField as RENF
    return RENF(*args, **kwargs)
