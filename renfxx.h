/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef NF_EMB_ELEMXX_H
#define NF_EMB_ELEMXX_H

#include "gmpxx.h"

#include "renf.h"
#include "renf_elem.h"

class renf_elem_class
{
private:
    mutable renf * nf;
    mutable renf_elem_t a;
    mutable fmpq_t b;
public:
    // construction as zero in a given number field
    renf_elem_class(renf_t nf);

    // construction as integers or rationals
    renf_elem_class(const int=0);
    renf_elem_class(const unsigned int=0);
    renf_elem_class(const long);
    renf_elem_class(const unsigned long);
    renf_elem_class(const mpz_class &);
    renf_elem_class(const mpq_class &);
    renf_elem_class(const fmpq_t);

    ~renf_elem_class();

    // assignment
    renf_elem_class& operator = (const fmpz_t);
    renf_elem_class& operator = (const fmpq_t);
    renf_elem_class& operator = (const fmpq_poly_t);
    renf_elem_class& operator = (const renf_elem_class&);

    // unary operations
    renf_elem_class operator-();
    renf_elem_class operator+();

    // binary operations
    renf_elem_class operator + (const renf_elem_class&) const;
    renf_elem_class operator - (const renf_elem_class&) const;
    renf_elem_class operator * (const renf_elem_class&) const;
    renf_elem_class operator / (const renf_elem_class&) const;
    renf_elem_class& operator += (const renf_elem_class&);
    renf_elem_class& operator -= (const renf_elem_class&);
    renf_elem_class& operator *= (const renf_elem_class&);
    renf_elem_class& operator /= (const renf_elem_class&);
    bool operator == (const renf_elem_class&) const;
    bool operator != (const renf_elem_class&) const;
    bool operator >= (const renf_elem_class&) const;
    bool operator <= (const renf_elem_class&) const;
    bool operator > (const renf_elem_class&) const;
    bool operator < (const renf_elem_class&) const;

    bool is_zero() { return renf_elem_is_zero(this->a, this->nf); };
    bool is_one() { return renf_elem_is_one(this->a, this->nf); };

    // input, output
    friend std::ostream& operator << (std::ostream &, const renf_elem_class&);
    void print();

    // macro for declaration of
    // - binary operations
    // - inplace binary operations
    // - comparisons
    // - assignment
    #define __renf_ops(TYP) \
    renf_elem_class operator + (const TYP) const; \
    renf_elem_class operator - (const TYP) const; \
    renf_elem_class operator * (const TYP) const; \
    renf_elem_class operator / (const TYP) const; \
    friend renf_elem_class operator + (const TYP, const renf_elem_class &); \
    friend renf_elem_class operator - (const TYP, const renf_elem_class &); \
    friend renf_elem_class operator * (const TYP, const renf_elem_class &); \
    friend renf_elem_class operator / (const TYP, const renf_elem_class &); \
    renf_elem_class& operator += (const TYP); \
    renf_elem_class& operator -= (const TYP); \
    renf_elem_class& operator *= (const TYP); \
    renf_elem_class& operator /= (const TYP); \
    bool operator == (const TYP) const; \
    bool operator != (const TYP) const; \
    bool operator >= (const TYP) const; \
    bool operator <= (const TYP) const; \
    bool operator > (const TYP) const; \
    bool operator < (const TYP) const; \
    renf_elem_class& operator = (const TYP);

    __renf_ops(mpq_class&);
    __renf_ops(mpz_class&);
    __renf_ops(unsigned long);
    __renf_ops(long);
    __renf_ops(unsigned int);
    __renf_ops(int);
    #undef __renf_ops
};

renf_elem_class::renf_elem_class(renf_t nf)
{
    this->nf = nf;
    renf_elem_init(this->a, nf);
    renf_elem_zero(this->a, nf);
}

renf_elem_class::renf_elem_class(const mpq_class &x)
{
    this->nf = NULL;
    fmpq_init(this->b);
    fmpq_set_mpq(this->b, x.__get_mp());
}

