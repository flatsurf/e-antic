/*  This is a -*- C++ -*- header file.

    Copyright (C) 2016-2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef NF_EMB_ELEMXX_H
#define NF_EMB_ELEMXX_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <gmpxx.h>

#include "renf.h"
#include "renf_elem.h"

class renf_class; /* forward declaration */

class renf_elem_class
{
private:
    mutable renf_class * nf;  /* not owned reference to a number field */
    mutable renf_elem_t a;    /* the element */
    mutable fmpq_t b;         /* rational value when nf == NULL */
public:
    // construction from rational elements
    // (results will be rationals)
    renf_elem_class();
    renf_elem_class(const fmpz_t);
    renf_elem_class(const fmpq_t);

    // construction with given number field
    renf_elem_class(renf_class&);
    renf_elem_class(renf_class&, const std::string);
    renf_elem_class(renf_class&, const fmpq_poly_t);

    // construction from another element
    renf_elem_class(const renf_elem_class&);

    ~renf_elem_class();

    // the underlying number field
    renf_class& parent(void);

    // assignment
    renf_elem_class& operator = (const fmpz_t);
    renf_elem_class& operator = (const fmpq_t);
    renf_elem_class& operator = (const fmpq_poly_t&);
    renf_elem_class& operator = (const renf_elem_class&);
    renf_elem_class& operator = (const std::string);

    // testing
    bool is_fmpq(void);
    bool is_zero();
    bool is_one();
    bool is_integer();
    bool is_rational();

    // data access
    fmpq * get_fmpq(void);
    renf_elem_srcptr get_renf_elem(void);
    mpz_class get_den(void);
    mpz_class get_num(void);
    std::vector<mpz_class> get_num_vector(void);

    // floor, ceil, round
    mpz_class floor() const;
    mpz_class ceil() const;

    // input, output
    friend std::ostream& operator << (std::ostream &, const renf_elem_class&);
    friend std::istream& operator >> (std::istream &, renf_elem_class&);

    // unary operations
    renf_elem_class operator - () const;
    renf_elem_class operator + () const;

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

    // macro declaration of
    // - constructor
    // - assignment
    // - binary operations
    // - inplace binary operations
    // - comparisons
    // for int, uint, long, ulong, mpz_class, mpq_class
    #define __renf_ops(TYP) \
    renf_elem_class(const TYP); \
    renf_elem_class(renf_class&, const TYP); \
    renf_elem_class& operator = (const TYP); \
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
    friend bool operator == (const TYP, const renf_elem_class &); \
    friend bool operator != (const TYP, const renf_elem_class &); \
    friend bool operator >= (const TYP, const renf_elem_class &); \
    friend bool operator <= (const TYP, const renf_elem_class &); \
    friend bool operator > (const TYP, const renf_elem_class &); \
    friend bool operator < (const TYP, const renf_elem_class &);

    __renf_ops(int);
    __renf_ops(unsigned int);
    __renf_ops(long);
    __renf_ops(unsigned long);
    __renf_ops(mpz_class&);
    __renf_ops(mpq_class&);
    #undef __renf_ops
};


class renf_class
{
    renf_t nf;
public:
    renf_class();
    renf_class(renf_t);
    renf_class(const renf_class&);
    renf_class(const std::string, const slong prec=64);
    renf_class(const std::string, const std::string, const slong prec=64);

    ~renf_class();

    const renf_srcptr get_renf() { return nf; }

    friend std::ostream& operator << (std::ostream &, const renf_class&);
    friend std::istream& operator >> (std::istream &, renf_class&);

    // standard elements
    renf_elem_class zero();
    renf_elem_class one();
    renf_elem_class gen();
};

/**/
/**/

inline mpz_class floor(renf_elem_class x) { return x.floor(); }
inline mpz_class ceil(renf_elem_class x) { return x.ceil(); }


/*********/
/* utils */
/*********/

