/*  This is a -*- C++ -*- header file.

    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef NF_EMB_ELEMXX_H
#define NF_EMB_ELEMXX_H

#include <iostream>
#include <gmpxx.h>

#include "renf.h"
#include "renf_elem.h"

class renf_elem_class
{
private:
    mutable renf_srcptr nf;
    mutable renf_elem_t a;
    mutable fmpq_t b;
public:
    // construction as zero in a given number field
    renf_elem_class(renf_t nf);

    // construction as integers or rationals
    renf_elem_class(const int=0);
    renf_elem_class(const unsigned int);
    renf_elem_class(const long);
    renf_elem_class(const unsigned long);
    renf_elem_class(const mpz_class&);
    renf_elem_class(const mpq_class&);
    renf_elem_class(const fmpz_t&);
    renf_elem_class(const fmpq_t&);
    renf_elem_class(const renf_elem_class&);

    ~renf_elem_class();

    // access to attribute
    bool is_fmpq(void);
    fmpq * get_fmpq(void);
    renf_elem_srcptr get_renf_elem(void);

    // assignment
    renf_elem_class& operator = (const fmpz_t&);
    renf_elem_class& operator = (const fmpq_t&);
    renf_elem_class& operator = (const fmpq_poly_t&);
    renf_elem_class& operator = (const renf_elem_class&);

    // unary operations
    renf_elem_class operator-() const;
    renf_elem_class operator+() const;

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
    // I/O manipulator that stores a renf in an input stream
    // for use by operator >>.
    friend std::ios_base& set_renf(std::ios_base &, renf_t);
    friend std::ostream& operator << (std::ostream &, const renf_elem_class&);
    friend std::istream& operator >> (std::istream &, renf_elem_class&);
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

renf_elem_class::renf_elem_class(renf_t k)
{
    nf = k;
    renf_elem_init(a, nf);
    renf_elem_zero(a, nf);
}
renf_elem_class::renf_elem_class(int x)
{
    nf = NULL;
    fmpq_init(b);
    fmpq_set_si(b, x, 1);
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
renf_elem_class::renf_elem_class(const mpz_class &x)
{
    nf = NULL;
    fmpq_init(b);
    fmpz_set_mpz(fmpq_numref(b), x.__get_mp());
    fmpz_set_si(fmpq_denref(b), 1);
}
renf_elem_class::renf_elem_class(const mpq_class &x)
{
    nf = NULL;
    fmpq_init(b);
    fmpq_set_mpq(b, x.__get_mp());
}
renf_elem_class::renf_elem_class(const fmpz_t& x)
{
    nf = NULL;
    fmpq_init(b);
    fmpz_set(fmpq_numref(b), x);
    fmpz_one(fmpq_denref(b));
}
renf_elem_class::renf_elem_class(const fmpq_t& x)
{
    nf = NULL;
    fmpq_init(b);
    fmpq_set(b, x);
}
renf_elem_class::renf_elem_class(const renf_elem_class& x)
{
    nf = x.nf;
    if (nf == NULL)
    {
        fmpq_init(b);
        fmpq_set(b, x.b);
    }
    else
    {
        renf_elem_init(a, nf);
        renf_elem_set(a, x.a, nf);
    }
}


renf_elem_class::~renf_elem_class(void)
{
    if (nf == NULL) fmpq_clear(b);
    else renf_elem_clear(a, nf);
}


bool renf_elem_class::is_fmpq(void)
{
    return (nf == NULL);
}

fmpq * renf_elem_class::get_fmpq(void)
{
    if(not is_fmpq()) throw 42;
    else return b;
}
renf_elem_srcptr renf_elem_class::get_renf_elem(void)
{
    if(is_fmpq()) throw 42;
    else return a;
}


renf_elem_class& renf_elem_class::operator = (const int n)
{
    if (nf == NULL) fmpq_set_si(b, n, 1);
    else renf_elem_set_si(a, n, nf);
    return *this;
}
renf_elem_class& renf_elem_class::operator = (const unsigned int n)
{
    fmpz_t x;
    fmpz_init(x);
    fmpz_set_ui(x, n);
    *this = x;
    fmpz_clear(x);
    return *this;
}
renf_elem_class& renf_elem_class::operator = (const long n)
{
    if (nf == NULL) fmpq_set_si(b, n, 1);
    else renf_elem_set_si(a, n, nf);
    return *this;
}
renf_elem_class& renf_elem_class::operator = (const unsigned long n)
{
    fmpz_t x;
    fmpz_init(x);
    fmpz_set_ui(x, n);
    *this = x;
    fmpz_clear(x);
    return *this;
}
renf_elem_class& renf_elem_class::operator = (const fmpz_t& z)
{
    if (nf == NULL)
    {
        fmpz_set(fmpq_numref(b), z);
        fmpz_one(fmpq_denref(b));
    }
    else renf_elem_set_fmpz(a, z, nf);
    return *this;
}
renf_elem_class& renf_elem_class::operator = (const fmpq_t& q)
{
    if (nf == NULL) fmpq_set(b, q);
    else renf_elem_set_fmpq(a, q, nf);
    return *this;
}
renf_elem_class& renf_elem_class::operator = (const fmpq_poly_t& p)
{
    if (nf == NULL) throw 42;
    renf_elem_set_fmpq_poly(a, p, nf);
    return *this;
}
renf_elem_class& renf_elem_class::operator = (const mpz_class& z)
{
    fmpz_t x;
    fmpz_init(x);
    fmpz_set_mpz(x, z.get_mpz_t());
    *this = x;
    fmpz_clear(x);
    return *this;
}
renf_elem_class& renf_elem_class::operator = (const mpq_class& q)
{
    fmpq_t x;
    fmpq_init(x);
    fmpq_set_mpq(x, q.get_mpq_t());
    *this = x;
    fmpq_clear(x);
    return *this;
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

// I/O

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

struct set_renf {
    renf *_nf;
    set_renf(renf_t nf) { _nf = nf; }
    static int xalloc();
};

inline int set_renf::xalloc()
{
    static int xa = std::ios_base::xalloc();
    return xa;
}

std::istream& operator>>(std::istream & is, const set_renf &sr)
{
    is.iword(set_renf::xalloc()) = (long) sr._nf;
    return is;
}

std::istream& operator>>(std::istream & is, renf_elem_class& a)
{
    renf *nf = (renf *) is.iword(set_renf::xalloc());
    if (!nf) {
        // If no number field has been set, use rational input.
        mpq_class x;
        is >> x;
        a = x;
    }
    else {
        // FIXME: Read element
        throw 42;
    }
}


renf_elem_class renf_elem_class::operator-() const
{
    renf_elem_class ans(*this);
    if (nf == NULL) fmpq_neg(ans.b, ans.b);
    else renf_elem_neg(ans.a, ans.a, ans.nf);
}
renf_elem_class renf_elem_class::operator+() const
{
    return *this;
}


static __inline__ void renf_elem_fmpq_add(renf_elem_t a, fmpq_t b, renf_elem_t c, renf_t d) {renf_elem_add_fmpq(a, c, b, d);}
static __inline__ void renf_elem_fmpq_mul(renf_elem_t a, fmpq_t b, renf_elem_t c, renf_t d) {renf_elem_mul_fmpq(a, c, b, d);}

#define __renf_elem_op(OP, INOP, FUN1, FUN2, FUN3, FUN4) \
renf_elem_class& renf_elem_class::operator INOP (const renf_elem_class & other) \
{                                         \
    if (nf != NULL)                       \
    {                                     \
        if (nf == other.nf)               \
            FUN1(a, a, other.a, nf);      \
        else if (other.nf == NULL)        \
            FUN2(a, a, other.b, nf);      \
        else                              \
            throw 42;                     \
    }                                     \
    else if (other.nf != NULL)            \
    {                                     \
        /* promote to nf elt */           \
        nf = other.nf;                    \
        renf_elem_init(a, nf);            \
        renf_elem_set_fmpq(a, b, nf);     \
        fmpq_clear(b);                    \
        FUN1(a, a, other.a, nf);          \
    }                                     \
    else                                  \
    {                                     \
        /* all rationals */               \
        FUN4(b, b, other.b);              \
    }                                     \
    return *this;                         \
}                                         \
renf_elem_class renf_elem_class::operator OP (const renf_elem_class & other) const \
{                                         \
    renf_elem_class ans(*this);           \
    ans INOP other;                       \
    return ans;                           \
}
__renf_elem_op(+, +=, renf_elem_add, renf_elem_add_fmpq, renf_elem_fmpq_add, fmpq_add);
__renf_elem_op(*, *=, renf_elem_mul, renf_elem_mul_fmpq, renf_elem_fmpq_add, fmpq_mul);
__renf_elem_op(-, -=, renf_elem_sub, renf_elem_sub_fmpq, renf_elem_fmpq_sub, fmpq_sub);
__renf_elem_op(/, /=, renf_elem_div, renf_elem_div_fmpq, renf_elem_fmpq_div, fmpq_div);
#undef __renf_elem_op