renf_elem_class::renf_elem_class(const fmpq_t x)
{
    this->nf = NULL;
    fmpq_init(this->b);
    fmpq_set(this->b, x);
}


renf_elem_class::renf_elem_class(const mpz_class &x)
{
    this->nf = NULL;
    fmpq_init(this->b);
    fmpz_set_mpz(fmpq_numref(this->b), x.__get_mp());
    fmpz_set_si(fmpq_denref(this->b), 1);
}

renf_elem_class::renf_elem_class(int x)
{
    this->nf = NULL;
    fmpq_init(this->b);
    fmpq_set_si(this->b, x, 1);
}
renf_elem_class::renf_elem_class(unsigned int x)
{
    nf = NULL;
    fmpq_init(b);
    fmpz_set_ui(fmpq_numref(b), x);
    fmpz_one(fmpq_denref(b));
}
renf_elem_class::renf_elem_class(long x)
{
    nf = NULL;
    fmpq_init(b);
    fmpq_set_si(b, x, 1);
}
renf_elem_class::renf_elem_class(unsigned long x)
{
    nf = NULL;
    fmpq_init(b);
    fmpz_set_ui(fmpq_numref(b), x);
    fmpz_one(fmpq_denref(b));
}



renf_elem_class& renf_elem_class::operator=(const int n)
{
    if (nf == NULL) fmpq_set_si(b, n, 1);
    else renf_elem_set_si(a, n, nf);
}
renf_elem_class& renf_elem_class::operator=(const unsigned int n)
{
    fmpz_t x;
    fmpz_init(x);
    fmpz_set_ui(x, n);
    *this = x;
    fmpz_clear(x);
}
renf_elem_class& renf_elem_class::operator=(const long n)
{
    if (nf == NULL) fmpq_set_si(b, n, 1);
    else renf_elem_set_si(a, n, nf);
}
renf_elem_class& renf_elem_class::operator=(const unsigned long n)
{
    fmpz_t x;
    fmpz_init(x);
    fmpz_set_ui(x, n);
    *this = x;
    fmpz_clear(x);
}
renf_elem_class& renf_elem_class::operator=(const fmpz_t z)
{
    if (nf == NULL)
    {
        fmpz_set(fmpq_numref(b), z);
        fmpz_one(fmpq_denref(b));
    }
    else renf_elem_set_fmpz(a, z, nf);
}
renf_elem_class& renf_elem_class::operator=(const fmpq_t q)
{
    if (nf == NULL) fmpq_set(b, q);
    else renf_elem_set_fmpq(a, q, nf);
}
renf_elem_class& renf_elem_class::operator = (const fmpq_poly_t p)
{
    if (nf == NULL) throw 42;
    renf_elem_set_fmpq_poly(a, p, nf);
}
renf_elem_class& renf_elem_class::operator = (const mpz_class& z)
{
    fmpz_t x;
    fmpz_init(x);
    fmpz_set_mpz(x, z.get_mpz_t());
    *this = x;
    fmpz_clear(x);
}
renf_elem_class& renf_elem_class::operator = (const mpq_class& q)
{
    fmpq_t x;
    fmpq_init(x);
    fmpq_set_mpq(x, q.get_mpq_t());
    *this = x;
    fmpq_clear(x);
}
renf_elem_class& renf_elem_class::operator=(const renf_elem_class &x)
{
    if (x.nf == NULL)
    {
        if (nf != NULL)
        {
            renf_elem_clear(a, nf);
            fmpq_init(b);
            nf = NULL;
        }
        fmpq_set(b, x.b);
    }
    else if (nf == NULL)
    {
        nf = x.nf;
        fmpq_clear(b);
        renf_elem_init(a, nf);
        renf_elem_set(a, x.a, nf);
    }
    else
    {
        renf_elem_clear(a, nf);
        nf = x.nf;
        renf_elem_init(a, nf);
        renf_elem_set(a, x.a, nf);
    }

    return *this;
}

renf_elem_class::~renf_elem_class(void)
{
    if (nf == NULL) fmpq_clear(b);
    else renf_elem_clear(a, nf);
}