/* read a number field from a string */
/* formatting "minpoly LEN coeff0 coeff1 ... coeffl embedding [mid +/- rad]" */
inline std::istream& parse_nf_stream(fmpq_poly_t minpoly, arb_t emb, std::istream& in)
{
    char c;
    int err;
    std::string s;
    std::string t;

    in >> s;
    if (s != "minpoly")
        throw std::ios_base::failure("Error in reading number field: expected keyword minpoly");

    /* go to the next non-space character */
    c = in.peek();
    while (c == ' ')
    {
        in.get(c);
        c = in.peek();
    }

    /* read the polynomial */
    while(true)
    {
        c = in.peek();
        if (c == 'e')
            break;
        in.get(c);
        t += c;
    }
    while (t.back() == ' ') t.pop_back();
    err = fmpq_poly_set_str_magic(minpoly, t.c_str());
    if (err)
        throw std::ios_base::failure("Error in reading minimal polynomial");

    in >> s;
    if (s != "embedding")
        throw std::ios_base::failure("Error in reading number field: expected keyword embedding");

    /* ignore spaces */
    c = in.peek();
    while (c == ' ')
    {
        in.get(c);
        c = in.peek();
    }

    /* read embedding */
    t.clear();
    c = in.peek();
    if (c == '[')
    {
        while(c != ']' and c != '\0')
        {
            in.get(c);
            t += c;
            if (c == ']')
                break;
        }
    }
    else
    {
        in >> t;
    }

    arb_init(emb);
    int error = arb_set_str(emb, t.c_str(), 10);
    if (error)
        throw std::ios_base::failure("Error in reading number field: bad formatting of embedding " + t);

    return in;
}


/*****************************/
/* renf_class implementation */
/*****************************/

inline renf_class::renf_class()
{
    fmpq_poly_t minpoly;
    arb_t emb;

    fmpq_poly_init(minpoly);
    fmpq_poly_set_coeff_si(minpoly, 0, -1);
    fmpq_poly_set_coeff_si(minpoly, 1, 1);

    arb_init(emb);
    arb_set_si(emb, 1);

    renf_init(nf, minpoly, emb, 64);

    fmpq_poly_clear(minpoly);
    arb_clear(emb);
}

inline renf_class::renf_class(renf_t k)
{
    renf_init_set(nf, k);
}

inline renf_class::renf_class(const renf_class& other)
{
    renf_init_set(nf, other.nf);
}

inline renf_class::renf_class(const std::string str, const slong prec)
{
    fmpq_poly_t minpoly;
    arb_t emb;
    std::istringstream in;

    in.str(str);

    fmpq_poly_init(minpoly);
    arb_init(emb);

    parse_nf_stream(minpoly, emb, in);
    renf_init(nf, minpoly, emb, prec);

    fmpq_poly_clear(minpoly);
    arb_clear(emb);
}

inline renf_class::renf_class(const std::string poly_str, const std::string emb_str, const slong prec)
{
    fmpq_poly_t minpoly;
    arb_t emb;
    int err;

    fmpq_poly_init(minpoly);
    err = fmpq_poly_set_str_magic(minpoly, poly_str.c_str());
    if (err)
    {
        fmpq_poly_clear(minpoly);
        throw std::ios_base::failure("Error in reading minimal polynomial");
    }

    arb_init(emb);
    int error = arb_set_str(emb, emb_str.c_str(), 10);
    if (error)
    {
        fmpq_poly_clear(minpoly);
        arb_clear(emb);
        throw std::ios_base::failure("Error in reading number field: bad formatting of embedding");
    }

    renf_init(nf, minpoly, emb, prec);

    fmpq_poly_clear(minpoly);
    arb_clear(emb);
}

inline renf_class::~renf_class()
{
    if (nf != NULL) renf_clear(nf);
}

inline std::ostream& operator << (std::ostream & os, const renf_class& nf)
{
    char *res;

    res = fmpq_poly_get_str(nf.nf->nf->pol);
    os << "minpoly "<< res;
    flint_free(res);

    res = arb_get_str(nf.nf->emb, 64, 0);
    os << " embedding " << res << std::endl;
    flint_free(res);

    return os;
}

inline std::istream& operator >> (std::istream & is, renf_class& a)
{
    fmpq_poly_t minpoly;
    arb_t emb;

    fmpq_poly_init(minpoly);
    arb_init(emb);

    parse_nf_stream(minpoly, emb, is);
    renf_clear(a.nf);
    renf_init(a.nf, minpoly, emb, 64);

    fmpq_poly_clear(minpoly);
    arb_clear(emb);

    return is;
}

