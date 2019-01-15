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

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

#include <gmpxx.h>

#include <e-antic/renf.h>
#include <e-antic/renf_elem.h>

class renf_elem_class; /* forward declaration */

class renf_class
{
    renf_t nf;
public:

    std::string gen_name;

    renf_class();
    renf_class(renf_t);
    renf_class(renf_t, const std::string);
    renf_class(const char * pol, const char * var, const char * emb, const slong prec=64);
    renf_class(const std::string& pol, const std::string& var, const std::string emb, const slong prec=64);

    ~renf_class();

    inline renf_class& operator = (const renf_class& k)
    {
        renf_clear(nf);
        renf_init_set(nf, k.nf);
        return *this;
    }

    renf * get_renf() { return nf; }

    // standard elements
    renf_elem_class zero();
    renf_elem_class one();
    renf_elem_class gen();

    bool operator == (const renf_class&) const;
    bool operator != (const renf_class&) const;

    // I/O
    static int xalloc();
    std::istream& set_istream(std::istream&);
};

class renf_elem_class
{
private:
    mutable renf_class * nf;  /* not owned reference to a number field or NULL */
    mutable renf_elem_t a;    /* the element when nf != NULL */
    mutable fmpq_t b;         /* rational value when nf == NULL */

    void reset_parent(renf_class * p);

    void assign(const int);
    void assign(const unsigned int);
    void assign(const slong);
    void assign(const ulong);

    void assign(const fmpz_t);
    void assign(const mpz_t);
    void assign(const mpz_class&);

    void assign(const fmpq_t);
    void assign(const mpq_t);
    void assign(const mpq_class&);

    void assign(const fmpq_poly_t);

    void assign(const char *);
    void assign(const std::string);
    void assign(std::istream& is);

    void assign(const renf_elem_class&);

public:
    // constructors and destructor
    renf_elem_class();
    ~renf_elem_class();
    renf_elem_class(renf_class& k);
    renf_elem_class(const renf_elem_class& x);

    // passed by values
    #define __constructor(TYP)           \
    inline renf_elem_class(const TYP z)  \
    {                                    \
        nf = NULL;                       \
        fmpq_init(b);                    \
        assign(z);                       \
    }                                    \
    inline renf_elem_class(renf_class &k, const TYP z) \
    {                                      \
        nf = &k;                           \
        renf_elem_init(a, nf->get_renf()); \
        assign(z);                         \
    };
    __constructor(int);
    __constructor(unsigned int);
    __constructor(slong);
    __constructor(ulong);
    __constructor(mpz_t);
    __constructor(mpq_t);
    __constructor(mpz_class&);
    __constructor(mpq_class&);
    __constructor(fmpz_t);
    __constructor(fmpq_t);
    __constructor(fmpq_poly_t);
    __constructor(std::string);
    #undef __constructor

    // underlying number field
    renf_class& parent(void) const { return *nf; };

    // assignment
    #define __eq(TYP) \
    inline renf_elem_class& operator = (const TYP n) \
    {                                                \
        this->assign(n);                             \
        return *this;                                \
    };
    __eq(int);
    __eq(unsigned int);
    __eq(slong);
    __eq(ulong);
    __eq(mpz_class&);
    __eq(mpq_class&);
    __eq(renf_elem_class&);
    #undef __eq

    // testing
    bool is_fmpq(void) const;
    bool is_zero() const;
    bool is_one() const;
    bool is_integer() const;
    bool is_rational() const;

    // data access
    fmpq * get_fmpq(void) const;
    renf_elem_srcptr get_renf_elem(void) const;
    mpz_class get_den(void) const;
    mpz_class get_num(void) const;
    std::vector<mpz_class> get_num_vector(void) const;

    // floor, ceil, round, approximation
    mpz_class floor() const;
    mpz_class ceil() const;
    int sgn() const;
    double get_d() const;

    // unary operations
    renf_elem_class operator - () const;
    renf_elem_class operator + () const;

