/*
    Copyright (C) 2018 Vincent Delecroix
    Copyright (C) 2019-2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <cassert>
#include <flint/fmpq.h>
#include <cstdlib>
#include <functional>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

#include "../e-antic/renfxx.h"
#include "../e-antic/fmpq_poly_extra.h"

namespace eantic {

namespace {

// Return the value of `a` as an integer (without allocating any memory.)
const fmpz_t& renf_elem_get_fmpz(const renf_elem_t a, const renf_t nf)
{
    if (nf->nf->flag & NF_LINEAR)
        return LNF_ELEM_NUMREF(a->elem);
    else if (nf->nf->flag & NF_QUADRATIC)
    {
        // Elements in a quadratic number field are stored as a denominator and
        // two (actually three) coefficients for the numerator. The first
        // coefficient of the numerator is the degree 0 entry (the denominator
        // is 1 for integers.)
        return reinterpret_cast<const fmpz_t&>(QNF_ELEM_NUMREF(a->elem)[0]);
    }
    else if (fmpq_poly_length(NF_ELEM(a->elem)) == 0)
    {
        // The zero element in a general number field is stored as a degree 0
        // polynomial, so we cannot reference any fmpz_t in that case.
        static fmpz_t zero = {0};
        return zero;
    }
    else
    {
        // As in the quadratic case, we return the degree 0 entry.
        return reinterpret_cast<const fmpz_t&>(NF_ELEM_NUMREF(a->elem)[0]);
    }
}

// Return the value of `a` as a rational (without allocating any memory.)
const fmpq_t& renf_elem_get_fmpq(fmpq_t buffer, const renf_elem_t a, const renf_t nf)
{
    if (nf->nf->flag & NF_LINEAR)
    {
        // A linear element is a pair (fmpz_t, fmpz_t) exactly like an fmpq_t.
        return reinterpret_cast<const fmpq_t&>(LNF_ELEM(a->elem));
    }
    else
    {
        // In the general case, we use an external `buffer` to store the value
        // as an fmpq_t.
        nf_elem_get_coeff_fmpq(buffer, a->elem, 0, nf->nf);
        return reinterpret_cast<const fmpq_t&>(buffer[0]);
    }
}

void coerce(renf_elem_class& self, const renf_class& parent)
{
    if (parent == self.parent())
        return;

    if (self.is_zero())
    {
        self = parent.zero();
    }
    else if (self.is_integer())
    {
        fmpz_t value;
        fmpz_init_set(value, renf_elem_get_fmpz(self.renf_elem_t(), self.parent().renf_t()));

        self = parent.zero();

        renf_elem_set_fmpz(self.renf_elem_t(), value, self.parent().renf_t());

        fmpz_clear(value);
    }
    else if (self.is_rational())
    {
        self = renf_elem_class(parent, self);
    }
    else
        throw std::logic_error("not implemented: coercion of non-rational elements");
}

template <
  void renf_op(renf_elem_t, const renf_elem_t, const renf_elem_t, const renf_t),
  void fmpz_op(renf_elem_t, const renf_elem_t, const fmpz_t, const renf_t),
  void fmpq_op(renf_elem_t, const renf_elem_t, const fmpq_t, const renf_t)
>
renf_elem_class& binop(renf_elem_class& lhs, const renf_elem_class& rhs)
{
    if (lhs.parent() == rhs.parent())
        renf_op(lhs.renf_elem_t(), lhs.renf_elem_t(), rhs.renf_elem_t(), lhs.parent().renf_t());
    else if (rhs.is_integer())
        fmpz_op(lhs.renf_elem_t(), lhs.renf_elem_t(), renf_elem_get_fmpz(rhs.renf_elem_t(), rhs.parent().renf_t()), lhs.parent().renf_t());
    else if (rhs.is_rational())
    {
        fmpq_t buffer;
        fmpq_init(buffer);
        fmpq_op(lhs.renf_elem_t(), lhs.renf_elem_t(), renf_elem_get_fmpq(buffer, rhs.renf_elem_t(), rhs.parent().renf_t()), lhs.parent().renf_t());
        fmpq_clear(buffer);
    }
    else
    {
        coerce(lhs, rhs.parent());
        binop<renf_op, fmpz_op, fmpq_op>(lhs, rhs);
    }

    return lhs;
}

template <void op(renf_elem_t, const renf_elem_t, const fmpz_t, const renf_t)>
renf_elem_class& binop_mpz(renf_elem_class& lhs, const mpz_class& rhs)
{
    ::fmpz_t r;
    fmpz_init_set_readonly(r, rhs.get_mpz_t());
    op(lhs.renf_elem_t(), lhs.renf_elem_t(), r, lhs.parent().renf_t());
    fmpz_clear_readonly(r);
    return lhs;
}

bool relop_mpz(const renf_elem_class& lhs, const mpz_class& rhs, const int cmp)
{
    if (cmp == 0 && !lhs.is_integer())
        return false;

    bool ret;

    ::fmpz_t r;
    fmpz_init_set_readonly(r, rhs.get_mpz_t());
    if (cmp == 0)
        ret = renf_elem_equal_fmpz(lhs.renf_elem_t(), r, lhs.parent().renf_t());
    else
        ret = renf_elem_cmp_fmpz(lhs.renf_elem_t(), r, lhs.parent().renf_t()) == cmp;
    fmpz_clear_readonly(r);

    return ret;
}

template <void op(renf_elem_t, const renf_elem_t, const fmpq_t, const renf_t)>
renf_elem_class& binop_mpq(renf_elem_class& lhs, const mpq_class& rhs)
{
    ::fmpq_t r;
    fmpq_init_set_readonly(r, rhs.get_mpq_t());
    op(lhs.renf_elem_t(), lhs.renf_elem_t(), r, lhs.parent().renf_t());
    fmpq_clear_readonly(r);

    return lhs;
}

bool relop_mpq(const renf_elem_class& lhs, const mpq_class& rhs, const int cmp)
{
    if (cmp == 0 && !lhs.is_rational())
        return false;

    bool ret;

    ::fmpq_t r;
    fmpq_init_set_readonly(r, rhs.get_mpq_t());
    if (cmp == 0)
        ret = renf_elem_equal_fmpq(lhs.renf_elem_t(), r, lhs.parent().renf_t());
    else
        ret = renf_elem_cmp_fmpq(lhs.renf_elem_t(), r, lhs.parent().renf_t()) == cmp;
    fmpq_clear_readonly(r);

    return ret;
}

template <typename Integer>
using Supported = std::conditional_t<std::is_signed<Integer>::value, slong, ulong>;

template <typename Integer>
void maybe_fmpz(Integer value, const std::function<void(Supported<Integer>)>& op, const std::function<void(const fmpz_t)>& fmpz_op)
{
     try
     {
         op(boost::numeric_cast<Supported<Integer>>(value));
     }
     catch (boost::bad_numeric_cast &)
     {
        mpz_class vv(boost::lexical_cast<std::string>(value));
        fmpz_t v;
        fmpz_init_set_readonly(v, vv.get_mpz_t());
        fmpz_op(v);
        fmpz_clear_readonly(v);
     }
}

template <typename Integer>
void assign_maybe_fmpz(renf_elem_class& lhs, Integer value, const std::function<void(renf_elem_t, Supported<Integer>, const renf_t)>& op)
{
    maybe_fmpz(value,
        [&](auto v) { op(lhs.renf_elem_t(), v, lhs.parent().renf_t()); },
        [&](const fmpz_t v) { renf_elem_set_fmpz(lhs.renf_elem_t(), v, lhs.parent().renf_t()); });
}

template <typename Integer>
renf_elem_class & binop_maybe_fmpz(renf_elem_class& lhs, Integer rhs, const std::function<void(renf_elem_t, const renf_elem_t, Supported<Integer>, const renf_t)>& op, const std::function<void(renf_elem_t, const renf_elem_t, const fmpz_t, const renf_t)>& fmpz_op)
{
    maybe_fmpz(rhs,
        [&](auto v) { op(lhs.renf_elem_t(), lhs.renf_elem_t(), v, lhs.parent().renf_t()); },
        [&](const fmpz_t v) { fmpz_op(lhs.renf_elem_t(), lhs.renf_elem_t(), v, lhs.parent().renf_t()); });
    return lhs;
}

template <typename Integer>
bool relop_maybe_fmpz(const renf_elem_class& lhs, Integer rhs, const std::function<int(renf_elem_t, Supported<Integer>, renf_t)>& op)
{
    if (!lhs.is_integer())
        return false;

    bool ret;

    maybe_fmpz(rhs,
        [&](auto v) { ret = op(lhs.renf_elem_t(), v, lhs.parent().renf_t()); },
        [&](const fmpz_t v) { ret = renf_elem_equal_fmpz(lhs.renf_elem_t(), v, lhs.parent().renf_t()); });

    return ret;
}

template <typename Integer>
bool relop_maybe_fmpz(const renf_elem_class& lhs, Integer rhs, const std::function<int(renf_elem_t, Supported<Integer>, renf_t)>& op, int cmp)
{
    bool ret;

    maybe_fmpz(rhs,
        [&](auto v) { ret = op(lhs.renf_elem_t(), v, lhs.parent().renf_t()) == cmp; },
        [&](const fmpz_t v) { ret = renf_elem_cmp_fmpz(lhs.renf_elem_t(), v, lhs.parent().renf_t()) == cmp; });

    return ret;
}

}

renf_elem_class::renf_elem_class() noexcept
    : renf_elem_class(renf_class::make())
{
}

renf_elem_class::renf_elem_class(const renf_elem_class & value)
    : renf_elem_class(value.parent(), value) {}

renf_elem_class::renf_elem_class(renf_elem_class && value) noexcept
    : nf(std::move(value.nf))
{
    *a = *value.a;
}

renf_elem_class::renf_elem_class(int value)
    : renf_elem_class(renf_class::make(), value) {}

renf_elem_class::renf_elem_class(unsigned int value)
    : renf_elem_class(renf_class::make(), value) {}

renf_elem_class::renf_elem_class(long value)
    : renf_elem_class(renf_class::make(), value) {}

renf_elem_class::renf_elem_class(unsigned long value)
    : renf_elem_class(renf_class::make(), value) {}

renf_elem_class::renf_elem_class(long long value)
    : renf_elem_class(renf_class::make(), value) {}

renf_elem_class::renf_elem_class(unsigned long long value)
    : renf_elem_class(renf_class::make(), value) {}

renf_elem_class::renf_elem_class(const mpz_class & value)
    : renf_elem_class(renf_class::make(), value) {}

renf_elem_class::renf_elem_class(const mpq_class & value)
    : renf_elem_class(renf_class::make(), value) {}

renf_elem_class::renf_elem_class(const ::fmpz_t value)
    : renf_elem_class(renf_class::make(), value) {}

renf_elem_class::renf_elem_class(const ::fmpq_t value)
    : renf_elem_class(renf_class::make(), value) {}

renf_elem_class::renf_elem_class(const renf_class& k)
    : nf(&k)
{
    renf_elem_init(a, nf->renf_t());
    renf_elem_zero(a, nf->renf_t());
}

renf_elem_class::renf_elem_class(const renf_class& k, int value)
    : renf_elem_class(k, static_cast<long>(value)) {}

renf_elem_class::renf_elem_class(const renf_class& k, unsigned int value)
    : renf_elem_class(k, static_cast<unsigned long>(value)) {}

renf_elem_class::renf_elem_class(const renf_class& k, long value)
    : nf(&k)
{
    renf_elem_init(a, nf->renf_t());
    renf_elem_set_si(a, value, nf->renf_t());
}

renf_elem_class::renf_elem_class(const renf_class& k, unsigned long value)
    : nf(&k)
{
    renf_elem_init(a, nf->renf_t());
    renf_elem_set_ui(a, value, nf->renf_t());
}

renf_elem_class::renf_elem_class(const renf_class& k, long long value)
    : nf(&k)
{
    renf_elem_init(a, nf->renf_t());
    assign_maybe_fmpz(*this, value, renf_elem_set_si);
}

renf_elem_class::renf_elem_class(const renf_class& k, unsigned long long value)
    : nf(&k)
{
    renf_elem_init(a, nf->renf_t());
    assign_maybe_fmpz(*this, value, renf_elem_set_ui);
}

renf_elem_class::renf_elem_class(const renf_class& k, const mpz_class & value)
    : nf(&k)
{
    renf_elem_init(a, nf->renf_t());
    renf_elem_set_mpz(a, value.get_mpz_t(), nf->renf_t());
}

renf_elem_class::renf_elem_class(const renf_class& k, const mpq_class & value)
    : nf(&k)
{
    renf_elem_init(a, nf->renf_t());
    renf_elem_set_mpq(a, value.get_mpq_t(), nf->renf_t());
}

renf_elem_class::renf_elem_class(const renf_class& k, const ::fmpz_t value)
    : nf(&k)
{
    renf_elem_init(a, nf->renf_t());
    renf_elem_set_fmpz(a, value, nf->renf_t());
}

renf_elem_class::renf_elem_class(const renf_class& k, const ::fmpq_t value)
    : nf(&k)
{
    renf_elem_init(a, nf->renf_t());
    renf_elem_set_fmpq(a, value, nf->renf_t());
}

renf_elem_class::renf_elem_class(const renf_class& k, const std::string & str)
    : nf(&k)
{
    const char * s = str.c_str();

    int err;
    const char * i = strchr(s, '~');
    char * t;

    if (i != nullptr)
    {
        ulong len = static_cast<ulong>(i - s);
        t = static_cast<char *>(flint_malloc((len + 1) * sizeof(char)));
        strncpy(t, s, len);
        t[len] = '\0';
    }
    else
    {
        t = static_cast<char *>(flint_malloc((strlen(s) + 1) * sizeof(char)));
        strcpy(t, s);
    }

    fmpq_poly_t p;

    fmpq_poly_init(p);
    err = fmpq_poly_set_str_pretty(p, t, nf->gen_name().c_str());
    if (err)
    {
        fmpq_poly_clear(p);
        throw std::invalid_argument("renf_elem_class fmpq_poly_set_str_pretty");
    }
    renf_elem_init(a, nf->renf_t());
    renf_elem_set_fmpq_poly(a, p, nf->renf_t());
    fmpq_poly_clear(p);

    flint_free(t);
}

renf_elem_class::renf_elem_class(const renf_class& k, const renf_elem_class & value)
    : nf(&k) {
    renf_elem_init(a, nf->renf_t());

    if (value.parent() == parent())
        renf_elem_set(a, value.renf_elem_t(), nf->renf_t());
    else if (value.is_rational())
        renf_elem_set_mpq(a, static_cast<mpq_class>(value).get_mpq_t(), nf->renf_t());
    else
        throw std::logic_error("not implemented: coercion between these number fields");
}

renf_elem_class::renf_elem_class(const renf_class& k, const std::vector<int> & coefficients)
    : renf_elem_class(k)
{
    assert(static_cast<slong>(coefficients.size()) <= nf->degree() &&
        "can not assign renf_elem_class from vector whose size exceeds number field degree");

    fmpq_poly_t p;
    fmpq_poly_init(p);
    for (size_t i = 0; i < coefficients.size(); i++)
        fmpq_poly_set_coeff_si(p, static_cast<slong>(i), coefficients[i]);

    renf_elem_set_fmpq_poly(a, p, nf->renf_t());
    fmpq_poly_clear(p);
}

renf_elem_class::renf_elem_class(const renf_class& k, const std::vector<unsigned int> & coefficients)
    : renf_elem_class(k)
{
    assert(static_cast<slong>(coefficients.size()) <= nf->degree() &&
        "can not assign renf_elem_class from vector whose size exceeds number field degree");

    fmpq_poly_t p;
    fmpq_poly_init(p);
    for (size_t i = 0; i < coefficients.size(); i++)
        fmpq_poly_set_coeff_ui(p, static_cast<slong>(i), coefficients[i]);

    renf_elem_set_fmpq_poly(a, p, nf->renf_t());
    fmpq_poly_clear(p);
}

renf_elem_class::renf_elem_class(const renf_class& k, const std::vector<long> & coefficients)
    : renf_elem_class(k)
{
    assert(static_cast<slong>(coefficients.size()) <= nf->degree() &&
        "can not assign renf_elem_class from vector whose size exceeds number field degree");

    fmpq_poly_t p;
    fmpq_poly_init(p);
    for (size_t i = 0; i < coefficients.size(); i++)
        fmpq_poly_set_coeff_si(p, static_cast<slong>(i), coefficients[i]);

    renf_elem_set_fmpq_poly(a, p, nf->renf_t());
    fmpq_poly_clear(p);
}

renf_elem_class::renf_elem_class(const renf_class& k, const std::vector<unsigned long> & coefficients)
    : renf_elem_class(k)
{
    assert(static_cast<slong>(coefficients.size()) <= nf->degree() &&
        "can not assign renf_elem_class from vector whose size exceeds number field degree");

    fmpq_poly_t p;
    fmpq_poly_init(p);
    for (size_t i = 0; i < coefficients.size(); i++)
        fmpq_poly_set_coeff_ui(p, static_cast<slong>(i), coefficients[i]);

    renf_elem_set_fmpq_poly(a, p, nf->renf_t());
    fmpq_poly_clear(p);
}

renf_elem_class::renf_elem_class(const renf_class& k, const std::vector<mpz_class> & coefficients)
    : renf_elem_class(k)
{
    assert(static_cast<slong>(coefficients.size()) <= nf->degree() &&
        "can not assign renf_elem_class from vector whose size exceeds number field degree");

    fmpq_poly_t p;
    fmpq_poly_init(p);
    for (size_t i = 0; i < coefficients.size(); i++)
        fmpq_poly_set_coeff_mpz(p, static_cast<slong>(i), coefficients[i].__get_mp());

    renf_elem_set_fmpq_poly(a, p, nf->renf_t());
    fmpq_poly_clear(p);
}

renf_elem_class::renf_elem_class(const renf_class& k, const std::vector<mpq_class> & coefficients)
    : renf_elem_class(k)
{
    assert(static_cast<slong>(coefficients.size()) <= nf->degree() &&
        "can not assign renf_elem_class from vector whose size exceeds number field degree");

    fmpq_poly_t p;
    fmpq_poly_init(p);
    for (size_t i = 0; i < coefficients.size(); i++)
        fmpq_poly_set_coeff_mpq(p, static_cast<slong>(i), coefficients[i].__get_mp());

    renf_elem_set_fmpq_poly(a, p, nf->renf_t());
    fmpq_poly_clear(p);
}

renf_elem_class::~renf_elem_class() noexcept
{
    // When this element has been moved out by the move-constructor, then nf is
    // null and a points to another element's storage.
    if (nf) renf_elem_clear(a, nf->renf_t());
}

renf_elem_class & renf_elem_class::operator=(int value)
{
    return *this = static_cast<long>(value);
}

renf_elem_class & renf_elem_class::operator=(unsigned int value)
{
    return *this = static_cast<unsigned long>(value);
}

renf_elem_class & renf_elem_class::operator=(long value)
{
    if (nf != &renf_class::make()) {
        if (nf) renf_elem_clear(a, nf->renf_t());
        nf = &renf_class::make();
        renf_elem_init(a, nf->renf_t());
    }
    renf_elem_set_si(a, value, nf->renf_t());

    return *this;
}

renf_elem_class & renf_elem_class::operator=(unsigned long value)
{
    if (nf != &renf_class::make()) {
        if (nf) renf_elem_clear(a, nf->renf_t());
        nf = &renf_class::make();
        renf_elem_init(a, nf->renf_t());
    }
    renf_elem_set_ui(a, value, nf->renf_t());

    return *this;
}

renf_elem_class & renf_elem_class::operator=(long long value)
{
    if (nf != &renf_class::make()) {
        if (nf) renf_elem_clear(a, nf->renf_t());
        nf = &renf_class::make();
        renf_elem_init(a, nf->renf_t());
    }
    assign_maybe_fmpz(*this, value, renf_elem_set_si);

    return *this;
}

renf_elem_class & renf_elem_class::operator=(unsigned long long value)
{
    if (nf != &renf_class::make()) {
        if (nf) renf_elem_clear(a, nf->renf_t());
        nf = &renf_class::make();
        renf_elem_init(a, nf->renf_t());
    }
    assign_maybe_fmpz(*this, value, renf_elem_set_ui);

    return *this;
}

renf_elem_class & renf_elem_class::operator=(const mpz_class& value)
{
    if (nf != &renf_class::make()) {
        if (nf) renf_elem_clear(a, nf->renf_t());
        nf = &renf_class::make();
        renf_elem_init(a, nf->renf_t());
    }
    renf_elem_set_mpz(a, value.get_mpz_t(), nf->renf_t());

    return *this;
}

renf_elem_class & renf_elem_class::operator=(const mpq_class& value)
{
    if (nf != &renf_class::make()) {
        if (nf) renf_elem_clear(a, nf->renf_t());
        nf = &renf_class::make();
        renf_elem_init(a, nf->renf_t());
    }
    renf_elem_set_mpq(a, value.get_mpq_t(), nf->renf_t());

    return *this;
}

renf_elem_class & renf_elem_class::operator=(const fmpz_t value)
{
    if (nf != &renf_class::make()) {
        if (nf) renf_elem_clear(a, nf->renf_t());
        nf = &renf_class::make();
        renf_elem_init(a, nf->renf_t());
    }
    renf_elem_set_fmpz(a, value, nf->renf_t());

    return *this;
}

renf_elem_class & renf_elem_class::operator=(const fmpq_t value)
{
    if (nf != &renf_class::make()) {
        if (nf) renf_elem_clear(a, nf->renf_t());
        nf = &renf_class::make();
        renf_elem_init(a, nf->renf_t());
    }
    renf_elem_set_fmpq(a, value, nf->renf_t());

    return *this;
}

renf_elem_class & renf_elem_class::operator=(const renf_elem_class & value)
{
    if (nf != value.nf) {
        if (nf) renf_elem_clear(a, nf->renf_t());
        nf = value.nf;
        renf_elem_init(a, nf->renf_t());
    }
    renf_elem_set(a, value.a, nf->renf_t());

    return *this;
}

renf_elem_class & renf_elem_class::operator=(renf_elem_class && value) noexcept
{
    if (nf) {
        using std::swap;
        swap(*this, value);
    } else {
        *a = *value.a;
        nf = std::move(value.nf);
    }

    return *this;
}

bool renf_elem_class::is_zero() const
{
    return renf_elem_is_zero(a, nf->renf_t());
}

bool renf_elem_class::is_one() const
{
    return renf_elem_is_one(a, nf->renf_t());
}

bool renf_elem_class::is_integer() const
{
    return renf_elem_is_integer(a, nf->renf_t());
}

bool renf_elem_class::is_rational() const
{
    return renf_elem_is_rational(a, nf->renf_t());
}

::renf_elem_t & renf_elem_class::renf_elem_t() const
{
    return a;
}

mpz_class renf_elem_class::num() const {
    mpz_class x;

    if (nf->renf_t()->nf->flag & NF_LINEAR)
        fmpz_get_mpz(x.__get_mp(), LNF_ELEM_NUMREF(a->elem));
    else if (nf->renf_t()->nf->flag & NF_QUADRATIC)
    {
        assert(fmpz_is_zero(QNF_ELEM_NUMREF(a->elem) + 1) && "renf_elem_class not a rational");
        fmpz_get_mpz(x.__get_mp(), QNF_ELEM_NUMREF(a->elem));
    }
    else
    {
        if (fmpq_poly_length(NF_ELEM(a->elem)) == 0)
            mpz_set_si(x.__get_mp(), 0);
        else
        {
            assert(fmpq_poly_length(NF_ELEM(a->elem)) == 1 && "renf_elem_class not a rational");
            fmpz_get_mpz(x.__get_mp(), NF_ELEM_NUMREF(a->elem));
        }
    }

    return x;
}

mpz_class renf_elem_class::num_content() const {
    mpz_class x;

    if (nf->renf_t()->nf->flag & NF_LINEAR)
    {
        fmpz_get_mpz(x.__get_mp(), LNF_ELEM_NUMREF(a->elem));
        if (fmpz_sgn(LNF_ELEM_NUMREF(a->elem)) < 0)
            mpz_neg(x.__get_mp(), x.__get_mp());
    }
    else if (nf->renf_t()->nf->flag & NF_QUADRATIC)
    {
        fmpz_t t;
        fmpz_init(t);
        fmpz_gcd(t, QNF_ELEM_NUMREF(a->elem), QNF_ELEM_NUMREF(a->elem) + 1);
        fmpz_get_mpz(x.__get_mp(), t);
        fmpz_clear(t);
    }
    else
    {
        fmpz_t t;
        fmpz_init(t);
        _fmpz_poly_content(t, NF_ELEM(a->elem)->coeffs, NF_ELEM(a->elem)->length);
        fmpz_get_mpz(x.__get_mp(), t);
        fmpz_clear(t);
    }

    return x;
}

mpz_class renf_elem_class::den() const {
    mpz_class res;

    fmpz_t z;
    fmpz_init(z);
    nf_elem_get_den(z, a->elem, nf->renf_t()->nf);
    fmpz_get_mpz(res.__get_mp(), z);
    fmpz_clear(z);

    return res;
}

renf_elem_class::operator mpz_class() const
{
    mpz_class z;

    assert(is_integer() && "renf_elem_class not an integer");

    fmpz_get_mpz(z.get_mpz_t(), renf_elem_get_fmpz(a, nf->renf_t()));

    return z;
}

renf_elem_class::operator mpq_class() const
{
    if (is_zero())
    {
        return 0;
    }
    else
    {
        mpq_class z;

        assert(is_rational() && "renf_elem_class not a rational");

        fmpq_poly_t f;
        fmpq_poly_init(f);
        nf_elem_get_fmpq_poly(f, a->elem, nf->renf_t()->nf);
        fmpz_get_mpz(z.get_num_mpz_t(), fmpq_poly_numref(f));
        fmpz_get_mpz(z.get_den_mpz_t(), fmpq_poly_denref(f));
        fmpq_poly_clear(f);

        return z;
    }
}

std::vector<mpz_class> renf_elem_class::num_vector() const
{
    mpz_class x;
    std::vector<mpz_class> res;

    fmpq_poly_t f;
    fmpq_poly_init(f);
    nf_elem_get_fmpq_poly(f, a->elem, nf->renf_t()->nf);
    for (slong i = 0; i < fmpq_poly_length(f); i++)
    {
        fmpz_get_mpz(x.__get_mp(), fmpq_poly_numref(f) + i);
        res.push_back(x);
    }
    slong deg = fmpq_poly_degree(nf->renf_t()->nf->pol);
    for (slong i = fmpq_poly_length(f); i < deg; i++)
        res.push_back(mpz_class(0));
    fmpq_poly_clear(f);

    return res;
}

renf_elem_class::operator std::string() const
{
    return to_string();
}

std::string renf_elem_class::to_string(int flags) const
{
    std::string s;

    assert(!((flags & EANTIC_STR_D) && (flags & EANTIC_STR_ARB)));

    char * u = renf_elem_get_str_pretty(renf_elem_t(), nf->gen_name().c_str(), nf->renf_t(), 10, flags);
    s += u;
    flint_free(u);

    if (flags != EANTIC_STR_ALG && flags != EANTIC_STR_D && flags != EANTIC_STR_ARB)
        return "(" + s + ")";
    else
        return s;
}

mpz_class renf_elem_class::floor() const
{
    fmpz_t tmp;
    fmpz_init(tmp);

    renf_elem_floor(tmp, a, nf->renf_t());

    mpz_class z;
    fmpz_get_mpz(z.get_mpz_t(), tmp);
    fmpz_clear(tmp);
    return z;
}

mpz_class renf_elem_class::ceil() const
{
    fmpz_t tmp;
    fmpz_init(tmp);

    renf_elem_ceil(tmp, a, nf->renf_t());

    mpz_class z;
    fmpz_get_mpz(z.get_mpz_t(), tmp);
    fmpz_clear(tmp);
    return z;
}

int renf_elem_class::sgn() const
{
    return renf_elem_sgn(a, nf->renf_t());
}

renf_elem_class::operator double() const
{
    return renf_elem_get_d(a, nf->renf_t(), ARF_RND_NEAR);
}

renf_elem_class renf_elem_class::operator+() const { return *this; }

renf_elem_class renf_elem_class::operator-() const
{
    renf_elem_class ans(*this);
    renf_elem_neg(ans.a, ans.a, ans.nf->renf_t());
    return ans;
}

renf_elem_class::operator bool() const
{
    return *this != 0;
}

renf_elem_class & renf_elem_class::operator+=(const renf_elem_class & rhs)
{
    return binop<renf_elem_add, renf_elem_add_fmpz, renf_elem_add_fmpq>(*this, rhs);
}

renf_elem_class & renf_elem_class::operator-=(const renf_elem_class & rhs)
{
    return binop<renf_elem_sub, renf_elem_sub_fmpz, renf_elem_sub_fmpq>(*this, rhs);
}

renf_elem_class & renf_elem_class::operator*=(const renf_elem_class & rhs)
{
    return binop<renf_elem_mul, renf_elem_mul_fmpz, renf_elem_mul_fmpq>(*this, rhs);
}

renf_elem_class & renf_elem_class::operator/=(const renf_elem_class & rhs)
{
    return binop<renf_elem_div, renf_elem_div_fmpz, renf_elem_div_fmpq>(*this, rhs);
}

mpz_class renf_elem_class::floordiv(const renf_elem_class& rhs) const
{
    if (parent() != rhs.parent())
    {
        if (rhs.is_rational())
            return (*this / rhs).floor();
        if (this->is_rational())
            return renf_elem_class(rhs.parent(), static_cast<mpq_class>(*this)).floordiv(rhs);
    }

    mpz_class ret;
    fmpz_t floor;

    fmpz_init(floor);

    renf_elem_fdiv(floor, a, rhs.a, nf->renf_t());
    fmpz_get_mpz(ret.get_mpz_t(), floor);

    fmpz_clear(floor);

    return ret;
}

renf_elem_class renf_elem_class::pow(int exp) const
{
    renf_elem_class res(parent());

    if (exp < 0)
    {
        renf_elem_inv(res.renf_elem_t(), renf_elem_t(), nf->renf_t());
        renf_elem_pow(res.renf_elem_t(), res.renf_elem_t(), static_cast<ulong>(-exp), nf->renf_t());
    }
    else
    {
        renf_elem_pow(res.renf_elem_t(), renf_elem_t(), static_cast<ulong>(exp), nf->renf_t());
    }

    return res;
}

bool operator==(const renf_elem_class & lhs, const renf_elem_class & rhs)
{
    if (*lhs.nf == *rhs.nf)
      return renf_elem_equal(lhs.a, rhs.a, lhs.nf->renf_t());

    if (lhs.is_rational())
    {
        if (!rhs.is_rational())
            return false;

        return static_cast<mpq_class>(lhs) == static_cast<mpq_class>(rhs);
    }
    else if (rhs.is_rational())
    {
        return false;
    }
    else
        throw std::logic_error("not implemented: cannot compare renf_elem_class from different number fields");
}

bool operator<(const renf_elem_class & lhs, const renf_elem_class & rhs)
{
    if (*lhs.nf == *rhs.nf)
        return renf_elem_cmp(lhs.a, rhs.a, lhs.nf->renf_t()) < 0;

    if (lhs.is_rational())
    {
        mpq_class self = static_cast<mpq_class>(lhs);
        return self < rhs;
    }
    else if (rhs.is_rational())
    {
        mpq_class o = static_cast<mpq_class>(rhs);
        return lhs < o;
    }
    else
        throw std::logic_error("not implemented: cannot compare renf_elem_class from different number fields");
}

renf_elem_class& renf_elem_class::operator+=(int rhs)
{
    return *this += static_cast<long>(rhs);
}

renf_elem_class& renf_elem_class::operator-=(int rhs)
{
    return *this -= static_cast<long>(rhs);
}

renf_elem_class& renf_elem_class::operator*=(int rhs)
{
    return *this *= static_cast<long>(rhs);
}

renf_elem_class& renf_elem_class::operator/=(int rhs)
{
    return *this /= static_cast<long>(rhs);
}

bool operator==(const renf_elem_class& lhs, int rhs) {
    if (rhs == 0)
        return lhs.is_zero();
    return lhs == static_cast<long>(rhs);
}

bool operator<(const renf_elem_class& lhs, int rhs) {
    return lhs < static_cast<long>(rhs);
}

bool operator>(const renf_elem_class& lhs, int rhs) {
    return lhs > static_cast<long>(rhs);
}

renf_elem_class& renf_elem_class::operator+=(unsigned int rhs)
{
    return *this += static_cast<unsigned long>(rhs);
}

renf_elem_class& renf_elem_class::operator-=(unsigned int rhs)
{
    return *this -= static_cast<unsigned long>(rhs);
}

renf_elem_class& renf_elem_class::operator*=(unsigned int rhs)
{
    return *this *= static_cast<unsigned long>(rhs);
}

renf_elem_class& renf_elem_class::operator/=(unsigned int rhs)
{
    return *this /= static_cast<unsigned long>(rhs);
}

bool operator==(const renf_elem_class& lhs, unsigned int rhs) {
    return lhs == static_cast<unsigned long>(rhs);
}

bool operator<(const renf_elem_class& lhs, unsigned int rhs) {
    return lhs < static_cast<unsigned long>(rhs);
}

bool operator>(const renf_elem_class& lhs, unsigned int rhs) {
    return lhs > static_cast<unsigned long>(rhs);
}

renf_elem_class& renf_elem_class::operator+=(long rhs)
{
    renf_elem_add_si(renf_elem_t(), renf_elem_t(), rhs, nf->renf_t());
    return *this;
}

renf_elem_class& renf_elem_class::operator-=(long rhs)
{
    renf_elem_sub_si(renf_elem_t(), renf_elem_t(), rhs, nf->renf_t());
    return *this;
}

renf_elem_class& renf_elem_class::operator*=(long rhs)
{
    renf_elem_mul_si(renf_elem_t(), renf_elem_t(), rhs, nf->renf_t());
    return *this;
}

renf_elem_class& renf_elem_class::operator/=(long rhs)
{
    renf_elem_div_si(renf_elem_t(), renf_elem_t(), rhs, nf->renf_t());
    return *this;
}

bool operator==(const renf_elem_class& lhs, long rhs) {
    return renf_elem_equal_si(lhs.renf_elem_t(), rhs, lhs.nf->renf_t());
}

bool operator<(const renf_elem_class& lhs, long rhs) {
    return renf_elem_cmp_si(lhs.renf_elem_t(), rhs, lhs.nf->renf_t()) == -1;
}

bool operator>(const renf_elem_class& lhs, long rhs) {
    return renf_elem_cmp_si(lhs.renf_elem_t(), rhs, lhs.nf->renf_t()) == 1;
}

renf_elem_class& renf_elem_class::operator+=(unsigned long rhs)
{
    renf_elem_add_ui(renf_elem_t(), renf_elem_t(), rhs, nf->renf_t());
    return *this;
}

renf_elem_class& renf_elem_class::operator-=(unsigned long rhs)
{
    renf_elem_sub_ui(renf_elem_t(), renf_elem_t(), rhs, nf->renf_t());
    return *this;
}

renf_elem_class& renf_elem_class::operator*=(unsigned long rhs)
{
    renf_elem_mul_ui(renf_elem_t(), renf_elem_t(), rhs, nf->renf_t());
    return *this;
}

renf_elem_class& renf_elem_class::operator/=(unsigned long rhs)
{
    renf_elem_div_ui(renf_elem_t(), renf_elem_t(), rhs, nf->renf_t());
    return *this;
}

bool operator==(const renf_elem_class& lhs, unsigned long rhs) {
    return renf_elem_equal_ui(lhs.renf_elem_t(), rhs, lhs.nf->renf_t());
}

bool operator<(const renf_elem_class& lhs, unsigned long rhs) {
    return renf_elem_cmp_ui(lhs.renf_elem_t(), rhs, lhs.nf->renf_t()) == -1;
}

bool operator>(const renf_elem_class& lhs, unsigned long rhs) {
    return renf_elem_cmp_ui(lhs.renf_elem_t(), rhs, lhs.nf->renf_t()) == 1;
}

renf_elem_class& renf_elem_class::operator+=(long long rhs)
{
    binop_maybe_fmpz(*this, rhs, renf_elem_add_si, renf_elem_add_fmpz);
    return *this;
}

renf_elem_class& renf_elem_class::operator-=(long long rhs)
{
    binop_maybe_fmpz(*this, rhs, renf_elem_sub_si, renf_elem_sub_fmpz);
    return *this;
}

renf_elem_class& renf_elem_class::operator*=(long long rhs)
{
    binop_maybe_fmpz(*this, rhs, renf_elem_mul_si, renf_elem_mul_fmpz);
    return *this;
}

renf_elem_class& renf_elem_class::operator/=(long long rhs)
{
    binop_maybe_fmpz(*this, rhs, renf_elem_div_si, renf_elem_div_fmpz);
    return *this;
}

bool operator==(const renf_elem_class& lhs, long long rhs) {
    return relop_maybe_fmpz(lhs, rhs, renf_elem_equal_si);
}

bool operator<(const renf_elem_class& lhs, long long rhs) {
    return relop_maybe_fmpz(lhs, rhs, renf_elem_cmp_si, -1);
}

bool operator>(const renf_elem_class& lhs, long long rhs) {
    return relop_maybe_fmpz(lhs, rhs, renf_elem_cmp_si, 1);
}

renf_elem_class& renf_elem_class::operator+=(unsigned long long rhs)
{
    binop_maybe_fmpz(*this, rhs, renf_elem_add_ui, renf_elem_add_fmpz);
    return *this;
}

renf_elem_class& renf_elem_class::operator-=(unsigned long long rhs)
{
    binop_maybe_fmpz(*this, rhs, renf_elem_sub_ui, renf_elem_sub_fmpz);
    return *this;
}

renf_elem_class& renf_elem_class::operator*=(unsigned long long rhs)
{
    binop_maybe_fmpz(*this, rhs, renf_elem_mul_ui, renf_elem_mul_fmpz);
    return *this;
}

renf_elem_class& renf_elem_class::operator/=(unsigned long long rhs)
{
    binop_maybe_fmpz(*this, rhs, renf_elem_div_ui, renf_elem_div_fmpz);
    return *this;
}

bool operator==(const renf_elem_class& lhs, unsigned long long rhs) {
    return relop_maybe_fmpz(lhs, rhs, renf_elem_equal_ui);
}

bool operator<(const renf_elem_class& lhs, unsigned long long rhs) {
    return relop_maybe_fmpz(lhs, rhs, renf_elem_cmp_ui, -1);
}

bool operator>(const renf_elem_class& lhs, unsigned long long rhs) {
    return relop_maybe_fmpz(lhs, rhs, renf_elem_cmp_ui, 1);
}

renf_elem_class& renf_elem_class::operator+=(const mpz_class& rhs)
{
    return binop_mpz<renf_elem_add_fmpz>(*this, rhs);
}

renf_elem_class& renf_elem_class::operator-=(const mpz_class& rhs)
{
    return binop_mpz<renf_elem_sub_fmpz>(*this, rhs);
}

renf_elem_class& renf_elem_class::operator*=(const mpz_class& rhs)
{
    return binop_mpz<renf_elem_mul_fmpz>(*this, rhs);
}

renf_elem_class& renf_elem_class::operator/=(const mpz_class& rhs)
{
    return binop_mpz<renf_elem_div_fmpz>(*this, rhs);
}

bool operator==(const renf_elem_class& lhs, const mpz_class& rhs) {
    return relop_mpz(lhs, rhs, 0);
}

bool operator<(const renf_elem_class& lhs, const mpz_class& rhs) {
    return relop_mpz(lhs, rhs, -1);
}

bool operator>(const renf_elem_class& lhs, const mpz_class& rhs) {
    return relop_mpz(lhs, rhs, 1);
}

renf_elem_class& renf_elem_class::operator+=(const mpq_class& rhs)
{
    return binop_mpq<renf_elem_add_fmpq>(*this, rhs);
}

renf_elem_class& renf_elem_class::operator-=(const mpq_class& rhs)
{
    return binop_mpq<renf_elem_sub_fmpq>(*this, rhs);
}

renf_elem_class& renf_elem_class::operator*=(const mpq_class& rhs)
{
    return binop_mpq<renf_elem_mul_fmpq>(*this, rhs);
}

renf_elem_class& renf_elem_class::operator/=(const mpq_class& rhs)
{
    return binop_mpq<renf_elem_div_fmpq>(*this, rhs);
}

bool operator==(const renf_elem_class& lhs, const mpq_class& rhs) {
    return relop_mpq(lhs, rhs, 0);
}

bool operator<(const renf_elem_class& lhs, const mpq_class& rhs) {
    return relop_mpq(lhs, rhs, -1);
}

bool operator>(const renf_elem_class& lhs, const mpq_class& rhs) {
    return relop_mpq(lhs, rhs, 1);
}

std::string renf_elem_class::get_str(int flag) const { return to_string(flag); }

renf_elem_srcptr renf_elem_class::get_renf_elem(void) const
{
    return a;
}

mpz_class renf_elem_class::get_den() const { return den(); }

mpz_class renf_elem_class::get_num(void) const { return num(); }

mpq_class renf_elem_class::get_rational(void) const { return static_cast<mpq_class>(*this); }

std::vector<mpz_class> renf_elem_class::get_num_vector(void) const { return num_vector(); }

double renf_elem_class::get_d() const { return static_cast<double>(*this); }

mpz_class floor(const renf_elem_class& x)
{
    return x.floor();
}

mpz_class ceil(const renf_elem_class& x)
{
    return x.ceil();
}

renf_elem_class pow(const renf_elem_class& x, int exp)
{
    return x.pow(exp);
}

void swap(renf_elem_class& lhs, renf_elem_class& rhs) noexcept
{
    using std::swap;
    swap(lhs.nf, rhs.nf);
    renf_elem_swap(lhs.a, rhs.a);
}

} // end of namespace eantic

namespace std {
size_t hash<eantic::renf_elem_class>::operator()(const eantic::renf_elem_class& x) const
{
  return hash<double>()(static_cast<double>(x));
}
}