inline renf_elem_class renf_class::zero()
{
    renf_elem_class a(*this, 0);
    return a;
}

inline renf_elem_class renf_class::one()
{
    renf_elem_class a(*this, 1);
    return a;
}

renf_elem_class renf_class::gen()
{
    renf_elem_class a(*this);
    renf_elem_gen(a.get_renf_elem(), this->get_renf());
    return a;
}

/**********************************/
/* renf_elem_class implementation */
/**********************************/

inline renf_elem_class::renf_elem_class()
{
    nf = NULL;
    fmpq_init(b);
    fmpq_zero(b);
}
inline renf_elem_class::renf_elem_class(renf_class& k)
{
    nf = &k;
    renf_elem_init(a, nf->get_renf());
    renf_elem_zero(a, nf->get_renf());
}

inline renf_elem_class::renf_elem_class(int x)
{
    nf = NULL;
    fmpq_init(b);
    fmpq_set_si(b, x, 1);
}
inline renf_elem_class::renf_elem_class(renf_class& k, int x)
{
    nf = &k;
    renf_elem_init(a, nf->get_renf());
    renf_elem_set_si(a, x, nf->get_renf());
}

inline renf_elem_class::renf_elem_class(unsigned int x)
{
    nf = NULL;
    fmpq_init(b);
    fmpz_set_ui(fmpq_numref(b), x);
    fmpz_one(fmpq_denref(b));
}
inline renf_elem_class::renf_elem_class(renf_class& k, unsigned int x)
{
    nf = &k;
    renf_elem_init(a, nf->get_renf());
    renf_elem_set_ui(a, x, nf->get_renf());
}

inline renf_elem_class::renf_elem_class(long x)
{
    nf = NULL;
    fmpq_init(b);
    fmpq_set_si(b, x, 1);
}
inline renf_elem_class::renf_elem_class(renf_class& k, long x)
{
    nf = &k;
    renf_elem_init(a, nf->get_renf());
    renf_elem_set_si(a, x, nf->get_renf());
}

inline renf_elem_class::renf_elem_class(unsigned long x)
{
    nf = NULL;
    fmpq_init(b);
    fmpz_set_ui(fmpq_numref(b), x);
    fmpz_one(fmpq_denref(b));
}
inline renf_elem_class::renf_elem_class(renf_class& k, unsigned long x)
{
    nf = &k;
    renf_elem_init(a, nf->get_renf());
    renf_elem_set_ui(a, x, nf->get_renf());
}

inline renf_elem_class::renf_elem_class(const mpz_class &x)
{
    nf = NULL;
    fmpq_init(b);
    fmpz_set_mpz(fmpq_numref(b), x.__get_mp());
    fmpz_one(fmpq_denref(b));
}
inline renf_elem_class::renf_elem_class(renf_class& k, const mpz_class &x)
{
    nf = &k;
    fmpz_t tmp;
    fmpz_init(tmp);
    fmpz_set_mpz(tmp, x.__get_mp());
    renf_elem_init(a, nf->get_renf());
    renf_elem_set_fmpz(a, tmp, nf->get_renf());
    fmpz_clear(tmp);
}

inline renf_elem_class::renf_elem_class(const mpq_class &x)
{
    nf = NULL;
    fmpq_init(b);
    fmpq_set_mpq(b, x.__get_mp());
}
inline renf_elem_class::renf_elem_class(renf_class &k, const mpq_class &x)
{
    nf = &k;
    fmpq_t tmp;
    fmpq_init(tmp);
    fmpq_set_mpq(tmp, x.__get_mp());
    renf_elem_init(a, nf->get_renf());
    renf_elem_set_fmpq(a, tmp, nf->get_renf());
    fmpq_clear(tmp);
}


inline renf_elem_class::renf_elem_class(renf_class& k, const fmpq_poly_t p)
{
    nf = &k;
    renf_elem_init(a, nf->get_renf());
    renf_elem_set_fmpq_poly(a, p, nf->get_renf());
}