#define __renf_elem_op(TYP, OP, INOP) \
renf_elem_class& renf_elem_class::operator INOP (const TYP other) \
{                                         \
    renf_elem_class x(other);             \
    *this INOP x;                         \
    return *this;                         \
}                                         \
renf_elem_class renf_elem_class::operator OP (const TYP other) const \
{                                         \
    renf_elem_class x(*this);             \
    x INOP other;                         \
    return x;                             \
}                                         \
renf_elem_class operator OP (const TYP a, const renf_elem_class& b) \
{                                         \
    renf_elem_class x(a);                 \
    x INOP b;                             \
    return x;                             \
}

__renf_elem_op(int, +, +=);
__renf_elem_op(unsigned int, +, +=);
__renf_elem_op(long, +, +=);
__renf_elem_op(unsigned long, +, +=);
__renf_elem_op(mpz_class&, +, +=);
__renf_elem_op(mpq_class&, +, +=);


__renf_elem_op(int, -, -=);
__renf_elem_op(unsigned int, -, -=);
__renf_elem_op(long, -, -=);
__renf_elem_op(unsigned long, -, -=);
__renf_elem_op(mpz_class&, -, -=);
__renf_elem_op(mpq_class&, -, -=);

__renf_elem_op(int, *, *=);
__renf_elem_op(unsigned int, *, *=);
__renf_elem_op(long, *, *=);
__renf_elem_op(unsigned long, *, *=);
__renf_elem_op(mpz_class&, *, *=);
__renf_elem_op(mpq_class&, *, *=);

