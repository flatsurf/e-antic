# -*- coding: utf-8 -*-
r"""
Real Embedded Number Fields for SageMath

This wraps e-antic for SageMath providing number fields with less of a focus
on number theory but fast exact ball arithmetic of the kind that is usually
required for classical geometry.
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

import cppyy

from sage.all import QQ, UniqueRepresentation, ZZ, RR, Fields, RBF, AA, Morphism, Hom, SetsWithPartialMaps, NumberField, NumberFields, RealBallField, CommutativeRing
from sage.structure.element import FieldElement
from sage.categories.map import Map

from pyeantic import eantic


class RealEmbeddedNumberFieldElement(FieldElement):
    r"""
    An element of a :class:`RealEmbeddedNumberField`, i.e., a wrapper of
    e-antic's ``renf_elem_class``.

    ..NOTES:

    This class wraps a ``renf_elem_class`` (which, at no additional runtime
    cost wraps a ``renf_elem``.) At the moment it's not possible to use a
    ``renf_elem_class`` directly in SageMath. Changing this might lead to a
    slight improvement in performance.

    EXAMPLES::

        sage: from pyeantic import RealEmbeddedNumberField
        sage: K = NumberField(x^2 - 2, 'a', embedding=sqrt(AA(2)))
        sage: K = RealEmbeddedNumberField(K)
        sage: a = K.gen()

    TESTS::

        sage: TestSuite(a).run()

    Verify that #192 has been resolved::

        sage: R.<x> = QQ[]
        sage: K.<b> = NumberField(x^2 - 2, embedding=sqrt(AA(2)))
        sage: K = RealEmbeddedNumberField(K)
        sage: K(b)
        (b ~ 1.4142136)

    ::

        sage: R.<y> = QQ[]
        sage: K.<b> = NumberField(y^2 - 2, embedding=sqrt(AA(2)))
        sage: K = RealEmbeddedNumberField(K)
        sage: K(b)
        (b ~ 1.4142136)

    """
    def __init__(self, parent, value):
        r"""
        TESTS::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x^2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: a = K.gen()

            sage: from pyeantic.real_embedded_number_field import RealEmbeddedNumberFieldElement
            sage: isinstance(a, RealEmbeddedNumberFieldElement)
            True

        """
        if isinstance(value, cppyy.gbl.eantic.renf_elem_class):
            self.renf_elem = value
        else:
            value = parent.number_field(value)
            self.renf_elem = parent.renf.zero()

            gen_pow = parent.renf.one()
            for coeff in value.polynomial().coefficients(sparse=False):
                self.renf_elem = self.renf_elem + coeff * gen_pow
                gen_pow = gen_pow * parent.renf.gen()

        FieldElement.__init__(self, parent)

    def _add_(self, other):
        r"""
        Return the sum of this element and ``other``.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: K.gen() + 1
            (a+1 ~ 2.4142136)

        """
        return self.parent()(self.renf_elem + other.renf_elem)

    def _sub_(self, other):
        r"""
        Return the difference of this element and ``other``.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: K.gen() - 1
            (a-1 ~ 0.41421356)

        """
        return self.parent()(self.renf_elem - other.renf_elem)

    def _mul_(self, other):
        r"""
        Return the product of this element and ``other``.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: K.gen() * K.gen()
            2

        """
        return self.parent()(self.renf_elem * other.renf_elem)

    def _div_(self, other):
        r"""
        Return the quotient of this element by ``other``.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: 1 / K.gen()
            (1/2*a ~ 0.70710678)

        """
        return self.parent()(self.renf_elem / other.renf_elem)

    def _neg_(self):
        r"""
        Return the negative of this element.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: -K.gen()
            (-a ~ -1.4142136)

        """
        return self.parent()(-self.renf_elem)

    def _repr_(self):
        r"""
        Return a printable representation of this element.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: K.gen()
            (a ~ 1.4142136)

        """
        return repr(self.renf_elem)

    def _richcmp_(self, other, op):
        r"""
        Compare this element and ``other``.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: K.gen() > 0
            True
            sage: K.gen() < 1
            False

        """
        from sage.structure.richcmp import rich_to_bool
        if self.renf_elem < other.renf_elem:
            return rich_to_bool(op, -1)
        elif self.renf_elem == other.renf_elem:
            return rich_to_bool(op, 0)
        else:
            return rich_to_bool(op, 1)

    def _rational_(self):
        r"""
        Convert this element to a rational number, if possible.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: QQ(K.one())
            1
            sage: QQ(K.gen())
            Traceback (most recent call last):
            ...
            TypeError: not a rational number

        TESTS::

            sage: K.one() in QQ
            True
            sage: K.gen() not in QQ
            True

        """
        if not self.renf_elem.is_rational():
            raise TypeError("not a rational number")
        return QQ(str(cppyy.gbl.eantic.cppyy.rational(self.renf_elem)))

    def _integer_(self, *args):
        r"""
        Convert this element to an integer, if possible.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: ZZ(K.one())
            1
            sage: ZZ(K.gen())
            Traceback (most recent call last):
            ...
            TypeError: not an  integer

        TESTS::

            sage: K.one() in ZZ
            True
            sage: K.gen() not in ZZ
            True

        """
        if not self.renf_elem.is_integer():
            raise TypeError("not an integer")
        return QQ(self).numerator()

    def __getstate__(self):
        r"""
        Return picklable data defining this element.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: loads(dumps(K.gen())) == K.gen()
            True

        """
        return (self.parent(), self.parent().number_field(self))

    def __setstate__(self, state):
        r"""
        Restore this element from the unpickled state.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: loads(dumps(K.one())) == K.one()
            True

        """
        self._set_parent(state[0])
        self.renf_elem = self.parent()(state[1]).renf_elem

    def __hash__(self):
        r"""
        Return a hash value of this element.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: hash(K.gen()) == hash(K.gen())
            True

        Note that this hash is not compatible with the corresponding number
        field element::

            sage: hash(K.gen()) == hash(K.number_field(K.gen()))
            False

        """
        return hash(self.renf_elem)

    def vector(self):
        r"""
        Return a vector representation of this element in terms of the basis of
        the number field.

        EXAPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: K.gen().vector()
            (0, 1)

        """
        return self.parent().number_field(self).vector()

    def minpoly(self, var='x'):
        r"""
        Return the minimal polynomial of this element over the rationals.

        EXAPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: K.gen().minpoly()
            x^2 - 2

        """
        return self.parent().number_field(self).minpoly(var)


class RealEmbeddedNumberField(UniqueRepresentation, CommutativeRing):
    r"""
    See ``RealEmbeddedNumberField`` in ``__init__.py`` for details.
    """
    @staticmethod
    def __classcall__(cls, embed, category=None):
        r"""
        Normalize parameters so embedded real number fields are unique::

            sage: from pyeantic import eantic, RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: L = NumberField(x**2 - 2, 'a')
            sage: L = RealEmbeddedNumberField(L.embeddings(AA)[1])
            sage: M = eantic.renf("a^2 - 2", "a", "1.4 +/- .1")
            sage: M = RealEmbeddedNumberField(M)
            sage: K is L
            True
            sage: K is M
            True

        Instead of an embedding, a `renf_class` can be used:

            sage: from pyeantic import eantic, RealEmbeddedNumberField
            sage: K = eantic.renf_class.make("x^2 - 2", "x", "1.4 +/- 1")
            sage: L = RealEmbeddedNumberField(K)

        TESTS:

        Check that #197 has been resolved::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = RealEmbeddedNumberField(QQ).renf
            sage: RealEmbeddedNumberField(K)
            Real Embedded Number Field in x with defining polynomial x - 1 with x = 1

        """
        if 'cppyy.gbl.boost.intrusive_ptr<const eantic::renf_class>' in str(type(embed)):
            embed = embed.get()
        if isinstance(embed, eantic.renf_class):
            # Since it is quite annoying to convert an fmpz polynomial, we parse
            # the printed representation of the renf_class. This is of course
            # not very robust…
            import re
            match = re.match("^NumberField\\(([^,]+), ([^)]+)\\)$", repr(embed))
            assert match, "renf_class printed in an unexpected way: " + repr(embed)
            minpoly = match.group(1)
            root_str = match.group(2)
            match = re.match("^\\d*\\*?([^\\^ *]+)[\\^ +-]", minpoly)
            assert match, "renf_class printed leading coefficient in an unexpected way: " + minpoly
            minpoly = QQ[match.group(1)](minpoly)
            roots = []
            AA_roots = minpoly.roots(AA, multiplicities=False)
            for prec in [53, 64, 128, 256]:
                R = RealBallField(prec)
                root = R(root_str)
                roots = [aa for aa in AA_roots if R(aa).overlaps(root)]
                if len(roots) == 1:
                    break
            if len(roots) != 1:
                raise RuntimeError("cannot distinguish roots with limited ball field precision")
            embed = NumberField(minpoly, minpoly.variable_name(), embedding=roots[0])
        if embed in NumberFields():
            if not RR.has_coerce_map_from(embed):
                raise ValueError("number field must be endowed with an embedding into the reals")
            if not embed.is_absolute():
                raise NotImplementedError("number field must be absolute")
            # We recreate our NumberField from the embedding since number
            # fields with the same embedding might differ by other parameters
            # and therefore do not serve immediately as unique keys.
            embed = AA.coerce_map_from(embed)
        if isinstance(embed, Map):
            K = embed.domain()
            if K not in NumberFields():
                raise ValueError("domain must be a number field")
            if not AA.has_coerce_map_from(embed.codomain()):
                raise ValueError("codomain must coerce into RR")
            if not K.is_absolute():
                raise NotImplementedError("number field must be absolute")
            # We explicitly construct an embedding from the given embedding to
            # make sure that we get a useable key.
            minpoly = (QQ['x'].gen() - 1) if K is QQ else K.polynomial()
            minpoly = minpoly.change_variable_name('x')
            embedding = AA.one() if K is QQ else embed(K.gen())
            embed = NumberField(minpoly, K.variable_name(), embedding=embedding)
        else:
            raise TypeError("cannot build RealEmbeddedNumberField from embedding %s" % (type(embed)))

        category = category or Fields()
        return super(RealEmbeddedNumberField, cls).__classcall__(cls, embed, category)

    def __init__(self, embedded, category=None):
        r"""
        TESTS::

            sage: import pyeantic
            sage: from pyeantic import RealEmbeddedNumberField

            sage: K = RealEmbeddedNumberField(QQ)
            sage: isinstance(K, pyeantic.real_embedded_number_field.RealEmbeddedNumberField)
            True

            sage: TestSuite(K).run()

        ::

            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)

            sage: import pyeantic.real_embedded_number_field
            sage: isinstance(K, pyeantic.real_embedded_number_field.RealEmbeddedNumberField)
            True

            sage: TestSuite(K).run()

        ::

            sage: K = NumberField(x**2 - 2, 'b', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)

            sage: isinstance(K, pyeantic.real_embedded_number_field.RealEmbeddedNumberField)
            True

            sage: TestSuite(K).run()

        ::

            sage: K.an_element() in K.number_field
            True
            sage: K.an_element() in QQbar
            True
            sage: K.number_field.an_element() in K
            True
            sage: 1 in K
            True

        ::

            sage: type(K.one() - K.number_field.one())
            <class 'sage.rings.number_field.number_field_element_quadratic.NumberFieldElement_quadratic'>
            sage: type(K.number_field.one() - K.one())
            <class 'sage.rings.number_field.number_field_element_quadratic.NumberFieldElement_quadratic'>
            sage: type(K.one() - 1)
            <class 'pyeantic.real_embedded_number_field.RealEmbeddedNumberField_with_category.element_class'>
            sage: type(1 - K.one())
            <class 'pyeantic.real_embedded_number_field.RealEmbeddedNumberField_with_category.element_class'>

        """
        self.number_field = embedded
        var = self.number_field.variable_name()
        self.renf = eantic.renf(
            repr(self.number_field.polynomial().change_variable_name(var)),
            var,
            str(RBF(self.number_field.gen())))

        CommutativeRing.__init__(self, QQ, category=category)

        # It is usually not a good idea to introduce cycles in the coercion
        # framework: when performing a + b with a in A and b in B, it's a bit
        # random whether the result will be in A or in B. So we make the map
        # into the SageMath number field a coercion since it makes lots of
        # stuff work that depends on having coercions to other number fields
        # and AA, QQbar, …. We register this coercion as an embedding so
        # transitive coercions are detected by the coercion framework such as
        # the coercion into AA.
        self.register_embedding(CoercionNumberFieldRenf(self))

        self.register_conversion(self.number_field)

        # Allow coercion of rationals and integers so arithmetic with integers
        # works on the SageMath prompt.
        self.register_coercion(QQ)

    def random_element(self):
        r"""
        Return a randomly generated element in this number field.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: K.random_element().parent() is K
            True

        """
        return self(self.number_field.random_element())

    def _repr_(self):
        r"""
        Return a printable representation of this number field.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: RealEmbeddedNumberField(K)
            Real Embedded Number Field in a with defining polynomial x^2 - 2 with a = 1.414213562373095?

        """
        return "Real Embedded %r"%(self.number_field,)

    def characteristic(self):
        r"""
        Return zero, the  characteristic of this number field.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: RealEmbeddedNumberField(K).characteristic()
            0

        """
        return ZZ(0)

    def an_element(self):
        r"""
        Return a typical element in this number field.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: K.an_element()
            (a ~ 1.4142136)

        """
        return self(self.number_field.an_element())

    def gen(self):
        r"""
        Return the generator of this number field, i.e., a root of its defining
        polynomial.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: K.gen()
            (a ~ 1.4142136)

        """
        return self(self.number_field.gen())

    def degree(self):
        r"""
        Return the absolute degree of this number field.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: K.degree()
            2

        """
        return self.number_field.degree()

    def is_field(self, *args, **kwargs):
        r"""
        Return whether this number field is a field, i.e., return ``True``.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: K.is_field()
            True

        """
        return True

    def __pow__(self, n, _ = None):
        r"""
        Return the vector space of dimension ``n`` over this field.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: K = RealEmbeddedNumberField(K)
            sage: K^3
            Vector space of dimension 3 over Real Embedded Number Field in a with defining polynomial x^2 - 2 with a = 1.414213562373095?

        """
        if isinstance(n, tuple):
            m, n = n
            from sage.all import MatrixSpace
            return MatrixSpace(self, m, n)
        else:
            from sage.all import VectorSpace
            return VectorSpace(self, n)

    Element = RealEmbeddedNumberFieldElement


class CoercionNumberFieldRenf(Morphism):
    r"""
    A coercion from :class:`RealEmbeddedNumberField` to a SageMath
    ``NumberField``.

    EXAMPLES::

        sage: from pyeantic import RealEmbeddedNumberField
        sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
        sage: KK = RealEmbeddedNumberField(K)
        sage: coercion = K.convert_map_from(KK)

    TESTS::

        sage: from pyeantic.real_embedded_number_field import CoercionNumberFieldRenf
        sage: isinstance(coercion, CoercionNumberFieldRenf)
        True

    """
    def __init__(self, domain):
        Morphism.__init__(self, Hom(domain, domain.number_field, SetsWithPartialMaps()))

    def _call_(self, x):
        r"""
        Convert ``x`` to the codomain.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: KK = RealEmbeddedNumberField(K)
            sage: a = KK.an_element()
            sage: K(a)
            a

        TESTS:

        Verify that the name of the generator is not relevant::

            sage: K = NumberField(x**2 - 2, 'b', embedding=sqrt(AA(2)))
            sage: KK = RealEmbeddedNumberField(K)
            sage: b = KK.an_element()
            sage: K(b)
            b

        """
        rational_coefficients = [ZZ(str(c.get_str())) / ZZ(str(x.renf_elem.den().get_str())) for c in x.renf_elem.num_vector()]
        while len(rational_coefficients) < self.domain().number_field.degree():
            rational_coefficients.append(QQ.zero())
        return self.codomain()(rational_coefficients)

    def section(self):
        r"""
        Return the inverse of this coercion.

        EXAMPLES::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: KK = RealEmbeddedNumberField(K)
            sage: K.coerce_map_from(KK).section()
            Conversion map:
              From: Number Field in a with defining polynomial x^2 - 2 with a = 1.414213562373095?
              To:   Real Embedded Number Field in a with defining polynomial x^2 - 2 with a = 1.414213562373095?

        """
        return self.domain().convert_map_from(self.codomain())