inline renf_elem_class::renf_elem_class(renf_class& k, const std::string s)
{
    nf = &k;
    renf_elem_init(a, nf->get_renf());
    renf_elem_zero(a, nf->get_renf());
    std::istringstream(s) >> *this;
}


inline renf_elem_class::renf_elem_class(const fmpz_t x)
{
    nf = NULL;
    fmpq_init(b);
    fmpz_set(fmpq_numref(b), x);
    fmpz_one(fmpq_denref(b));
}

inline renf_elem_class::renf_elem_class(const fmpq_t x)
{
    nf = NULL;
    fmpq_init(b);
    fmpq_set(b, x);
}

inline renf_elem_class::renf_elem_class(const renf_elem_class& x)
{
    nf = x.nf;
    if (nf == NULL)
    {
        fmpq_init(b);
        fmpq_set(b, x.b);
    }
    else
    {
        renf_elem_init(a, nf->get_renf());
        renf_elem_set(a, x.a, nf->get_renf());
    }
}

inline renf_elem_class::~renf_elem_class(void)
{
    if (nf == NULL) fmpq_clear(b);
    else renf_elem_clear(a, nf->get_renf());
}

inline bool renf_elem_class::is_fmpq(void)
{
    return (nf == NULL);
}

inline fmpq * renf_elem_class::get_fmpq(void)
{
    if (not is_fmpq()) throw 42;
    else return b;
}

inline renf_elem_srcptr renf_elem_class::get_renf_elem(void)
{
    if (is_fmpq()) throw 42;
    else return a;
}

inline mpz_class renf_elem_class::get_den()
{
    mpz_class res;

    if (nf == NULL)
        fmpz_get_mpz(res.__get_mp(), fmpq_denref(b));
    else
    {
        fmpz_t z;
        fmpz_init(z);
        nf_elem_get_den(z, a->elem, nf->get_renf()->nf);
        fmpz_get_mpz(res.__get_mp(), z);
        fmpz_clear(z);
    }

    return res;
}

inline mpz_class renf_elem_class::get_num(void)
{
    mpz_class x;

    if (nf == NULL)
    {
        fmpz_get_mpz(x.__get_mp(), fmpq_numref(b));
        return x;
    }
    else if (nf->get_renf()->nf->flag & NF_LINEAR)
        fmpz_get_mpz(x.__get_mp(), LNF_ELEM_NUMREF(a->elem));
    else if (nf->get_renf()->nf->flag & NF_QUADRATIC)
    {
        if (! fmpz_is_zero(QNF_ELEM_NUMREF(a->elem) + 1))
            throw 42;
        fmpz_get_mpz(x.__get_mp(), QNF_ELEM_NUMREF(a->elem));
    }
    else
    {
        if (fmpq_poly_length(NF_ELEM(a->elem)) == 0)
            mpz_set_si(x.__get_mp(), 0);
        else if (fmpq_poly_length(NF_ELEM(a->elem)) == 1)
            fmpz_get_mpz(x.__get_mp(), NF_ELEM_NUMREF(a->elem));
        else
            throw 42;
    }

    return x;
}

inline std::vector<mpz_class> renf_elem_class::get_num_vector(void)
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
        nf_elem_get_fmpq_poly(f, a->elem, nf->get_renf()->nf);
        for (size_t i = 0; i < fmpq_poly_length(f); i++)
        {
            fmpz_get_mpz(x.__get_mp(), fmpq_poly_numref(f) + i);
            res.push_back(x);
        }
        size_t deg = fmpq_poly_length(nf->get_renf()->nf->pol);
        for (size_t i = fmpq_poly_length(f); i < deg; i++)
            res.push_back(mpz_class(0));
        fmpq_poly_clear(f);
    }

    return res;
}