std::ostream& operator<<(std::ostream & os, const renf_elem_class& a)
{
    char * res;
    if (a.nf == NULL)
        res = fmpq_get_str(NULL, 10, a.b);
    else
        res = renf_elem_get_str_pretty(a.a, "x", a.nf, 5);
    os << res;
    flint_free(res);
    return os;
}

renf_elem_class renf_elem_class::operator-()
{
    return *this;
}
renf_elem_class renf_elem_class::operator+()
{
    return *this;
}


#define __renf_elem_op(OP, INOP, FUN1, FUN2, FUN3) \
renf_elem_class renf_elem_class::operator OP (const renf_elem_class & other) const \
{                                         \
    if (nf != NULL)                       \
    {                                     \
        renf_elem_class ans(nf);          \
        if (nf == other.nf)               \
            FUN1(ans.a, a, other.a, nf);  \
        else if (other.nf == NULL)        \
            FUN2(ans.a, a, other.b, nf);  \
        else                              \
            throw 42;                     \
        return ans;                       \
    }                                     \
    else if (other.nf == NULL)            \
    {                                     \
        renf_elem_class ans(b);           \
        FUN3(ans.b, ans.b, other.b);      \
        return ans;                       \
    }                                     \
    else                                  \
    {                                     \
        renf_elem_class ans(other.nf);    \
        FUN2(ans.a, other.a, b, other.nf);\
        return ans;                       \
    }                                     \
}                                         \
renf_elem_class& renf_elem_class::operator INOP (const renf_elem_class & other) \
{                                         \
    return *this;                         \
}                                         \
renf_elem_class& renf_elem_class::operator INOP (const mpq_class& other) \
{                                         \
    return *this;                         \
}                                         \
renf_elem_class& renf_elem_class::operator INOP (const mpz_class& other) \
{                                         \
    return *this;                         \
}                                         \
renf_elem_class& renf_elem_class::operator INOP (const long other) \
{                                         \
    return *this;                         \
}                                         \
renf_elem_class& renf_elem_class::operator INOP (const int other) \
{                                         \
    return *this;                         \
}                                         \
// TODO: implement inplace operators
__renf_elem_op(+, +=, renf_elem_add, renf_elem_add_fmpq, fmpq_add);
__renf_elem_op(*, *=, renf_elem_mul, renf_elem_mul_fmpq, fmpq_mul);
__renf_elem_op(-, -=, renf_elem_sub, renf_elem_sub_fmpq, fmpq_sub);
__renf_elem_op(/, /=, renf_elem_div, renf_elem_div_fmpq, fmpq_div);
#undef __renf_elem_op

#define __renf_elem_op(TYP, OP, INOP) \
renf_elem_class renf_elem_class::operator OP(const TYP other) const \
{                                                                   \
    renf_elem_class ans(other);                                     \
    return (*this) OP other;                                        \
}                                                                   \
renf_elem_class operator OP (const TYP a, const renf_elem_class& b) \
{                                                                   \
    renf_elem_class ans(b);                                         \
    return b OP a;                                                  \
}
__renf_elem_op(mpq_class&, +, +=);
__renf_elem_op(mpz_class&, +, +=);
__renf_elem_op(long, +, +=);
__renf_elem_op(unsigned long, +, +=);
__renf_elem_op(int, +, +=);
__renf_elem_op(unsigned int, +, +=);
__renf_elem_op(mpq_class&, -, -=);
__renf_elem_op(mpz_class&, -, -=);
__renf_elem_op(long, -, -=);
__renf_elem_op(unsigned long, -, -=);
__renf_elem_op(int, -, -=);
__renf_elem_op(unsigned int, -, -=);
__renf_elem_op(mpq_class&, *, *=);
__renf_elem_op(mpz_class&, *, *=);
__renf_elem_op(long, *, *=);
__renf_elem_op(unsigned long, *, *=);
__renf_elem_op(int, *, *=);
__renf_elem_op(unsigned int, *, *=);
__renf_elem_op(mpq_class&, /, /=);
__renf_elem_op(mpz_class&, /, /=);
__renf_elem_op(long, /, /=);
__renf_elem_op(unsigned long, /, /=);
__renf_elem_op(int, /, /=);
__renf_elem_op(unsigned int, /, /=);
#undef __renf_elem_op

