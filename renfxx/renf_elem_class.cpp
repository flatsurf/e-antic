/*
    Copyright (C) 2018 Vincent Delecroix
    Copyright (C) 2019 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <cassert>
#include <e-antic/renfxx.h>

namespace eantic {

renf_elem_class::renf_elem_class() noexcept
    : renf_elem_class(0)
{
}

renf_elem_class::renf_elem_class(const renf_elem_class & value) noexcept
    : renf_elem_class()
{
    *this = value;
}

renf_elem_class::renf_elem_class(renf_elem_class && value) noexcept
    : renf_elem_class()
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

renf_elem_class::renf_elem_class(const renf_class & k) noexcept
    : renf_elem_class(k, 0) {};

renf_elem_class::renf_elem_class(const renf_class & k, const mpz_class & value) noexcept
    : renf_elem_class(k)
{
    assign(value);
}

renf_elem_class::renf_elem_class(const renf_class & k, const mpq_class & value) noexcept
    : renf_elem_class(k)
{
    assign(value);
}

renf_elem_class::renf_elem_class(const renf_class & k, const ::fmpq_t value) noexcept
    : renf_elem_class(k)
{
    assign(value);
}

renf_elem_class::renf_elem_class(const renf_class & k, const std::string & str)
    : renf_elem_class(k)
{
    const char * s = str.c_str();

    int err;
    const char * i = strchr(s, '~');
    char * t;

    if (i != nullptr)
    {
        t = (char *) flint_malloc((i - s + 1) * sizeof(char));
        strncpy(t, s, i - s);
        t[i - s] = '\0';
    }
    else
    {
        t = (char *) flint_malloc((strlen(s) + 1) * sizeof(char));
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
    if (is_fmpq())
        fmpq_clear(b);
    else
        renf_elem_clear(a, nf->renf_t());
}

renf_elem_class & renf_elem_class::operator=(const renf_elem_class & value) noexcept
{
    if (value.nf == nullptr)
    {
        if (nf != nullptr)
        {
            renf_elem_clear(a, nf->renf_t());
            nf = nullptr;
            fmpq_init(b);
        }
    }
    else
    {
        if (nf == nullptr)
        {
            fmpq_clear(b);
            renf_elem_init(a, value.nf->renf_t());
            nf = value.nf;
        }
        else if (value.nf != nf)
        {
            renf_elem_clear(a, nf->renf_t());
            renf_elem_init(a, value.nf->renf_t());
            nf = value.nf;
        }
    }

    if (value.nf == nullptr)
        fmpq_set(b, value.b);
    else
        renf_elem_set(a, value.a, nf->renf_t());

    return *this;
}

renf_elem_class & renf_elem_class::operator=(renf_elem_class && value) noexcept
{
    if (value.nf == nullptr)
    {
        if (nf != nullptr)
        {
            renf_elem_clear(a, nf->renf_t());
            fmpq_init(b);
        }
        nf = value.nf;
        fmpq_swap(b, value.b);
    }
    else
    {
        if (nf == nullptr)
        {
            fmpq_clear(b);
            renf_elem_init(a, value.nf->renf_t());
        }
        nf = value.nf;
        renf_elem_swap(a, value.a);
    }

    return *this;
}

bool renf_elem_class::is_fmpq(void) const noexcept { return (nf == nullptr); }

bool renf_elem_class::is_zero() const noexcept
{
    if (nf == nullptr)
        return fmpq_is_zero(b);
    else
        return renf_elem_is_zero(a, nf->renf_t());
}

bool renf_elem_class::is_one() const noexcept
{
    if (nf == nullptr)
        return fmpq_is_one(b);
    else
        return renf_elem_is_one(a, nf->renf_t());
}

bool renf_elem_class::is_integer() const noexcept
{
    if (nf == nullptr)
        return fmpz_is_one(fmpq_denref(b));
    else
        return renf_elem_is_integer(a, nf->renf_t());
}

bool renf_elem_class::is_rational() const noexcept
{
    if (nf == nullptr)
        return true;
    else
        return renf_elem_is_rational(a, nf->renf_t());
}

::fmpq_t & renf_elem_class::fmpq_t() noexcept
{
    assert(is_fmpq());
    return b;
}

const ::fmpq_t & renf_elem_class::fmpq_t() const noexcept
{
    assert(is_fmpq());
    return b;
}

::renf_elem_t & renf_elem_class::renf_elem_t() const noexcept
{
    assert(is_renf_elem());
    return a;
}

mpz_class renf_elem_class::num() const noexcept {
    mpz_class x;

    if (nf == nullptr)
    {
        fmpz_get_mpz(x.__get_mp(), fmpq_numref(b));
        return x;
    }
    else if (nf->renf_t()->nf->flag & NF_LINEAR)
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

mpz_class renf_elem_class::den() const noexcept {
    mpz_class res;

    if (nf == nullptr)
        fmpz_get_mpz(res.__get_mp(), fmpq_denref(b));
    else
    {
        fmpz_t z;
        fmpz_init(z);
        nf_elem_get_den(z, a->elem, nf->renf_t()->nf);
        fmpz_get_mpz(res.__get_mp(), z);
        fmpz_clear(z);
    }

    return res;
}

renf_elem_class::operator mpq_class() const noexcept
{
    mpq_class z;

    if (nf == nullptr)
        fmpq_get_mpq(z.__get_mp(), b);
    else if (is_rational())
    {
        assert(is_rational() && "renf_elem_class not a rational");
        ::fmpq_t q;
        fmpq_init(q);
        nf_elem_get_fmpq(q, a->elem, parent().renf_t()->nf);
        fmpq_get_mpq(z.__get_mp(), q);
        fmpq_clear(q);
    }

    return z;
}

std::vector<mpz_class> renf_elem_class::num_vector() const noexcept
{
    mpz_class x;
    std::vector<mpz_class> res;

    if (nf == NULL)
    {
        fmpz_get_mpz(x.__get_mp(), fmpq_numref(b));
        res.push_back(x);
    }
    else
    {
        fmpq_poly_t f;
        fmpq_poly_init(f);
        nf_elem_get_fmpq_poly(f, a->elem, nf->renf_t()->nf);
        for (size_t i = 0; i < fmpq_poly_length(f); i++)
        {
            fmpz_get_mpz(x.__get_mp(), fmpq_poly_numref(f) + i);
            res.push_back(x);
        }
        size_t deg = fmpq_poly_degree(nf->renf_t()->nf->pol);
        for (size_t i = fmpq_poly_length(f); i < deg; i++)
            res.push_back(mpz_class(0));
        fmpq_poly_clear(f);
    }

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

    // call to renf_elem_get_str_pretty
    if (nf == nullptr)
    {
        if (flags & EANTIC_STR_ALG)
        {
            char * u = fmpq_get_str(nullptr, 10, b);
            s += u;
            flint_free(u);

            if (flags & (EANTIC_STR_D | EANTIC_STR_ARB)) s += " ~ ";
        }
        if (flags & EANTIC_STR_D)
        {
            char * u = (char *) flint_malloc(20 * sizeof(char));
            sprintf(u, "%lf", fmpq_get_d(b));
            s += u;
            flint_free(u);
        }
        if (flags & EANTIC_STR_ARB)
        {
            char * u;
            arb_t x;
            arb_init(x);
            arb_set_fmpq(x, b, 128);
            u = arb_get_str(x, 64, 0);
            s += u;
            arb_clear(x);
            flint_free(u);
        }
    }
    else
    {
        char * u = renf_elem_get_str_pretty(renf_elem_t(), parent().gen_name().c_str(), parent().renf_t(), 10, flags);
        s += u;
        flint_free(u);
    }

    if (flags != EANTIC_STR_ALG && flags != EANTIC_STR_D && flags != EANTIC_STR_ARB)
        return "(" + s + ")";
    else
        return s;
}

mpz_class renf_elem_class::floor() const noexcept
{
    fmpz_t tmp;
    fmpz_init(tmp);

    if (nf == nullptr)
        fmpz_fdiv_q(tmp, fmpq_numref(b), fmpq_denref(b));
    else
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

    if (nf == nullptr)
    {
        fmpz_add(tmp, fmpq_numref(b), fmpq_denref(b));
        fmpz_sub_ui(tmp, tmp, 1);
        fmpz_fdiv_q(tmp, tmp, fmpq_denref(b));
    }
    else
        renf_elem_ceil(tmp, a, nf->renf_t());

    mpz_class z;
    fmpz_get_mpz(z.get_mpz_t(), tmp);
    fmpz_clear(tmp);
    return z;
}

int renf_elem_class::sgn() const noexcept
{
    if (nf == nullptr)
        return fmpq_sgn(b);
    else
        return renf_elem_sgn(a, nf->renf_t());
}

renf_elem_class::operator double() const noexcept
{
    if (nf == nullptr)
    {
        arb_t s;
        arb_init(s);
        arb_set_fmpq(s, b, 128);
        double ans = arf_get_d(arb_midref(s), ARF_RND_NEAR);
        arb_clear(s);
        return ans;
    }
    else
        return renf_elem_get_d(a, nf->renf_t(), ARF_RND_NEAR);
}

renf_elem_class renf_elem_class::operator-() const noexcept
{
    renf_elem_class ans(*this);
    if (nf == nullptr)
        fmpq_neg(ans.b, ans.b);
    else
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
    if (rhs.is_fmpq())
        inplace_binop(rhs.b, fmpq_add, renf_elem_add_fmpq);
    else
    {
        promote(*rhs.nf);
        renf_elem_add(a, a, rhs.a, nf->renf_t());
    }
    return *this;
}

renf_elem_class & renf_elem_class::operator-=(const renf_elem_class & rhs) noexcept
{
    if (rhs.is_fmpq())
        inplace_binop(rhs.b, fmpq_sub, renf_elem_sub_fmpq);
    else
    {
        promote(*rhs.nf);
        renf_elem_sub(a, a, rhs.a, nf->renf_t());
    }
    return *this;
}

renf_elem_class & renf_elem_class::operator*=(const renf_elem_class & rhs) noexcept
{
    if (rhs.is_fmpq())
        inplace_binop(rhs.b, fmpq_mul, renf_elem_mul_fmpq);
    else
    {
        promote(*rhs.nf);
        renf_elem_mul(a, a, rhs.a, nf->renf_t());
    }
    return *this;
}

renf_elem_class & renf_elem_class::operator/=(const renf_elem_class & rhs)
{
    if (rhs.is_fmpq())
        inplace_binop(rhs.b, fmpq_div, renf_elem_div_fmpq);
    else
    {
        promote(*rhs.nf);
        renf_elem_div(a, a, rhs.a, nf->renf_t());
    }
    return *this;
}

bool renf_elem_class::operator==(const renf_elem_class & other) const noexcept
{
    return (*this - other).is_zero();
}

bool renf_elem_class::operator<(const renf_elem_class & other) const noexcept
{
    if (nf != nullptr)
    {
        if (nf == other.nf)
            return renf_elem_cmp(a, other.a, nf->renf_t()) < 0;
        else
        {
            assert(other.nf == nullptr && "can not compare renf_elem_class from different number fields");
            return renf_elem_cmp_fmpq(a, other.b, nf->renf_t()) < 0;
        }
    }
    else if (other.nf == nullptr)
        return fmpq_cmp(b, other.b) < 0;
    else
        return renf_elem_cmp_fmpq(other.a, b, other.nf->renf_t()) > 0;
}

std::string renf_elem_class::get_str(int flag) const noexcept { return to_string(flag); }

fmpq * renf_elem_class::get_fmpq(void) const
{
    if (not is_fmpq()) throw std::invalid_argument("renf_elem_class not a fmpq");
    // to stay compatible with the pre-1.0 interface, we cast away constness
    else
        return const_cast<fmpq *>(b);
}

renf_elem_srcptr renf_elem_class::get_renf_elem(void) const
{
    if (is_fmpq())
        throw std::invalid_argument("renf_elem_class is a fmpq");
    else
        return a;
}

mpz_class renf_elem_class::get_den() const { return den(); }

mpz_class renf_elem_class::get_num(void) const { return num(); }

mpq_class renf_elem_class::get_rational(void) const { return static_cast<mpq_class>(*this); }

std::vector<mpz_class> renf_elem_class::get_num_vector(void) const noexcept { return num_vector(); }

double renf_elem_class::get_d() const noexcept { return static_cast<double>(*this); }

void renf_elem_class::promote(const renf_class & nf) noexcept
{
    if (is_fmpq())
    {
        renf_elem_class promoted = renf_elem_class(nf);
        promoted.assign(b);
        *this = std::move(promoted);
    }
    else
    {
        if (*this->nf == nf)
            return;
        else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wterminate"
            throw std::logic_error("not implemented: cannot promote a "
                                   "renf_elem_class to a new number field");
#pragma GCC diagnostic pop
    }
}

void renf_elem_class::assign(slong value) noexcept
{
    if (nf == nullptr)
    {
        fmpz_one(fmpq_denref(b));
        fmpz_set_si(fmpq_numref(b), value);
    }
    else
        renf_elem_set_si(a, value, nf->renf_t());
}

void renf_elem_class::assign(ulong value) noexcept
{
    if (nf == nullptr)
    {
        fmpz_one(fmpq_denref(b));
        fmpz_set_ui(fmpq_numref(b), value);
    }
    else
        renf_elem_set_ui(a, value, nf->renf_t());
}

void renf_elem_class::assign(const ::fmpq_t value) noexcept
{
    if (nf == nullptr)
        fmpq_set(b, value);
    else
        renf_elem_set_fmpq(a, value, nf->renf_t());
}

void renf_elem_class::assign(const mpz_class & value) noexcept
{
    if (nf == nullptr)
    {
        fmpz_one(fmpq_denref(b));
        fmpz_set_mpz(fmpq_numref(b), value.__get_mp());
    }
    else
        renf_elem_set_mpz(a, value.get_mpz_t(), nf->renf_t());
}

void renf_elem_class::assign(const mpq_class & value) noexcept
{
    if (nf == nullptr)
        fmpq_set_mpq(b, value.__get_mp());
    else
        renf_elem_set_mpq(a, value.get_mpq_t(), nf->renf_t());
}

} // end of namespace eantic