inline renf_elem_class& renf_elem_class::operator = (const int n)
{
    if (nf == NULL) fmpq_set_si(b, n, 1);
    else renf_elem_set_si(a, n, nf->get_renf());
    return *this;
}
inline renf_elem_class& renf_elem_class::operator = (const long n)
{
    if (nf == NULL) fmpq_set_si(b, n, 1);
    else renf_elem_set_si(a, n, nf->get_renf());
    return *this;
}
inline renf_elem_class& renf_elem_class::operator = (const unsigned int n)
{
    if (nf == NULL)
    {
        fmpz_set_ui(fmpq_numref(b), n);
        fmpz_one(fmpq_denref(b));
    }
    else renf_elem_set_ui(a, n, nf->get_renf());
    return *this;
}
inline renf_elem_class& renf_elem_class::operator = (const unsigned long n)
{
    if (nf == NULL)
    {
        fmpz_set_ui(fmpq_numref(b), n);
        fmpz_one(fmpq_denref(b));
    }
    else renf_elem_set_ui(a, n, nf->get_renf());
    return *this;
}
inline renf_elem_class& renf_elem_class::operator = (const fmpz_t z)
{
    if (nf == NULL)
    {
        fmpz_set(fmpq_numref(b), z);
        fmpz_one(fmpq_denref(b));
    }
    else renf_elem_set_fmpz(a, z, nf->get_renf());
    return *this;
}
inline renf_elem_class& renf_elem_class::operator = (const fmpq_t q)
{
    if (nf == NULL) fmpq_set(b, q);
    else renf_elem_set_fmpq(a, q, nf->get_renf());
    return *this;
}
inline renf_elem_class& renf_elem_class::operator = (const fmpq_poly_t& p)
{
    if (nf == NULL) throw 42;
    renf_elem_set_fmpq_poly(a, p, nf->get_renf());
    return *this;
}
inline renf_elem_class& renf_elem_class::operator = (const mpz_class& z)
{
    fmpz_t x;
    fmpz_init(x);
    fmpz_set_mpz(x, z.get_mpz_t());
    *this = x;
    fmpz_clear(x);
    return *this;
}
inline renf_elem_class& renf_elem_class::operator = (const mpq_class& q)
{
    fmpq_t x;
    fmpq_init(x);
    fmpq_set_mpq(x, q.get_mpq_t());
    *this = x;
    fmpq_clear(x);
    return *this;
}
inline renf_elem_class& renf_elem_class::operator = (const renf_elem_class &x)
{
    if (x.nf == NULL)
    {
        if (nf != NULL)
        {
            renf_elem_clear(a, nf->get_renf());
            fmpq_init(b);
            nf = NULL;
        }
        fmpq_set(b, x.b);
    }
    else if (nf == NULL)
    {
        nf = x.nf;
        fmpq_clear(b);
        renf_elem_init(a, nf->get_renf());
        renf_elem_set(a, x.a, nf->get_renf());
    }
    else
    {
        renf_elem_clear(a, nf->get_renf());
        nf = x.nf;
        renf_elem_init(a, nf->get_renf());
        renf_elem_set(a, x.a, nf->get_renf());
    }

    return *this;
}
inline renf_elem_class& renf_elem_class::operator = (const std::string s)
{
    if (nf == NULL)
        fmpq_set_str(b, s.c_str(), 10);
    else
    {
        fmpq_poly_t p;
        int err;

        err = fmpq_poly_set_str_magic(p, s.c_str());
        if (err) throw 10;
        renf_elem_set_fmpq_poly(a, p, nf->get_renf());
        fmpq_poly_clear(p);
    }
}

// I/O

inline std::ostream& operator << (std::ostream & os, const renf_elem_class& a)
{
    char * res;
    if (a.nf == NULL)
        res = fmpq_get_str(NULL, 10, a.b);
    else
    {
        // TODO: make a recursive evaluation so that we have at least
        // a certain amount of significant digits
        renf_elem_set_evaluation(a.a, a.nf->get_renf(), 64);
        res = renf_elem_get_str_pretty(a.a, "a", a.nf->get_renf(), 5);
    }
    os << res;
    flint_free(res);
    return os;
}

