r"""
Make e-antic accessible from Python through cppyy

EXAMPLES::

    >>> from pyeantic import eantic
    >>> K = eantic.renf("x^2 - 2", "x", "[1.4 +/- 1]")
    >>> x = eantic.renf_elem(K, "x"); x
    (x ~ 1.4142136)
    >>> x + 2
    (x+2 ~ 3.4142136)

TESTS:

Test that objects can be pickled::

    >>> from pickle import loads, dumps
    >>> loads(dumps(x)) == x
    True
    >>> loads(dumps(x.parent())) == x.parent()
    True

Test that we can recover from an invalid field definition. (This works equally
in Python and SageMath but pytest handles the signal differently which makes it
easier to test this in a SageMath doctest.)::

    sage: from pyeantic import eantic
    sage: K = eantic.renf("x^2 - 3", "x", "1.22 +/- 0.1")
    Traceback (most recent call last):
    ...
    TypeError: ...

"""
# -*- coding: utf-8 -*-
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

import os
import cppyy

from cppyythonizations.printing import enable_pretty_printing
from cppyythonizations.pickling.cereal import enable_cereal
from cppyythonizations.operators.order import enable_total_order
from cppyythonizations.util import filtered

cppyy.py.add_pythonization(enable_pretty_printing, "eantic")

# Make sure that the operators <, >, <=, >=, ==, != are available.
cppyy.py.add_pythonization(filtered('renf_elem_class')(enable_total_order), "eantic")

def enable_arithmetic(proxy, name):
    r"""
    Make sure arithmetic works with SageMath types.
    """
    for (op, infix) in [('add', '+'), ('sub', '-'), ('mul', '*'), ('truediv', '/')]:
        python_op = "__%s__" % (op,)
        python_rop = "__r%s__" % (op,)

        implementation = getattr(cppyy.gbl.eantic.cppyy, op)
        def binary(lhs, rhs, implementation=implementation):
            lhs, rhs = for_eantic(lhs), for_eantic(rhs)
            return implementation[type(lhs), type(rhs)](lhs, rhs)
        def rbinary(rhs, lhs, implementation=implementation):
            lhs, rhs = for_eantic(lhs), for_eantic(rhs)
            return implementation[type(lhs), type(rhs)](lhs, rhs)

        setattr(proxy, python_op, binary)
        setattr(proxy, python_rop, rbinary)

    setattr(proxy, "__neg__", lambda self: cppyy.gbl.eantic.cppyy.neg(self))
    setattr(proxy, "__pow__", lambda self, n: cppyy.gbl.eantic.pow(self, n))

# Make sure that the operators +, -, *, /, ** are available and work with SageMath arguments.
cppyy.py.add_pythonization(filtered("renf_elem_class")(enable_arithmetic), "eantic")

def enable_intrusive_serialization(proxy, name):
    r"""
    Enable seralization for an eantic::renf_class& as returned by
    renf_class::parent().
    """
    def reduce(self):
        cppyy.include('e-antic/cereal.hpp')
        return (intrusive_ptr_deserialize, (cppyy.gbl.boost.intrusive_ptr['const eantic::renf_class'](self),))
    proxy.__reduce__ = reduce

def unwrap_intrusive_ptr(K):
    if isinstance(K, eantic.renf_class):
        K = cppyy.gbl.boost.intrusive_ptr['const eantic::renf_class'](K)
    if isinstance(K, cppyy.gbl.boost.intrusive_ptr['const eantic::renf_class']):
        K = K.get()
        K.__intrusive__ = K
    return K

def intrusive_ptr_deserialize(intrusive):
    cppyy.include('e-antic/cereal.hpp')
    return unwrap_intrusive_ptr(intrusive)

cppyy.py.add_pythonization(filtered('renf_elem_class')(lambda proxy, name: enable_cereal(proxy, name, ["e-antic/cereal.hpp"])), "eantic")
cppyy.py.add_pythonization(filtered('intrusive_ptr<const eantic::renf_class>')(lambda proxy, name: enable_cereal(proxy, name, ["e-antic/cereal.hpp"])), "boost")
cppyy.py.add_pythonization(filtered("renf_class")(enable_intrusive_serialization), "eantic")

for path in os.environ.get('PYEANTIC_INCLUDE','').split(':'):
    if path: cppyy.add_include_path(path)

cppyy.include("e-antic/cppyy.hpp")

from cppyy.gbl import eantic, boost

eantic.renf = lambda *args: unwrap_intrusive_ptr(eantic.renf_class.make(*args))
eantic.renf_class.make.__sig2exc__ = True

def for_eantic(x):
    r"""
    Attempt to convert ``x`` from something that SageMath understand to
    something that the constructor of renf_elem_class understands.

    Typically, this is the conversion of a SageMath Integer to a mpz_class and
    such.

    If no such conversion exists, leave the argument unchanged.
    """
    if isinstance(x, (int, eantic.renf_elem_class, cppyy.gbl.mpz_class, cppyy.gbl.mpq_class)):
        return x
    if isinstance(x, (tuple, list)):
        x = [for_eantic(v) for v in x]
        if not all([isinstance(v, (int, cppyy.gbl.mpz_class, cppyy.gbl.mpq_class)) for v in x]):
            raise TypeError("Coefficients must be convertible to mpq")
        x = [cppyy.gbl.mpq_class(v) for v in x]
        x = cppyy.gbl.std.vector[cppyy.gbl.mpq_class](x)
    if hasattr(x, '__mpq__'):
        x = x.__mpq__()
    if hasattr(x, '__mpz__'):
        x = x.__mpz__()

    try:
        from gmpy2 import mpz, mpq
    except ModuleNotFoundError:
        return x

    if isinstance(x, mpz):
        # we need std.string, or cppyy resolves to the wrong constructor:
        # https://bitbucket.org/wlav/cppyy/issues/127/string-argument-resolves-incorrectly
        x = cppyy.gbl.mpz_class(cppyy.gbl.std.string(str(x)))
    if isinstance(x, mpq):
        # we need std.string, or cppyy resolves to the wrong constructor:
        # https://bitbucket.org/wlav/cppyy/issues/127/string-argument-resolves-incorrectly
        x = cppyy.gbl.mpq_class(cppyy.gbl.std.string(str(x)))
    return x

# cppyy is confused by template resolution, see
# https://bitbucket.org/wlav/cppyy/issues/119/templatized-constructor-is-ignored
# and https://github.com/flatsurf/pyeantic/issues/10
def make_renf_elem_class(*args):
    if len(args) > 2:
        raise NotImplementedError("can not create renf_elem_class from more than two arguments")

    if len(args) == 0:
        return eantic.renf_elem_class()

    K = args[0]
    K = unwrap_intrusive_ptr(K)

    if len(args) == 1:
        if isinstance(K, eantic.renf_class):
            return eantic.renf_elem_class(K)
        else:
            K, v = (None, K)
    else:
        v = args[1]

    if K is None:
        return eantic.renf_elem_class(for_eantic(v))
    else:
        return eantic.renf_elem_class(K, for_eantic(v))


eantic.renf_elem = make_renf_elem_class