__renf_elem_op(int, /, /=);
__renf_elem_op(unsigned int, /, /=);
__renf_elem_op(long, /, /=);
__renf_elem_op(unsigned long, /, /=);
__renf_elem_op(mpz_class&, /, /=);
__renf_elem_op(mpq_class&, /, /=);

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
            throw 42;
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

bool renf_elem_class::operator>(const renf_elem_class & other) const
{
    if (this->nf != NULL)
    {
        if (this->nf == other.nf)
            return renf_elem_cmp(this->a, other.a, this->nf) > 0;
        else if (other.nf == NULL)
            return renf_elem_cmp_fmpq(this->a, other.b, this->nf) > 0;
        else
            throw 42;
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

#define __other_ops(TYP) \
bool renf_elem_class::operator == (const TYP other) const \
{                               \
    renf_elem_class x(other);   \
    return (*this) == x;        \
}                               \
bool renf_elem_class::operator > (const TYP other) const \
{                               \
    renf_elem_class x(other);   \
    return (*this) > x;         \
}
__other_ops(int);
__other_ops(unsigned int);
__other_ops(long);
__other_ops(unsigned long);
__other_ops(mpz_class&);
__other_ops(mpq_class&);
#undef __other_ops

#define __other_ops(TYP) \
  bool renf_elem_class::operator!=(const TYP other) const {return not (*this == other);} \
  bool renf_elem_class::operator>=(const TYP other) const {return *this == other || *this > other;} \
  bool renf_elem_class::operator<=(const TYP other) const {return not (*this > other);} \
  bool renf_elem_class::operator< (const TYP other) const {return not (*this >= other);}

__other_ops(int);
__other_ops(unsigned int);
__other_ops(long);
__other_ops(unsigned long);
__other_ops(renf_elem_class&);
__other_ops(mpz_class&);
__other_ops(mpq_class&);
#undef __other_ops

#endif