inline std::istream& operator >> (std::istream & is, renf_elem_class& a)
{
    std::string t;

    if (a.nf == NULL)
    {
        /* read a rational */
        is >> t;
        mpq_class b(t);
        a = b;
    }
    else
    {
        /* read a number field element (given as a list of rationals) */
        fmpq_poly_t p;
        mpq_class b;
        size_t i;

        fmpq_poly_init(p);
        for (i = 0; i < fmpq_poly_length(a.nf->get_renf()->nf->pol) - 1; i++)
        {
            is >> t;
            b = t;
            fmpq_poly_set_coeff_mpq(p, i, b.get_mpq_t());
            t.clear();

            if (is.eof())
                break;
        }

        a = p;
        fmpq_poly_clear(p);
    }

    return is;
}

// Arithmetic

inline renf_elem_class renf_elem_class::operator - () const
{
    renf_elem_class ans(*this);
    if (nf == NULL) fmpq_neg(ans.b, ans.b);
    else renf_elem_neg(ans.a, ans.a, ans.nf->get_renf());
    return ans;
}
inline renf_elem_class renf_elem_class::operator + () const
{
    return *this;
}

static inline void renf_elem_fmpq_add(renf_elem_t a, fmpq_t b, renf_elem_t c, renf_t d) {renf_elem_add_fmpq(a, c, b, d);}
static inline void renf_elem_fmpq_mul(renf_elem_t a, fmpq_t b, renf_elem_t c, renf_t d) {renf_elem_mul_fmpq(a, c, b, d);}

#define __renf_elem_op(OP, INOP, FUN1, FUN2, FUN3, FUN4) \
inline renf_elem_class& renf_elem_class::operator INOP (const renf_elem_class & other) \
{                                                 \
    if (nf != NULL)                               \
    {                                             \
        if (nf == other.nf)                       \
            FUN1(a, a, other.a, nf->get_renf());  \
        else if (other.nf == NULL)                \
            FUN2(a, a, other.b, nf->get_renf());  \
        else                                      \
            throw 42;                             \
    }                                             \
    else if (other.nf != NULL)                    \
    {                                             \
        /* promote to nf elt */                   \
        nf = other.nf;                            \
        renf_elem_init(a, nf->get_renf());        \
        renf_elem_set_fmpq(a, b, nf->get_renf()); \
        fmpq_clear(b);                            \
        FUN1(a, a, other.a, nf->get_renf());      \
    }                                             \
    else                                          \
    {                                             \
        /* all rationals */                       \
        FUN4(b, b, other.b);                      \
    }                                             \
    return *this;                                 \
}                                                 \
inline renf_elem_class renf_elem_class::operator OP (const renf_elem_class & other) const \
{                                         \
    renf_elem_class ans(*this);           \
    ans INOP other;                       \
    return ans;                           \
}
__renf_elem_op(+, +=, renf_elem_add, renf_elem_add_fmpq, renf_elem_fmpq_add, fmpq_add);
__renf_elem_op(*, *=, renf_elem_mul, renf_elem_mul_fmpq, renf_elem_fmpq_mul, fmpq_mul);
__renf_elem_op(-, -=, renf_elem_sub, renf_elem_sub_fmpq, renf_elem_fmpq_sub, fmpq_sub);
__renf_elem_op(/, /=, renf_elem_div, renf_elem_div_fmpq, renf_elem_fmpq_div, fmpq_div);
#undef __renf_elem_op


#define __renf_elem_op(TYP, OP, INOP) \
inline renf_elem_class& renf_elem_class::operator INOP (const TYP other) \
{                                         \
    renf_elem_class x(other);             \
    *this INOP x;                         \
    return *this;                         \
}                                         \
inline renf_elem_class renf_elem_class::operator OP (const TYP other) const \
{                                         \
    renf_elem_class x(*this);             \
    x INOP other;                         \
    return x;                             \
}                                         \
inline renf_elem_class operator OP (const TYP a, const renf_elem_class& b) \
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
// not seriously defined
// __renf_elem_op(unsigned int, -, -=);
__renf_elem_op(long, -, -=);
// not seriously defined
//__renf_elem_op(unsigned long, -, -=);
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