    // string output
    std::string get_str(int flag=EANTIC_STR_ALG|EANTIC_STR_D) const;

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
    // for int, slong, mpz_class, mpq_class
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
    friend bool operator == (const TYP, const renf_elem_class &); \
    friend bool operator != (const TYP, const renf_elem_class &); \
    friend bool operator >= (const TYP, const renf_elem_class &); \
    friend bool operator <= (const TYP, const renf_elem_class &); \
    friend bool operator > (const TYP, const renf_elem_class &); \
    friend bool operator < (const TYP, const renf_elem_class &);

    __renf_ops(int);
    __renf_ops(unsigned int);
    __renf_ops(slong);
    __renf_ops(unsigned long);
    __renf_ops(fmpz_t);
    __renf_ops(fmpq_t);
    __renf_ops(mpz_t);
    __renf_ops(mpq_t);
    __renf_ops(mpz_class&);
    __renf_ops(mpq_class&);
    #undef __renf_ops

    /* I/O */
    friend std::ostream& operator << (std::ostream &, const renf_elem_class&);
    friend std::istream& operator >> (std::istream &, renf_elem_class&);
};

/*****************/
/* I/O operators */
/*****************/

inline std::istream& renf_class::set_istream(std::istream& is)
{
    is.pword(renf_class::xalloc()) = this;
    return is;
}

inline std::ostream& operator << (std::ostream& os, const renf_elem_class& a)
{
    os << a.get_str();
    return os;
}

inline std::istream& operator >> (std::istream& is, renf_elem_class& a)
{
    renf_class * nf = (renf_class *) is.pword(renf_class::xalloc());

    if (nf != NULL)
        // reset the number field with the one from the stream
        a.reset_parent(nf);

    a.assign(is);
    return is;
}

/*********************/
/* function overload */
/*********************/

inline mpz_class floor(renf_elem_class x) { return x.floor(); }
inline mpz_class ceil(renf_elem_class x) { return x.ceil(); }

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

    gen_name = "a";
}

inline renf_class::~renf_class()
{
    renf_clear(nf);
}

inline renf_class::renf_class(renf_t k)
{
    renf_init_set(nf, k);
    gen_name = "a";
}

inline renf_class::renf_class(renf_t k, const std::string gen_name)
{
    renf_init_set(nf, k);
    this->gen_name = gen_name;
}

inline renf_class::renf_class(const char * pol, const char * var, const char * emb, const slong prec)
{
    arb_t e;
    fmpq_poly_t p;

    fmpq_poly_init(p);
    fmpq_poly_set_str_pretty(p, pol, var);
    gen_name = var;

    arb_init(e);
    arb_set_str(e, emb, prec);

    renf_init(nf, p, e, prec);
    fmpq_poly_clear(p);
    arb_clear(e);
}

inline renf_class::renf_class(const std::string& pol, const std::string& var, const std::string emb, const slong prec)
{
    arb_t e;
    fmpq_poly_t p;

    fmpq_poly_init(p);
    fmpq_poly_set_str_pretty(p, pol.c_str(), var.c_str());
    gen_name = var;

    arb_init(e);
    arb_set_str(e, emb.c_str(), prec);

    renf_init(nf, p, e, prec);

    fmpq_poly_clear(p);
    arb_clear(e);
}

inline bool renf_class::operator == (const renf_class& other) const
{
    return renf_equal(this->nf, other.nf);
}