bool renf_elem_class::operator==(const renf_elem_class& other) const
{
    if (this->nf != NULL)
    {
        if (this->nf == other.nf)
            return renf_elem_equal(this->a, other.a, this->nf);
        else if (other.nf == NULL)
            return renf_elem_cmp_fmpq(this->a, other.b, this->nf) == 0;
        else
            flint_abort();
    }
    else if (other.nf == NULL)
    {
        return fmpq_equal(this->b, other.b);
    }
    else
    {
        return renf_elem_cmp_fmpq(other.a, this->b, other.nf) == 0;
    }
}
bool renf_elem_class::operator==(long n) const
{
    if (this->nf != NULL)
    {
        renf_elem_class other(this->nf);
        renf_elem_set_si(other.a, n, this->nf);
        return renf_elem_equal(this->a, other.a, this->nf);
    }
    else
    {
        fmpq_t other;
        int ans;
        fmpq_init(other);
        fmpq_set_si(other, n, 1);
        ans = fmpq_cmp(this->b, other) == 0;
        flint_free(other);
        return ans;
    }
}
bool renf_elem_class::operator==(unsigned long n) const
{
    renf_elem_class other(n);
    return *this == other;
}
bool renf_elem_class::operator==(int n) const
{
    return (*this) == (long) n;
}
bool renf_elem_class::operator==(unsigned int n) const
{
    return (*this) == (unsigned long) n;
}

bool renf_elem_class::operator>(const renf_elem_class & other) const
{
    if (this->nf != NULL)
    {
        if (this->nf == other.nf)
            return renf_elem_cmp(this->a, other.a, this->nf) > 0;
        else if (other.nf == NULL)
            return renf_elem_cmp_fmpq(this->a, other.b, this->nf) > 0;
        else
            flint_abort();
    }
    else if (other.nf == NULL)
    {
        return fmpq_cmp(this->b, other.b) > 0;
    }
    else
    {
        return renf_elem_cmp_fmpq(other.a, this->b, other.nf) < 0;
    }
}
bool renf_elem_class::operator>(long n) const
{
    if (this->nf != NULL)
    {
        renf_elem_class other(this->nf);
        renf_elem_set_si(other.a, n, this->nf);
        return renf_elem_cmp(this->a, other.a, this->nf) > 0;
    }
    else
    {
        fmpq_t other;
        int ans;
        fmpq_init(other);
        fmpq_set_si(other, n, 1);
        ans = fmpq_cmp(this->b, other) > 0;
        flint_free(other);
        return ans;
    }
}
bool renf_elem_class::operator>(int n) const
{
    return (*this) == (long) n;
}

bool renf_elem_class::operator!=(const renf_elem_class& other) const
{
    return not (*this == other);
}
bool renf_elem_class::operator!=(long n) const
{
    return not (*this == n);
}
bool renf_elem_class::operator!=(int n) const
{
    return not (*this == n);
}

bool renf_elem_class::operator>=(const renf_elem_class & other) const
{
    return *this == other || *this > other;
}
bool renf_elem_class::operator>=(long n) const
{
    return *this == n || *this > n;
}
bool renf_elem_class::operator>=(int n) const
{
    return *this == n || *this > n;
}


bool renf_elem_class::operator<=(const renf_elem_class& other) const
{
    return not (*this > other);
}
bool renf_elem_class::operator<=(long n) const
{
    return not (*this > n);
}
bool renf_elem_class::operator<=(int n) const
{
    return not (*this > n);
}


bool renf_elem_class::operator<(const renf_elem_class& other) const
{
    return not (*this >= other);
}
bool renf_elem_class::operator<(long n) const
{
    return not (*this >= 0);
}
bool renf_elem_class::operator<(int n) const
{
    return not (*this >= 0);
}

#endif