inline bool renf_elem_class::operator == (const renf_elem_class& other) const
{
    if (nf != NULL)
    {
        if (nf == other.nf)
            return renf_elem_equal(a, other.a, nf->get_renf());
        else if (other.nf == NULL)
            return renf_elem_equal_fmpq(a, other.b, nf->get_renf());
        else
            throw 42;
    }
    else if (other.nf == NULL)
        return fmpq_equal(b, other.b);
    else
        return renf_elem_equal_fmpq(other.a, b, other.nf->get_renf());
}

inline bool renf_elem_class::operator > (const renf_elem_class & other) const
{
    if (nf != NULL)
    {
        if (nf == other.nf)
            return renf_elem_cmp(a, other.a, nf->get_renf()) > 0;
        else if (other.nf == NULL)
            return renf_elem_cmp_fmpq(a, other.b, nf->get_renf()) > 0;
        else
            throw 42;
    }
    else if (other.nf == NULL)
        return fmpq_cmp(b, other.b) > 0;
    else
        return renf_elem_cmp_fmpq(other.a, b, other.nf->get_renf()) < 0;
}

inline bool renf_elem_class::operator != (const renf_elem_class & other) const
{
    return not (*this == other);
}
inline bool renf_elem_class::operator >= (const renf_elem_class & other) const
{
    return (*this > other) || (*this == other);
}
inline bool renf_elem_class::operator < (const renf_elem_class & other) const
{
    return not (*this >= other);
}
inline bool renf_elem_class::operator <= (const renf_elem_class & other) const
{
    return not (* this > other);
}

/* TODO: the code below would result in something rather slow as each operation */
/* involves creating a renf_elem_class (that can easily be avoided)             */

#define __other_ops(TYP, OP) \
inline bool renf_elem_class::operator OP (const TYP other) const \
{                               \
    renf_elem_class x(other);   \
    return (*this) OP x;        \
}                               \
inline bool operator OP (const TYP left, const renf_elem_class& other) \
{                               \
    renf_elem_class x(left);    \
    return x OP other;          \
}
#define __all_other_ops(TYP) \
__other_ops(TYP, ==) \
__other_ops(TYP, !=) \
__other_ops(TYP, >)  \
__other_ops(TYP, <)  \
__other_ops(TYP, >=) \
__other_ops(TYP, <=)

__all_other_ops(int)
__all_other_ops(unsigned int)
__all_other_ops(long)
__all_other_ops(unsigned long)
__all_other_ops(mpz_class&)
__all_other_ops(mpq_class&)

#undef __other_ops
#undef __all_other_ops

inline bool renf_elem_class::is_zero()
{
    if (nf == NULL)
        return fmpq_is_zero(b);
    else
        return renf_elem_is_zero(a, nf->get_renf());
}

inline bool renf_elem_class::is_one()
{
    if (nf == NULL)
        return fmpq_is_one(b);
    else
        return renf_elem_is_one(a, nf->get_renf());
}

inline bool renf_elem_class::is_rational()
{
    if (nf == NULL)
        return true;
    else
        return renf_elem_is_rational(a, nf->get_renf());
}

inline bool renf_elem_class::is_integer()
{
    if (nf == NULL)
        return fmpz_is_one(fmpq_denref(b));
    else
        return renf_elem_is_integer(a, nf->get_renf());
}

inline mpz_class renf_elem_class::floor() const
{
    fmpz_t tmp;
    fmpz_init(tmp);

    if (nf == NULL) fmpz_fdiv_q(tmp, fmpq_numref(b), fmpq_denref(b));
    else renf_elem_floor(tmp, a, nf->get_renf());

    mpz_class z;
    fmpz_get_mpz(z.get_mpz_t(), tmp);
    fmpz_clear(tmp);
    return z;
}

inline mpz_class renf_elem_class::ceil() const
{
    fmpz_t tmp;
    fmpz_init(tmp);

    if (nf == NULL)
    {
        fmpz_add(tmp, fmpq_numref(b), fmpq_denref(b));
        fmpz_sub_ui(tmp, tmp, 1);
        fmpz_fdiv_q(tmp, tmp, fmpq_denref(b));
    }
    else renf_elem_ceil(tmp, a, nf->get_renf());

    mpz_class z;
    fmpz_get_mpz(z.get_mpz_t(), tmp);
    fmpz_clear(tmp);
    return z;
}

#endif