inline bool renf_class::operator != (const renf_class& other) const
{
    return not renf_equal(this->nf, other.nf);
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

inline renf_elem_class renf_class::gen()
{
    renf_elem_class a(*this);
    renf_elem_gen(a.get_renf_elem(), this->get_renf());
    return a;
}

/**********************************/
/* renf_elem_class implementation */
/**********************************/

inline void renf_elem_class::assign(const fmpz_t z)
{
    if (nf == NULL)
    {
        fmpz_one(fmpq_denref(b));
        fmpz_set(fmpq_numref(b), z);
    }
    else
        renf_elem_set_fmpz(a, z, nf->get_renf());
}

inline void renf_elem_class::assign(const mpz_t z)
{
    if (nf == NULL)
    {
        fmpz_one(fmpq_denref(b));
        fmpz_set_mpz(fmpq_numref(b), z);
    }
    else
        renf_elem_set_mpz(a, z, nf->get_renf());
}

inline void renf_elem_class::assign(const mpz_class& z)
{
    this->assign(z.__get_mp());
}

inline void renf_elem_class::assign(const mpq_t z)
{
    if (nf == NULL)
        fmpq_set_mpq(b, z);
    else
        renf_elem_set_mpq(a, z, nf->get_renf());
}

inline void renf_elem_class::assign(const mpq_class& z)
{
    this->assign(z.__get_mp());
}

inline void renf_elem_class::assign(const fmpq_t z)
{
    if (nf == NULL)
        fmpq_set(b, z);
    else
        renf_elem_set_fmpq(a, z, nf->get_renf());
}

inline void renf_elem_class::assign(const slong z)
{
    if (nf == NULL)
    {
        fmpz_one(fmpq_denref(b));
        fmpz_set_si(fmpq_numref(b), z);
    }
    else
        renf_elem_set_si(a, z, nf->get_renf());
}

inline void renf_elem_class::assign(const int z)
{
    assign((slong) z);
}

inline void renf_elem_class::assign(const ulong z)
{
    if (nf == NULL)
    {
        fmpz_one(fmpq_denref(b));
        fmpz_set_ui(fmpq_numref(b), z);
    }
    else
        renf_elem_set_ui(a, z, nf->get_renf());
}

inline void renf_elem_class::assign(const unsigned int z)
{
    assign((ulong) z);
}

inline void renf_elem_class::assign(const fmpq_poly_t p)
{
    if (nf == NULL)
        throw std::invalid_argument("renf_elem_class: can not assign from polynomial if number field not set");
    renf_elem_set_fmpq_poly(a, p, nf->get_renf());
}

inline void renf_elem_class::assign(std::istream& is)
{
    std::string s;          /* part of the stream to use */
    char c;                 /* current character in the stream */

    std::string g;
    if (nf == NULL)
        g = "";
    else
        g = parent().gen_name;
    bool after_g = false; /* whether we just read a variable name */
    while (!is.eof())
    {
        c = is.peek();

        if (isdigit(c) || c == '+' || c == '-' || c == '*' || c == '^' || c == ' ' || c == '/')
        {
            if (after_g && c != ' ' && c != '^' && c != '-' && c != '+') // not allowed carachter after variable
                throw std::invalid_argument("wrong character after generator");
            s += c;
            is.get();
            after_g = false;
        }

        else if (nf != NULL && c == g[0])
        {
            if (after_g)
                throw -3;

            std::string::iterator j = g.begin();
            while (!is.eof() && j != g.end() && *j == c)
            {
                j++;
                is.get();
                c = is.peek();
            }

            if (j != g.end()) // not full variable name
                throw std::invalid_argument("variable not read in full");

            s += g;
            after_g = true;
        }

        else
            break;
    }

    // TODO: possibly read ~ double or ~ [arb]
    // the separator is considered to be ' ~ ' (exactly one space)
    assign(s);
}

inline void renf_elem_class::assign(const char * s)
{
    int err;
    const char * i = strchr(s, '~');
    char * t;

    if (i != NULL)
    {
        t = (char *) flint_malloc((i - s + 1) * sizeof(char));
        strncpy(t, s, i - s);
        t[i-s] = '\0';
    }
    else
    {
        t = (char *) flint_malloc((strlen(s) + 1) * sizeof(char));
        strcpy(t, s);
    }

    if (nf == NULL)
    {
        err = fmpq_set_str(b, t, 10);
        if (err)
            throw std::invalid_argument("renf_elem_class fmpq_set_str");
    }
    else
    {
        fmpq_poly_t p;

        fmpq_poly_init(p);
        err = fmpq_poly_set_str_pretty(p, t, nf->gen_name.c_str());
        if (err)
        {
            fmpq_poly_clear(p);
            throw std::invalid_argument("renf_elem_class fmpq_poly_set_str_pretty");
        }
        renf_elem_set_fmpq_poly(a, p, nf->get_renf());
        fmpq_poly_clear(p);
    }

    flint_free(t);
}

inline void renf_elem_class::reset_parent(renf_class * p)
{
    if (p == NULL)
    {
        if (nf != NULL)
        {
            renf_elem_clear(a, nf->get_renf());
            nf = NULL;
            fmpq_init(b);
        }
    }
    else
    {
        if (nf == NULL)
        {
            fmpq_clear(b);
            renf_elem_init(a, p->get_renf());
            nf = p;
        }
        else if (p != nf)
        {
            renf_elem_clear(a, nf->get_renf());
            renf_elem_init(a, p->get_renf());
            nf = p;
        }
    }
}

inline void renf_elem_class::assign(const renf_elem_class& x)
{
    reset_parent(x.nf);
    if (x.nf == NULL)
        fmpq_set(b, x.b);
    else
        renf_elem_set(a, x.a, nf->get_renf());
}

inline void renf_elem_class::assign(const std::string s)
{
    this->assign(s.c_str());
}

/* constructors destructor */

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

inline renf_elem_class::renf_elem_class(const renf_elem_class& x)
{
    nf = x.nf;
    if (nf == NULL)
        fmpq_init(b);
    else
        renf_elem_init(a, nf->get_renf());
    assign(x);
}

inline renf_elem_class::~renf_elem_class(void)
{
    if (nf == NULL) fmpq_clear(b);
    else renf_elem_clear(a, nf->get_renf());
}

/* testing */

inline bool renf_elem_class::is_fmpq(void) const
{
    return (nf == NULL);
}

inline fmpq * renf_elem_class::get_fmpq(void) const
{
    if (not is_fmpq())
        throw std::invalid_argument("renf_elem_class not a fmpq");
    else return b;
}

inline renf_elem_srcptr renf_elem_class::get_renf_elem(void) const
{
    if (is_fmpq())
        throw std::invalid_argument("renf_elem_class is a fmpq");
    else return a;
}

inline mpz_class renf_elem_class::get_den() const
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

inline mpz_class renf_elem_class::get_num(void) const
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
            throw std::invalid_argument("renf_elem_class not a rational");
        fmpz_get_mpz(x.__get_mp(), QNF_ELEM_NUMREF(a->elem));
    }
    else
    {
        if (fmpq_poly_length(NF_ELEM(a->elem)) == 0)
            mpz_set_si(x.__get_mp(), 0);
        else if (fmpq_poly_length(NF_ELEM(a->elem)) == 1)
            fmpz_get_mpz(x.__get_mp(), NF_ELEM_NUMREF(a->elem));
        else
            throw std::invalid_argument("renf_elem_class not a rational");
    }

    return x;
}

