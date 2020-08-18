/*
    Copyright (C) 2018 Vincent Delecroix
    Copyright (C) 2019-2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <cassert>
#include <flint/fmpq.h>
#include <iostream>
#include <cstdlib>
#include <functional>
#include <e-antic/renfxx.h>

namespace eantic {

renf_elem_class::renf_elem_class() noexcept
    : renf_elem_class(0)
{
}

renf_elem_class::renf_elem_class(const renf_elem_class & value) noexcept
    : renf_elem_class(value.nf)
{
    *this = value;
}

renf_elem_class::renf_elem_class(renf_elem_class && value) noexcept
    : renf_elem_class(value.nf)
{
    *this = std::move(value);
}

renf_elem_class::renf_elem_class(const mpz_class & value) noexcept
    : renf_elem_class()
{
    assign(value);
}
renf_elem_class::renf_elem_class(const mpq_class & value) noexcept
    : renf_elem_class()
{
    assign(value);
}

renf_elem_class::renf_elem_class(const ::fmpq_t value) noexcept
    : renf_elem_class()
{
    assign(value);
}

renf_elem_class::renf_elem_class(std::shared_ptr<const renf_class> k) noexcept
    : renf_elem_class(std::move(k), 0) {}

renf_elem_class::renf_elem_class(std::shared_ptr<const renf_class> k, const mpz_class & value) noexcept
    : renf_elem_class(std::move(k))
{
    assign(value);
}

renf_elem_class::renf_elem_class(std::shared_ptr<const renf_class> k, const mpq_class & value) noexcept
    : renf_elem_class(std::move(k))
{
    assign(value);
}

renf_elem_class::renf_elem_class(std::shared_ptr<const renf_class> k, const ::fmpq_t value) noexcept
    : renf_elem_class(std::move(k))
{
    assign(value);
}

renf_elem_class::renf_elem_class(std::shared_ptr<const renf_class> k, const std::string & str)
    : renf_elem_class(std::move(k))
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
    renf_elem_set_fmpq_poly(a, p, nf->renf_t());
    fmpq_poly_clear(p);

    flint_free(t);
}

renf_elem_class::~renf_elem_class() noexcept
{
    renf_elem_clear(a, nf->renf_t());
}

renf_elem_class & renf_elem_class::operator=(const renf_elem_class & value) noexcept
{
    if (value.nf != nf)
    {
        renf_elem_clear(a, nf->renf_t());
        renf_elem_init(a, value.nf->renf_t());
        nf = value.nf;
    }

    renf_elem_set(a, value.a, nf->renf_t());

    return *this;
}

renf_elem_class & renf_elem_class::operator=(renf_elem_class && value) noexcept
{
    std::swap(nf, value.nf);
    renf_elem_swap(a, value.a);

    return *this;
}

bool renf_elem_class::is_zero() const noexcept
{
    return renf_elem_is_zero(a, nf->renf_t());
}

bool renf_elem_class::is_one() const noexcept
{
    return renf_elem_is_one(a, nf->renf_t());
}

bool renf_elem_class::is_integer() const noexcept
{
    return renf_elem_is_integer(a, nf->renf_t());
}

bool renf_elem_class::is_rational() const noexcept
{
    return renf_elem_is_rational(a, nf->renf_t());
}

::renf_elem_t & renf_elem_class::renf_elem_t() const noexcept
{
    return a;
}

mpz_class renf_elem_class::num() const noexcept {
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

mpz_class renf_elem_class::num_content() const noexcept {
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

mpz_class renf_elem_class::den() const noexcept {
    mpz_class res;

    fmpz_t z;
    fmpz_init(z);
    nf_elem_get_den(z, a->elem, nf->renf_t()->nf);
    fmpz_get_mpz(res.__get_mp(), z);
    fmpz_clear(z);

    return res;
}

renf_elem_class::operator mpq_class() const noexcept
{
    mpq_class z;

    if (is_zero())
    {
        return 0;
    }
    else
    {
        assert(is_rational() && "renf_elem_class not a rational");

        fmpq_poly_t f;
        fmpq_poly_init(f);
        nf_elem_get_fmpq_poly(f, a->elem, nf->renf_t()->nf);
        fmpz_get_mpz(z.get_num_mpz_t(), fmpq_poly_numref(f));
        fmpz_get_mpz(z.get_den_mpz_t(), fmpq_poly_denref(f));
        fmpq_poly_clear(f);
    }

    return z;
}

std::vector<mpz_class> renf_elem_class::num_vector() const noexcept
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

renf_elem_class::operator std::string() const noexcept
{
    return to_string();
}

std::string renf_elem_class::to_string(int flags) const noexcept
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

mpz_class renf_elem_class::floor() const noexcept
{
    fmpz_t tmp;
    fmpz_init(tmp);

    renf_elem_floor(tmp, a, nf->renf_t());

    mpz_class z;
    fmpz_get_mpz(z.get_mpz_t(), tmp);
    fmpz_clear(tmp);
    return z;
}

mpz_class renf_elem_class::ceil() const noexcept
{
    fmpz_t tmp;
    fmpz_init(tmp);

    renf_elem_ceil(tmp, a, nf->renf_t());

    mpz_class z;
    fmpz_get_mpz(z.get_mpz_t(), tmp);
    fmpz_clear(tmp);
    return z;
}

int renf_elem_class::sgn() const noexcept
{
    return renf_elem_sgn(a, nf->renf_t());
}

renf_elem_class::operator double() const noexcept
{
    return renf_elem_get_d(a, nf->renf_t(), ARF_RND_NEAR);
}

renf_elem_class renf_elem_class::operator-() const noexcept
{
    renf_elem_class ans(*this);
    renf_elem_neg(ans.a, ans.a, ans.nf->renf_t());
    return ans;
}

renf_elem_class renf_elem_class::operator+() const noexcept { return *this; }

renf_elem_class::operator bool() const noexcept
{
    return *this != 0;
}

renf_elem_class & renf_elem_class::operator+=(const renf_elem_class & rhs) noexcept
{
    if (*nf == *rhs.nf)
        renf_elem_add(a, a, rhs.a, nf->renf_t());
    else if (is_rational())
        promote(rhs.nf) += rhs;
    else if (rhs.is_rational())
        *this += static_cast<mpq_class>(rhs);

    return *this;
}

renf_elem_class & renf_elem_class::operator-=(const renf_elem_class & rhs) noexcept
{
    if (*nf == *rhs.nf)
        renf_elem_sub(a, a, rhs.a, nf->renf_t());
    else if (is_rational())
        promote(rhs.nf) -= rhs;
    else if (rhs.is_rational())
        *this -= static_cast<mpq_class>(rhs);

    return *this;
}

renf_elem_class & renf_elem_class::operator*=(const renf_elem_class & rhs) noexcept
{
    if (*nf == *rhs.nf)
        renf_elem_mul(a, a, rhs.a, nf->renf_t());
    else if (is_rational())
        promote(rhs.nf) *= rhs;
    else if (rhs.is_rational())
        *this *= static_cast<mpq_class>(rhs);

    return *this;
}

renf_elem_class & renf_elem_class::operator/=(const renf_elem_class & rhs)
{
    if (*nf == *rhs.nf)
        renf_elem_div(a, a, rhs.a, nf->renf_t());
    else if (is_rational())
        promote(rhs.nf) /= rhs;
    else if (rhs.is_rational())
        *this /= static_cast<mpq_class>(rhs);

    return *this;
}

renf_elem_class renf_elem_class::pow(int exp) const noexcept
{
    renf_elem_class res(nf);

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

bool renf_elem_class::operator==(const renf_elem_class & other) const noexcept
{
    if (*nf == *other.nf)
      return renf_elem_equal(a, other.a, nf->renf_t());

    if (is_rational())
    {
        mpq_class self = static_cast<mpq_class>(*this);
        return self == other;
    }
    else if (other.is_rational())
    {
        mpq_class o = static_cast<mpq_class>(other);
        return *this == o;
    }

    std::cerr << "not implemented: cannot compare renf_elem_class from different number fields" << std::endl;
    abort();
}

bool renf_elem_class::operator<(const renf_elem_class & other) const noexcept
{
    if (*nf == *other.nf)
        return renf_elem_cmp(a, other.a, nf->renf_t()) < 0;

    if (is_rational())
    {
        mpq_class self = static_cast<mpq_class>(*this);
        return self < other;
    }
    else if (other.is_rational())
    {
        mpq_class o = static_cast<mpq_class>(other);
        return *this < o;
    }

    std::cerr << "not implemented: cannot compare renf_elem_class from different number fields" << std::endl;
    abort();
}

renf_elem_class& renf_elem_class::operator+=(const mpz_class& rhs) noexcept
{
    ::fmpz_t r;
    fmpz_init_set_readonly(r, rhs.get_mpz_t());
    renf_elem_add_fmpz(a, a, r, nf->renf_t());
    fmpz_clear_readonly(r);

    return *this;
}

renf_elem_class& renf_elem_class::operator-=(const mpz_class& rhs) noexcept
{
    ::fmpz_t r;
    fmpz_init_set_readonly(r, rhs.get_mpz_t());
    renf_elem_sub_fmpz(a, a, r, nf->renf_t());
    fmpz_clear_readonly(r);

    return *this;
}

renf_elem_class& renf_elem_class::operator*=(const mpz_class& rhs) noexcept
{
    ::fmpz_t r;
    fmpz_init_set_readonly(r, rhs.get_mpz_t());
    renf_elem_mul_fmpz(a, a, r, nf->renf_t());
    fmpz_clear_readonly(r);

    return *this;
}

renf_elem_class& renf_elem_class::operator/=(const mpz_class& rhs)
{
    ::fmpz_t r;
    fmpz_init_set_readonly(r, rhs.get_mpz_t());
    renf_elem_div_fmpz(a, a, r, nf->renf_t());
    fmpz_clear_readonly(r);

    return *this;
}

bool renf_elem_class::operator==(const mpz_class& rhs) const noexcept {
    ::fmpz_t r;
    fmpz_init_set_readonly(r, rhs.get_mpz_t());
    bool ret = renf_elem_equal_fmpz(a, r, nf->renf_t());
    fmpz_clear_readonly(r);

    return ret;
}

bool renf_elem_class::operator<(const mpz_class& rhs) const noexcept {
    ::fmpz_t r;
    fmpz_init_set_readonly(r, rhs.get_mpz_t());
    int ret = renf_elem_cmp_fmpz(a, r, nf->renf_t());
    fmpz_clear_readonly(r);

    return ret == -1;
}

bool renf_elem_class::operator>(const mpz_class& rhs) const noexcept {
    ::fmpz_t r;
    fmpz_init_set_readonly(r, rhs.get_mpz_t());
    int ret = renf_elem_cmp_fmpz(a, r, nf->renf_t());
    fmpz_clear_readonly(r);

    return ret == 1;
}

renf_elem_class& renf_elem_class::operator+=(const mpq_class& rhs) noexcept
{
    ::fmpq_t r;
    fmpq_init_set_readonly(r, rhs.get_mpq_t());
    renf_elem_add_fmpq(a, a, r, nf->renf_t());
    fmpq_clear_readonly(r);

    return *this;
}

renf_elem_class& renf_elem_class::operator-=(const mpq_class& rhs) noexcept
{
    ::fmpq_t r;
    fmpq_init_set_readonly(r, rhs.get_mpq_t());
    renf_elem_sub_fmpq(a, a, r, nf->renf_t());
    fmpq_clear_readonly(r);

    return *this;
}

renf_elem_class& renf_elem_class::operator*=(const mpq_class& rhs) noexcept
{
    ::fmpq_t r;
    fmpq_init_set_readonly(r, rhs.get_mpq_t());
    renf_elem_mul_fmpq(a, a, r, nf->renf_t());
    fmpq_clear_readonly(r);

    return *this;
}

renf_elem_class& renf_elem_class::operator/=(const mpq_class& rhs)
{
    ::fmpq_t r;
    fmpq_init_set_readonly(r, rhs.get_mpq_t());
    renf_elem_div_fmpq(a, a, r, nf->renf_t());
    fmpq_clear_readonly(r);

    return *this;
}

bool renf_elem_class::operator==(const mpq_class& rhs) const noexcept {
    ::fmpq_t r;
    fmpq_init_set_readonly(r, rhs.get_mpq_t());
    bool ret = renf_elem_equal_fmpq(a, r, nf->renf_t());
    fmpq_clear_readonly(r);

    return ret;
}

bool renf_elem_class::operator<(const mpq_class& rhs) const noexcept {
    ::fmpq_t r;
    fmpq_init_set_readonly(r, rhs.get_mpq_t());
    int ret = renf_elem_cmp_fmpq(a, r, nf->renf_t());
    fmpq_clear_readonly(r);

    return ret == -1;
}

bool renf_elem_class::operator>(const mpq_class& rhs) const noexcept {
    ::fmpq_t r;
    fmpq_init_set_readonly(r, rhs.get_mpq_t());
    int ret = renf_elem_cmp_fmpq(a, r, nf->renf_t());
    fmpq_clear_readonly(r);

    return ret == 1;
}

std::string renf_elem_class::get_str(int flag) const noexcept { return to_string(flag); }

renf_elem_srcptr renf_elem_class::get_renf_elem(void) const
{
    return a;
}

mpz_class renf_elem_class::get_den() const { return den(); }

mpz_class renf_elem_class::get_num(void) const { return num(); }

mpq_class renf_elem_class::get_rational(void) const { return static_cast<mpq_class>(*this); }

std::vector<mpz_class> renf_elem_class::get_num_vector(void) const noexcept { return num_vector(); }

double renf_elem_class::get_d() const noexcept { return static_cast<double>(*this); }

renf_elem_class& renf_elem_class::promote(std::shared_ptr<const renf_class> other) noexcept
{
    if (*this->nf == *other)
        return *this;
    if (is_rational())
    {
        renf_elem_class promoted = renf_elem_class(other);
        promoted.assign(static_cast<mpq_class>(*this));
        return *this = std::move(promoted);
    }

    std::cerr << "not implemented: cannot promote a renf_elem_class to a new number field" << std::endl;
    abort();
}

void renf_elem_class::assign(slong value) noexcept
{
    renf_elem_set_si(a, value, nf->renf_t());
}

void renf_elem_class::assign(ulong value) noexcept
{
    renf_elem_set_ui(a, value, nf->renf_t());
}

void renf_elem_class::assign(const ::fmpq_t value) noexcept
{
    renf_elem_set_fmpq(a, value, nf->renf_t());
}

void renf_elem_class::assign(const mpz_class & value) noexcept
{
    renf_elem_set_mpz(a, value.get_mpz_t(), nf->renf_t());
}

void renf_elem_class::assign(const mpq_class & value) noexcept
{
    renf_elem_set_mpq(a, value.get_mpq_t(), nf->renf_t());
}

} // end of namespace eantic

namespace std {
size_t hash<eantic::renf_elem_class>::operator()(const eantic::renf_elem_class& x) const noexcept
{
  return hash<double>()(static_cast<double>(x));
}
}