inline std::string renf_elem_class::get_str(int flag) const
{
    std::string s;

    // call to renf_elem_get_str_pretty
    if (nf == NULL)
    {
        char * t = fmpq_get_str(NULL, 10, b);
        s += t;
        flint_free(t);
    }
    else
    {
        char * t = renf_elem_get_str_pretty(get_renf_elem(),
                parent().gen_name.c_str(),
                parent().get_renf(),
                10,
                flag);
        s += t;
        flint_free(t);
    }

    return s;
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
            throw std::domain_error("arithmetic invalid on renf_elem_classes with different fields"); \
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


#define __renf_elem_op(TYP, OP, INOP, FUN1, FUN2) \
inline renf_elem_class& renf_elem_class::operator INOP (const TYP other) \
{                                            \
    if (nf == NULL)                          \
        FUN2(b, b, other);                   \
    else                                     \
        FUN1(a, a, other, nf->get_renf());   \
    return *this;                            \
}                                            \
inline renf_elem_class renf_elem_class::operator OP (const TYP other) const \
{ \
    renf_elem_class ans(*this);  \
    ans INOP other;              \
    return ans;                  \
} \
inline renf_elem_class operator OP (const TYP a, const renf_elem_class& b) \
{                                         \
    renf_elem_class x(a);                 \
    x INOP b;                             \
    return x;                             \
}

__renf_elem_op(slong, +, +=, renf_elem_add_si, fmpq_add_si);
__renf_elem_op(slong, -, -=, renf_elem_sub_si, fmpq_sub_si);
__renf_elem_op(slong, *, *=, renf_elem_mul_si, fmpq_mul_si);
__renf_elem_op(slong, /, /=, renf_elem_div_si, fmpq_div_si);
__renf_elem_op(ulong, +, +=, renf_elem_add_ui, fmpq_add_ui);
__renf_elem_op(ulong, -, -=, renf_elem_sub_ui, fmpq_sub_ui);
__renf_elem_op(ulong, *, *=, renf_elem_mul_ui, fmpq_mul_ui);
__renf_elem_op(ulong, /, /=, renf_elem_div_ui, fmpq_div_ui);
__renf_elem_op(fmpz_t, +, +=, renf_elem_add_fmpz, fmpq_add_fmpz);
__renf_elem_op(fmpz_t, -, -=, renf_elem_sub_fmpz, fmpq_sub_fmpz);
__renf_elem_op(fmpz_t, *, *=, renf_elem_mul_fmpz, fmpq_mul_fmpz);
__renf_elem_op(fmpz_t, /, /=, renf_elem_div_fmpz, fmpq_div_fmpz);
__renf_elem_op(fmpq_t, +, +=, renf_elem_add_fmpq, fmpq_add);
__renf_elem_op(fmpq_t, -, -=, renf_elem_sub_fmpq, fmpq_sub);
__renf_elem_op(fmpq_t, *, *=, renf_elem_mul_fmpq, fmpq_mul);
__renf_elem_op(fmpq_t, /, /=, renf_elem_div_fmpq, fmpq_div);
#undef __renf_elem_op

/* special mpz/mpq care */

#define __renf_gmp_op(MP_TYP, MPCPP_TYP, FLINT_TYP, FLINT_INIT, FLINT_SET, FLINT_CLEAR, INOP, OP) \
inline renf_elem_class& renf_elem_class::operator INOP (const MP_TYP z) \
{                                         \
    FLINT_TYP t;                          \
    FLINT_INIT(t);                        \
    FLINT_SET(t, z);                      \
    *this INOP t;                         \
    FLINT_CLEAR(t);                       \
    return *this;                         \
}                                         \
inline renf_elem_class renf_elem_class::operator OP (const MP_TYP z) const \
{                                         \
    renf_elem_class x(*this);             \
    x INOP z;                             \
    return x;                             \
}                                         \
inline renf_elem_class operator OP (const MP_TYP a, const renf_elem_class& b) \
{                                         \
    renf_elem_class x(a);                 \
    x INOP b;                             \
    return x;                             \
}                                         \
inline renf_elem_class& renf_elem_class::operator INOP (const MPCPP_TYP& other) \
{                                         \
    return (*this) INOP other.__get_mp(); \
}                                         \
inline renf_elem_class renf_elem_class::operator OP (const MPCPP_TYP& other) const \
{                                         \
    return (*this) OP other.__get_mp();   \
}                                         \
inline renf_elem_class operator OP (const MPCPP_TYP& a, const renf_elem_class& b) \
{                                         \
    return a.__get_mp() OP b;             \
}

__renf_gmp_op(mpz_t, mpz_class, fmpz_t, fmpz_init, fmpz_set_mpz, fmpz_clear, +=, +)
__renf_gmp_op(mpz_t, mpz_class, fmpz_t, fmpz_init, fmpz_set_mpz, fmpz_clear, -=, -)
__renf_gmp_op(mpz_t, mpz_class, fmpz_t, fmpz_init, fmpz_set_mpz, fmpz_clear, *=, *)
__renf_gmp_op(mpz_t, mpz_class, fmpz_t, fmpz_init, fmpz_set_mpz, fmpz_clear, /=, /)
__renf_gmp_op(mpq_t, mpq_class, fmpq_t, fmpq_init, fmpq_set_mpq, fmpq_clear, +=, +)
__renf_gmp_op(mpq_t, mpq_class, fmpq_t, fmpq_init, fmpq_set_mpq, fmpq_clear, -=, -)
__renf_gmp_op(mpq_t, mpq_class, fmpq_t, fmpq_init, fmpq_set_mpq, fmpq_clear, *=, *)
__renf_gmp_op(mpq_t, mpq_class, fmpq_t, fmpq_init, fmpq_set_mpq, fmpq_clear, /=, /)

#undef __renf_gmp_op

#define __renf_elem_op(TYP, OP, INOP) \

__renf_elem_op(mpz_class&, +, +=);
__renf_elem_op(mpz_class&, -, -=);
__renf_elem_op(mpz_class&, *, *=);
__renf_elem_op(mpz_class&, /, /=);

__renf_elem_op(mpq_class&, +, +=);
__renf_elem_op(mpq_class&, -, -=);
__renf_elem_op(mpq_class&, *, *=);
__renf_elem_op(mpq_class&, /, /=);

#undef __renf_elem_op

/* special int care */

#define __renf_int_op(INOP, OP) \
inline renf_elem_class& renf_elem_class::operator INOP (const int z) \
{ \
    *this INOP (slong) z; \
    return *this; \
} \
inline renf_elem_class& renf_elem_class::operator INOP (const unsigned int z) \
{ \
    *this INOP (ulong) z; \
    return *this; \
} \
inline renf_elem_class renf_elem_class::operator OP (const int other) const \
{ \
    return *this OP (slong) other; \
} \
inline renf_elem_class renf_elem_class::operator OP (const unsigned int other) const \
{ \
    return *this OP (slong) other; \
} \
inline renf_elem_class operator OP (const int a, const renf_elem_class& b) \
{                                         \
    return (slong) a OP b;                \
}                                         \
inline renf_elem_class operator OP (const unsigned int a, const renf_elem_class& b) \
{                                         \
    return (slong) a OP b;                \
}

__renf_int_op(*=, *)
__renf_int_op(+=, +)
__renf_int_op(-=, -)
__renf_int_op(/=, /)

#undef __renf_int_op

inline bool renf_elem_class::operator == (const renf_elem_class& other) const
{
    if (nf != NULL)
    {
        if (nf == other.nf)
        {
            return renf_elem_equal(a, other.a, nf->get_renf());
        }
        else if (other.nf == NULL)
            return renf_elem_equal_fmpq(a, other.b, nf->get_renf());
        else
            throw std::domain_error("can not compare renf_elem_class on different number fields");
    }
    else if (other.nf == NULL)
        return fmpq_equal(b, other.b);
    else
        return renf_elem_equal_fmpq(other.a, b, other.nf->get_renf());
}

inline bool renf_elem_class::operator != (const renf_elem_class & other) const
{
    return not (*this == other);
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
            throw std::domain_error("can not compare renf_elem_class on different number fields");
    }
    else if (other.nf == NULL)
        return fmpq_cmp(b, other.b) > 0;
    else
        return renf_elem_cmp_fmpq(other.a, b, other.nf->get_renf()) < 0;
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

__all_other_ops(slong)
__all_other_ops(int)
__all_other_ops(ulong)
__all_other_ops(unsigned int)
__all_other_ops(mpz_class&)
__all_other_ops(mpq_class&)

#undef __other_ops
#undef __all_other_ops

inline bool renf_elem_class::is_zero() const
{
    if (nf == NULL)
        return fmpq_is_zero(b);
    else
        return renf_elem_is_zero(a, nf->get_renf());
}

inline bool renf_elem_class::is_one() const
{
    if (nf == NULL)
        return fmpq_is_one(b);
    else
        return renf_elem_is_one(a, nf->get_renf());
}

inline bool renf_elem_class::is_rational() const
{
    if (nf == NULL)
        return true;
    else
        return renf_elem_is_rational(a, nf->get_renf());
}

inline bool renf_elem_class::is_integer() const
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

inline int renf_elem_class::sgn() const
{
    if (nf == NULL)
        return fmpq_sgn(b);
    else
        return renf_elem_sgn(a, nf->get_renf());
}

inline double renf_elem_class::get_d() const
{
    if (nf == NULL)
    {
        arb_t s;
        arb_init(s);
        arb_set_fmpq(s, b, 128);
        double ans = arf_get_d(arb_midref(s), ARF_RND_NEAR);
        arb_clear(s);
        return ans;
    }
    else
        return renf_elem_get_d(a, nf->get_renf(), ARF_RND_NEAR);
}

#endif
