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

#include <flint/fmpz.h>
#include <flint/fmpq.h>

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
    renf_class(renf_t, const std::string&);
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

    slong degree();

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
    mutable renf_class * nf;  /* not owned reference to a number field or nullptr */
    mutable renf_elem_t a;    /* the element when nf != nullptr */
    mutable fmpq_t b;         /* rational value when nf == nullptr */

    void reset_parent(renf_class * p);

    void assign_si(signed long int);
    void assign_ui(unsigned long int);

    void assign_fmpz(const fmpz_t);
    void assign_mpz(const mpz_t);
    void assign_mpz_class(const mpz_class&);

    void assign_fmpq(const fmpq_t);
    void assign_mpq(const mpq_t);
    void assign_mpq_class(const mpq_class&);

    void assign_mpz_vector(const std::vector<mpz_class>&);
    void assign_mpq_vector(const std::vector<mpq_class>&);
    void assign_fmpq_poly(const fmpq_poly_t);

    void assign_char_ptr(const char *);
    void assign_string(const std::string&);
    void assign_stream(std::istream& is);

    void assign_renf_elem_class(const renf_elem_class&);

public:
    // constructors and destructor
    renf_elem_class();
    ~renf_elem_class();
    explicit renf_elem_class(renf_class& k);
    renf_elem_class(const renf_elem_class& x);

    inline renf_elem_class& operator = (const renf_elem_class& x)
    {
        assign_renf_elem_class(x);
        return *this;
    }

    // passed by values
    #define __RENFXX_construct(TYP, ASSIGN) \
    inline renf_elem_class(const TYP z)  \
    {                                    \
        nf = nullptr;                       \
        fmpq_init(b);                    \
        ASSIGN(z);                       \
    }                                    \
    inline renf_elem_class(renf_class &k, const TYP z) \
    {                                      \
        nf = &k;                           \
        renf_elem_init(a, nf->get_renf()); \
        ASSIGN(z);                         \
    };                                     \
    inline renf_elem_class& operator = (const TYP n) \
    {                                                \
        ASSIGN(n);                                   \
        return *this;                                \
    };

    __RENFXX_construct(signed char, assign_si)
    __RENFXX_construct(signed int, assign_si)
    __RENFXX_construct(signed short int, assign_si)
    __RENFXX_construct(signed long int, assign_si)

    __RENFXX_construct(unsigned char, assign_ui)
    __RENFXX_construct(unsigned int, assign_ui)
    __RENFXX_construct(unsigned short int, assign_ui)
    __RENFXX_construct(unsigned long int, assign_ui)
    __RENFXX_construct(mpz_class&, assign_mpz_class)
    __RENFXX_construct(mpq_class&, assign_mpq_class)
    __RENFXX_construct(fmpq_poly_t, assign_fmpq_poly)
    explicit __RENFXX_construct(std::vector<mpz_class>&, assign_mpz_vector)
    explicit __RENFXX_construct(std::vector<mpq_class>&, assign_mpq_vector)
    explicit __RENFXX_construct(std::string&, assign_string)
    #undef __constructor

    // underlying number field
    renf_class& parent(void) const { return *nf; };

    // testing
    bool is_fmpq(void) const;
    bool is_zero() const;
    bool is_one() const;
    bool is_integer() const;
    bool is_rational() const;

    // raw data access
    fmpq * get_fmpq(void) const;
    renf_elem_srcptr get_renf_elem(void) const;

    // data conversion
    mpz_class get_den(void) const;
    mpz_class get_num(void) const;
    mpq_class get_rational(void) const;
    std::vector<mpz_class> get_num_vector(void) const;

    // gcd of numerator
    mpz_class num_content() const noexcept;

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

    // macro declaration for numerical types
    // - binary operations
    // - inplace binary operations
    // - comparisons
    #define __RENFXX_binop(TYP, OP, INOP) \
    renf_elem_class& operator INOP (TYP); \
    renf_elem_class operator OP (TYP) const; \
    friend renf_elem_class operator OP (TYP, const renf_elem_class &);

    #define __RENFXX_cmpop(TYP, OP) \
    bool operator OP (TYP) const; \
    friend bool operator OP (TYP, const renf_elem_class &);

    #define __RENFXX_ops(TYP)  \
    __RENFXX_binop(TYP, +, +=) \
    __RENFXX_binop(TYP, -, -=) \
    __RENFXX_binop(TYP, *, *=) \
    __RENFXX_binop(TYP, /, /=) \
    __RENFXX_cmpop(TYP, ==)    \
    __RENFXX_cmpop(TYP, !=)    \
    __RENFXX_cmpop(TYP, >=)    \
    __RENFXX_cmpop(TYP, <=)    \
    __RENFXX_cmpop(TYP, > )    \
    __RENFXX_cmpop(TYP, < )

    __RENFXX_ops(signed char)
    __RENFXX_ops(unsigned char)
    __RENFXX_ops(signed int)
    __RENFXX_ops(unsigned int)
    __RENFXX_ops(signed short int)
    __RENFXX_ops(unsigned short int)
    __RENFXX_ops(signed long int)
    __RENFXX_ops(unsigned long int)
    __RENFXX_ops(const mpz_class&)
    __RENFXX_ops(const mpq_class&)
    #undef __RENFXX_binop
    #undef __RENFXX_cmpop
    #undef __RENFXX_ops

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
    if (a.is_integer())
        os << a.get_str(EANTIC_STR_ALG);
    else
        os << a.get_str(EANTIC_STR_ALG | EANTIC_STR_D);
    return os;
}

inline std::istream& operator >> (std::istream& is, renf_elem_class& a)
{
    renf_class * nf = (renf_class *) is.pword(renf_class::xalloc());

    if (nf != nullptr)
        // reset the number field with the one from the stream
        a.reset_parent(nf);

    a.assign_stream(is);
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

inline renf_class::renf_class(renf_t k, const std::string& gen_name)
{
    renf_init_set(nf, k);
    this->gen_name = gen_name;
}

inline renf_class::renf_class(const char * pol, const char * var, const char * emb, const slong prec)
{
    arb_t e;
    fmpq_poly_t p;

    fmpq_poly_init(p);
    if (fmpq_poly_set_str_pretty(p, pol, var))
    {
        fmpq_poly_clear(p);
        std::invalid_argument("renf_class: can not read polynomial from string");
    }
    gen_name = var;

    arb_init(e);
    if (arb_set_str(e, emb, prec))
    {
        arb_clear(e);
        std::invalid_argument("renf_class: can not read ball from string");
    }

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

inline slong renf_class::degree()
{
    return nf_degree(nf->nf);
}


/**********************************/
/* renf_elem_class implementation */
/**********************************/

inline void renf_elem_class::assign_fmpz(const fmpz_t z)
{
    if (nf == nullptr)
    {
        fmpz_one(fmpq_denref(b));
        fmpz_set(fmpq_numref(b), z);
    }
    else
        renf_elem_set_fmpz(a, z, nf->get_renf());
}

inline void renf_elem_class::assign_mpz(const mpz_t z)
{
    if (nf == nullptr)
    {
        fmpz_one(fmpq_denref(b));
        fmpz_set_mpz(fmpq_numref(b), z);
    }
    else
        renf_elem_set_mpz(a, z, nf->get_renf());
}

inline void renf_elem_class::assign_mpz_class(const mpz_class& z)
{
    this->assign_mpz(z.__get_mp());
}

inline void renf_elem_class::assign_mpq(const mpq_t z)
{
    if (nf == nullptr) 
        fmpq_set_mpq(b, z);
    else
        renf_elem_set_mpq(a, z, nf->get_renf());
}

inline void renf_elem_class::assign_mpq_class(const mpq_class& z)
{
    this->assign_mpq(z.__get_mp());
}

inline void renf_elem_class::assign_fmpq(const fmpq_t z)
{
    if (nf == nullptr)
        fmpq_set(b, z);
    else
        renf_elem_set_fmpq(a, z, nf->get_renf());
}

// TODO: the conversion "signed long int" -> slong is not very careful
inline void renf_elem_class::assign_si(signed long int z)
{
    if (nf == nullptr)
    {
        fmpz_one(fmpq_denref(b));
        fmpz_set_si(fmpq_numref(b), (slong) z);
    }
    else
        renf_elem_set_si(a, (slong) z, nf->get_renf());
}

// TODO: the conversion "unsigned long int" -> ulong is not very careful
inline void renf_elem_class::assign_ui(unsigned long int z)
{
    if (nf == nullptr)
    {
        fmpz_one(fmpq_denref(b));
        fmpz_set_ui(fmpq_numref(b), (ulong) z);
    }
    else
        renf_elem_set_ui(a, (ulong) z, nf->get_renf());
}

inline void renf_elem_class::assign_fmpq_poly(const fmpq_poly_t p)
{
    if (nf == nullptr)
        throw std::invalid_argument("renf_elem_class: can not assign from polynomial if number field not set");
    renf_elem_set_fmpq_poly(a, p, nf->get_renf());
}

inline void renf_elem_class::assign_mpz_vector(const std::vector<mpz_class>& v)
{
    fmpq_poly_t p;

    if (nf == nullptr)
        throw std::invalid_argument("renf_elem_class: can not assign from std::vector<mpz_class> if number field not set");
    if ((long) v.size() > nf->degree())
        throw std::invalid_argument("vector too long");

    fmpq_poly_init(p);
    for (slong i = 0; i < (long) v.size(); i++)
        fmpq_poly_set_coeff_mpz(p, i, v[i].__get_mp());
    assign_fmpq_poly(p);
    fmpq_poly_clear(p);
}

inline void renf_elem_class::assign_mpq_vector(const std::vector<mpq_class>& v)
{
    fmpq_poly_t p;

    if (nf == nullptr)
        throw std::invalid_argument("renf_elem_class: can not assign from std::vector<mpz_class> if number field not set");
    if ((long) v.size() > nf->degree())
        throw std::invalid_argument("vector too long");

    fmpq_poly_init(p);
    for (slong i = 0; i < (long) v.size(); i++)
        fmpq_poly_set_coeff_mpq(p, i, v[i].__get_mp());
    assign_fmpq_poly(p);
    fmpq_poly_clear(p);

}

inline void renf_elem_class::assign_stream(std::istream& is)
{
    std::string s;          /* part of the stream to use */
    char c;                 /* current character in the stream */

    if (is.eof())
        throw std::invalid_argument("empty stream");

    std::string g;
    if (nf == nullptr)
        g = "";
    else
        g = parent().gen_name;

    c = is.peek();
    if (c == '(')
    {
        // read until ")"
        is.get();
        while (!is.eof() && is.peek() != ')' && is.peek() != EOF)
            s += is.get();
        if (is.eof())
            throw std::invalid_argument("invalid stream");
        is.get();  // remove ) from the stream
    }
    else
    {
        // read until space or EOF
        while (!is.eof() && !isspace(is.peek()) && is.peek() != EOF)
            s += is.get();
    }

    assign_string(s);
}

inline void renf_elem_class::assign_char_ptr(const char * s)
{
    int err;
    const char * i = strchr(s, '~');
    char * t;

    if (i != nullptr)
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

    if (nf == nullptr)
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
    if (p == nullptr)
    {
        if (nf != nullptr)
        {
            renf_elem_clear(a, nf->get_renf());
            nf = nullptr;
            fmpq_init(b);
        }
    }
    else
    {
        if (nf == nullptr)
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

inline void renf_elem_class::assign_renf_elem_class(const renf_elem_class& x)
{
    reset_parent(x.nf);
    if (x.nf == nullptr)
        fmpq_set(b, x.b);
    else
        renf_elem_set(a, x.a, nf->get_renf());
}

inline void renf_elem_class::assign_string(const std::string& s)
{
    assign_char_ptr(s.c_str());
}

/* constructors destructor */

inline renf_elem_class::renf_elem_class()
{
    nf = nullptr;
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
    if (nf == nullptr)
        fmpq_init(b);
    else
        renf_elem_init(a, nf->get_renf());
    assign_renf_elem_class(x);
}

inline renf_elem_class::~renf_elem_class(void)
{
    if (nf == nullptr) fmpq_clear(b);
    else renf_elem_clear(a, nf->get_renf());
}

/* testing */

inline bool renf_elem_class::is_fmpq(void) const
{
    return (nf == nullptr);
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

    if (nf == nullptr)
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

    if (nf == nullptr)
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

inline mpq_class renf_elem_class::get_rational(void) const
{
    mpq_class z;

    if (nf == nullptr)
        fmpq_get_mpq(z.__get_mp(), b);
    else if(is_rational())
    {
        fmpq_t q;
        fmpq_init(q);
        nf_elem_get_fmpq(q, a->elem, parent().get_renf()->nf);
        fmpq_get_mpq(z.__get_mp(), q);
        fmpq_clear(q);
    }
    else
    {
    }

    return z;
}

inline std::string renf_elem_class::get_str(int flag) const
{
    std::string s;

    if ((flag & EANTIC_STR_D) && (flag & EANTIC_STR_ARB))
    {
        throw std::invalid_argument("invalid flag");
    }

    // call to renf_elem_get_str_pretty
    if (nf == nullptr)
    {
        if (flag & EANTIC_STR_ALG)
        {
            char * u = fmpq_get_str(nullptr, 10, b);
            s += u;
            flint_free(u);

            if (flag & (EANTIC_STR_D | EANTIC_STR_ARB))
                s += " ~ ";
        }
        if (flag & EANTIC_STR_D)
        {
            char * u = (char *) flint_malloc(20 * sizeof(char));
            sprintf(u, "%lf", fmpq_get_d(b));
            s += u;
            flint_free(u);
        }
        if (flag & EANTIC_STR_ARB)
        {
            char *u;
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
        char * u = renf_elem_get_str_pretty(get_renf_elem(),
                parent().gen_name.c_str(),
                parent().get_renf(),
                10,
                flag);
        s += u;
        flint_free(u);
    }

    if (flag != EANTIC_STR_ALG &&
        flag != EANTIC_STR_D &&
        flag != EANTIC_STR_ARB)
        return "(" + s + ")";
    else
        return s;
}



// Unary arithmetic

inline renf_elem_class renf_elem_class::operator - () const
{
    renf_elem_class ans(*this);
    if (nf == nullptr) fmpq_neg(ans.b, ans.b);
    else renf_elem_neg(ans.a, ans.a, ans.nf->get_renf());
    return ans;
}

inline renf_elem_class renf_elem_class::operator + () const
{
    return *this;
}

// Binary arithmetic

static inline void renf_elem_fmpq_add(renf_elem_t a, fmpq_t b, renf_elem_t c, renf_t d) {renf_elem_add_fmpq(a, c, b, d);}
static inline void renf_elem_fmpq_mul(renf_elem_t a, fmpq_t b, renf_elem_t c, renf_t d) {renf_elem_mul_fmpq(a, c, b, d);}

#define __RENFXX_binop(OP, INOP, FUN1, FUN2, FUN3, FUN4) \
inline renf_elem_class& renf_elem_class::operator INOP (const renf_elem_class & other) \
{                                                 \
    if (nf != nullptr)                               \
    {                                             \
        if (nf == other.nf)                       \
            FUN1(a, a, other.a, nf->get_renf());  \
        else if (other.nf == nullptr)                \
            FUN2(a, a, other.b, nf->get_renf());  \
        else                                      \
            throw std::domain_error("arithmetic invalid on renf_elem_classes with different fields"); \
    }                                             \
    else if (other.nf != nullptr)                    \
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

__RENFXX_binop(+, +=, renf_elem_add, renf_elem_add_fmpq, renf_elem_fmpq_add, fmpq_add)
__RENFXX_binop(*, *=, renf_elem_mul, renf_elem_mul_fmpq, renf_elem_fmpq_mul, fmpq_mul)
__RENFXX_binop(-, -=, renf_elem_sub, renf_elem_sub_fmpq, renf_elem_fmpq_sub, fmpq_sub)
__RENFXX_binop(/, /=, renf_elem_div, renf_elem_div_fmpq, renf_elem_fmpq_div, fmpq_div)
#undef __RENFXX_binop

// TODO: we are not careful enough about conversions to slong/ulong
#define __RENFXX_binop(TYP, FMPQ_FUN, RENF_FUN, INOP, OP) \
inline renf_elem_class& renf_elem_class::operator INOP (TYP other) \
{                                              \
    if (nf == nullptr)                            \
        FMPQ_FUN(b, b, other);                 \
    else                                       \
        RENF_FUN(a, a, other, nf->get_renf()); \
    return *this;                              \
}                                              \
inline renf_elem_class renf_elem_class::operator OP (TYP other) const \
{ \
    renf_elem_class ans(*this);  \
    ans INOP other;              \
    return ans;                  \
} \
inline renf_elem_class operator OP (TYP a, const renf_elem_class& b) \
{                                         \
    renf_elem_class x(a);                 \
    x INOP b;                             \
    return x;                             \
}
#define __RENFXX_all_binop_si(TYP) \
__RENFXX_binop(TYP, fmpq_add_si, renf_elem_add_si, +=, +) \
__RENFXX_binop(TYP, fmpq_sub_si, renf_elem_sub_si, -=, -) \
__RENFXX_binop(TYP, fmpq_mul_si, renf_elem_mul_si, *=, *) \
__RENFXX_binop(TYP, fmpq_div_si, renf_elem_div_si, /=, /)

__RENFXX_all_binop_si(signed char)
__RENFXX_all_binop_si(signed short int)
__RENFXX_all_binop_si(signed int)
__RENFXX_all_binop_si(signed long int)

#undef __RENFXX_all_binop_si

#define __RENFXX_all_binop_ui(TYP) \
__RENFXX_binop(TYP, fmpq_add_ui, renf_elem_add_ui, +=, +) \
__RENFXX_binop(TYP, fmpq_sub_ui, renf_elem_sub_ui, -=, -) \
__RENFXX_binop(TYP, fmpq_mul_ui, renf_elem_mul_ui, *=, *) \
__RENFXX_binop(TYP, fmpq_div_ui, renf_elem_div_ui, /=, /)

__RENFXX_all_binop_ui(unsigned char)
__RENFXX_all_binop_ui(unsigned short int)
__RENFXX_all_binop_ui(unsigned int)
__RENFXX_all_binop_ui(unsigned long int)

#undef __RENFXX_all_binop_ui

#undef __RENFXX_binop
#undef __RENFXX_all_binop

#define __RENFXX_binop(TYP, FLINT_TYP, INITSET, CLEAR, FMPQ_FUN, RENF_FUN, INOP, OP) \
inline renf_elem_class& renf_elem_class::operator INOP (const TYP& other) \
{                                              \
    FLINT_TYP tmp;                             \
    INITSET(tmp, other.__get_mp());            \
    if (nf == nullptr)                            \
        FMPQ_FUN(b, b, tmp);                   \
    else                                       \
        RENF_FUN(a, a, tmp, nf->get_renf());   \
    CLEAR(tmp);                                \
    return *this;                              \
}                                              \
inline renf_elem_class renf_elem_class::operator OP (const TYP& other) const \
{                                \
    renf_elem_class ans(*this);  \
    ans INOP other;              \
    return ans;                  \
}                                \
inline renf_elem_class operator OP (const TYP& a, const renf_elem_class& b) \
{                                         \
    renf_elem_class x(a);                 \
    x INOP b;                             \
    return x;                             \
}

__RENFXX_binop(mpz_class, fmpz_t, fmpz_init_set_readonly, fmpz_clear_readonly, fmpq_add_fmpz, renf_elem_add_fmpz, +=, +)
__RENFXX_binop(mpz_class, fmpz_t, fmpz_init_set_readonly, fmpz_clear_readonly, fmpq_sub_fmpz, renf_elem_sub_fmpz, -=, -)
__RENFXX_binop(mpz_class, fmpz_t, fmpz_init_set_readonly, fmpz_clear_readonly, fmpq_mul_fmpz, renf_elem_mul_fmpz, *=, *)
__RENFXX_binop(mpz_class, fmpz_t, fmpz_init_set_readonly, fmpz_clear_readonly, fmpq_div_fmpz, renf_elem_div_fmpz, /=, /)

__RENFXX_binop(mpq_class, fmpq_t, fmpq_init_set_readonly, fmpq_clear_readonly, fmpq_add, renf_elem_add_fmpq, +=, +)
__RENFXX_binop(mpq_class, fmpq_t, fmpq_init_set_readonly, fmpq_clear_readonly, fmpq_sub, renf_elem_sub_fmpq, -=, -)
__RENFXX_binop(mpq_class, fmpq_t, fmpq_init_set_readonly, fmpq_clear_readonly, fmpq_mul, renf_elem_mul_fmpq, *=, *)
__RENFXX_binop(mpq_class, fmpq_t, fmpq_init_set_readonly, fmpq_clear_readonly, fmpq_div, renf_elem_div_fmpq, /=, /)

#undef __RENFXX_binop
#undef __RENFXX_all_binop

// Comparisons

inline bool renf_elem_class::operator == (const renf_elem_class& other) const
{
    if (nf != nullptr)
    {
        if (nf == other.nf)
        {
            return renf_elem_equal(a, other.a, nf->get_renf());
        }
        else if (other.nf == nullptr)
            return renf_elem_equal_fmpq(a, other.b, nf->get_renf());
        else
            throw std::domain_error("can not compare renf_elem_class on different number fields");
    }
    else if (other.nf == nullptr)
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
    if (nf != nullptr)
    {
        if (nf == other.nf)
            return renf_elem_cmp(a, other.a, nf->get_renf()) > 0;
        else if (other.nf == nullptr)
            return renf_elem_cmp_fmpq(a, other.b, nf->get_renf()) > 0;
        else
            throw std::domain_error("can not compare renf_elem_class on different number fields");
    }
    else if (other.nf == nullptr)
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

#define __RENFXX_cmp(TYP, FMPQ_EQ, FMPQ_CMP, RENF_EQ, RENF_CMP) \
inline bool renf_elem_class::operator == (TYP other) const \
{                                                    \
    if (nf == nullptr)                                  \
        return FMPQ_EQ(b, other);                    \
    else                                             \
        return RENF_EQ(a, other, nf->get_renf());    \
}                                                    \
inline bool operator == (TYP other, const renf_elem_class& r) \
{                                                    \
    if (r.nf == nullptr)                               \
        return FMPQ_EQ(r.b, other);                 \
    else                                             \
        return RENF_EQ(r.a, other, r.nf->get_renf()); \
}                                                    \
inline bool renf_elem_class::operator != (TYP other) const \
{                                                     \
    if (nf == nullptr)                                   \
        return not FMPQ_EQ(b, other);                 \
    else                                              \
        return not RENF_EQ(a, other, nf->get_renf()); \
}                                \
inline bool operator != (TYP other, const renf_elem_class& r) \
{                                                             \
    if (r.nf == nullptr)                                        \
        return not FMPQ_EQ(r.b, other);                      \
    else                                                      \
        return not RENF_EQ(r.a, other, r.nf->get_renf());    \
}                                                             \
inline bool renf_elem_class:: operator < (TYP other) const    \
{                                                             \
    if (nf == nullptr)                                           \
        return FMPQ_CMP(b, other) == -1;                      \
    else                                                      \
        return RENF_CMP(a, other, nf->get_renf()) == -1;      \
}                                                             \
inline bool operator < (TYP other, const renf_elem_class& r)        \
{                                                             \
    if (r.nf == nullptr)                                        \
        return FMPQ_CMP(r.b, other) == 1;                    \
    else                                                      \
        return RENF_CMP(r.a, other, r.nf->get_renf()) == 1; \
}                                                             \
inline bool renf_elem_class:: operator > (TYP other) const    \
{                                                             \
    if (nf == nullptr)                                           \
        return FMPQ_CMP(b, other) == 1;                       \
    else                                                      \
        return RENF_CMP(a, other, nf->get_renf()) == 1;       \
}                                                             \
inline bool operator > (TYP other, const renf_elem_class& r)        \
{                                                             \
    if (r.nf == nullptr)                                        \
        return FMPQ_CMP(r.b, other) == -1;                   \
    else                                                      \
        return RENF_CMP(r.a, other, r.nf->get_renf()) == -1; \
}                                                              \
inline bool renf_elem_class:: operator <= (TYP other) const    \
{                                                              \
    if (nf == nullptr)                                            \
        return FMPQ_CMP(b, other) <= 0;                        \
    else                                                       \
        return RENF_CMP(a, other, nf->get_renf()) <= 0;        \
}                                                              \
inline bool operator <= (TYP other, const renf_elem_class& r)        \
{                                                              \
    if (r.nf == nullptr)                                         \
        return FMPQ_CMP(r.b, other) >= 0;                     \
    else                                                       \
        return RENF_CMP(r.a, other, r.parent().get_renf()) >= 0;  \
}                                                              \
inline bool renf_elem_class:: operator >= (TYP other) const    \
{                                                              \
    if (nf == nullptr)                                            \
        return FMPQ_CMP(b, other) >= 0;                        \
    else                                                       \
        return RENF_CMP(a, other, nf->get_renf()) >= 0;        \
}                                                              \
inline bool operator >= (TYP other, const renf_elem_class& r)        \
{                                                              \
    if (r.nf == nullptr)                                         \
        return FMPQ_CMP(r.b, other) <= 0;                     \
    else                                                       \
        return RENF_CMP(r.a, other, r.parent().get_renf()) <= 0;  \
}

__RENFXX_cmp(signed char, fmpq_equal_si, fmpq_cmp_si, renf_elem_equal_si, renf_elem_cmp_si)
__RENFXX_cmp(signed short int, fmpq_equal_si, fmpq_cmp_si, renf_elem_equal_si, renf_elem_cmp_si)
__RENFXX_cmp(signed int, fmpq_equal_si, fmpq_cmp_si, renf_elem_equal_si, renf_elem_cmp_si)
__RENFXX_cmp(signed long int, fmpq_equal_si, fmpq_cmp_si, renf_elem_equal_si, renf_elem_cmp_si)

__RENFXX_cmp(unsigned char, fmpq_equal_ui, fmpq_cmp_ui, renf_elem_equal_ui, renf_elem_cmp_ui)
__RENFXX_cmp(unsigned short int, fmpq_equal_ui, fmpq_cmp_ui, renf_elem_equal_ui, renf_elem_cmp_ui)
__RENFXX_cmp(unsigned int, fmpq_equal_ui, fmpq_cmp_ui, renf_elem_equal_ui, renf_elem_cmp_ui)
__RENFXX_cmp(unsigned long int, fmpq_equal_ui, fmpq_cmp_ui, renf_elem_equal_ui, renf_elem_cmp_ui)

#undef __RENFXX_cmp


#define __RENFXX_eq_ne(TYP, FLINT_TYP, INITSET, CLEAR, FMPQ_EQ, RENF_EQ) \
inline bool renf_elem_class:: operator == (const TYP& other) const \
{                                    \
    bool res;                        \
    FLINT_TYP tmp;                   \
    INITSET(tmp, other.__get_mp());      \
    if (nf == nullptr)                  \
        res = FMPQ_EQ(b, tmp);       \
    else                             \
        res = RENF_EQ(a, tmp, nf->get_renf()); \
    CLEAR(tmp);                      \
    return res;                      \
}                                    \
inline bool operator == (const TYP& other, const renf_elem_class& r) \
{                                    \
    bool res;                        \
    FLINT_TYP tmp;                   \
    INITSET(tmp, other.__get_mp());      \
    if (r.nf == nullptr)                  \
        res = FMPQ_EQ(r.b, tmp);       \
    else                             \
        res = RENF_EQ(r.a, tmp, r.nf->get_renf()); \
    CLEAR(tmp);                      \
    return res;                      \
}                                    \
inline bool renf_elem_class:: operator != (const TYP& other) const \
{                                    \
    bool res;                        \
    FLINT_TYP tmp;                   \
    INITSET(tmp, other.__get_mp());      \
    if (nf == nullptr)                  \
        res = not FMPQ_EQ(b, tmp);       \
    else                             \
        res = not RENF_EQ(a, tmp, nf->get_renf()); \
    CLEAR(tmp);                      \
    return res;                      \
}                                    \
inline bool operator != (const TYP& other, const renf_elem_class& r) \
{                                    \
    bool res;                        \
    FLINT_TYP tmp;                   \
    INITSET(tmp, other.__get_mp());      \
    if (r.nf == nullptr)                  \
        res = not FMPQ_EQ(r.b, tmp);       \
    else                             \
        res = not RENF_EQ(r.a, tmp, r.nf->get_renf()); \
    CLEAR(tmp);                      \
    return res;                      \
}

__RENFXX_eq_ne(mpz_class, fmpz_t, fmpz_init_set_readonly, fmpz_clear_readonly, fmpq_equal_fmpz, renf_elem_equal_fmpz)
__RENFXX_eq_ne(mpq_class, fmpq_t, fmpq_init_set_readonly, fmpq_clear_readonly, fmpq_equal, renf_elem_equal_fmpq)

#undef __RENFXX_eq_ne

#define __RENFXX_cmp(TYP, FLINT_TYP, INITSET, CLEAR, FMPQ_CMP, RENF_CMP, OP, COND, COND_INV) \
inline bool renf_elem_class:: operator OP (const TYP& other) const \
{                                    \
    bool res;                        \
    FLINT_TYP tmp;                   \
    INITSET(tmp, other.__get_mp());      \
    if (nf == nullptr)                  \
        res = FMPQ_CMP(b, tmp) COND;       \
    else                             \
        res = RENF_CMP(a, tmp, nf->get_renf()) COND; \
    CLEAR(tmp);                      \
    return res;                      \
}                                    \
inline bool operator OP (const TYP& other, const renf_elem_class& r) \
{                                    \
    bool res;                        \
    FLINT_TYP tmp;                   \
    INITSET(tmp, other.__get_mp());      \
    if (r.nf == nullptr)                  \
        res = FMPQ_CMP(r.b, tmp) COND_INV;       \
    else                             \
        res = RENF_CMP(r.a, tmp, r.nf->get_renf()) COND_INV; \
    CLEAR(tmp);                      \
    return res;                      \
}                                    \

#define __RENFXX_all_cmp(TYP, FLINT_TYP, INITSET, CLEAR, FMPQ_CMP, RENF_CMP) \
__RENFXX_cmp(TYP, FLINT_TYP, INITSET, CLEAR, FMPQ_CMP, RENF_CMP, >=, >= 0, <= 0) \
__RENFXX_cmp(TYP, FLINT_TYP, INITSET, CLEAR, FMPQ_CMP, RENF_CMP, <=, <= 0, >= 0) \
__RENFXX_cmp(TYP, FLINT_TYP, INITSET, CLEAR, FMPQ_CMP, RENF_CMP, >, > 0, < 0) \
__RENFXX_cmp(TYP, FLINT_TYP, INITSET, CLEAR, FMPQ_CMP, RENF_CMP, <, < 0, > 0)

__RENFXX_all_cmp(mpz_class, fmpz_t, fmpz_init_set_readonly, fmpz_clear_readonly, fmpq_cmp_fmpz, renf_elem_cmp_fmpz)
__RENFXX_all_cmp(mpq_class, fmpq_t, fmpq_init_set_readonly, fmpq_clear_readonly, fmpq_cmp, renf_elem_cmp_fmpq)

#undef __RENFXX_cmp
#undef __RENFXX_all_cmp

// Other functions

inline bool renf_elem_class::is_zero() const
{
    if (nf == nullptr)
        return fmpq_is_zero(b);
    else
        return renf_elem_is_zero(a, nf->get_renf());
}

inline bool renf_elem_class::is_one() const
{
    if (nf == nullptr)
        return fmpq_is_one(b);
    else
        return renf_elem_is_one(a, nf->get_renf());
}

inline bool renf_elem_class::is_rational() const
{
    if (nf == nullptr)
        return true;
    else
        return renf_elem_is_rational(a, nf->get_renf());
}

inline bool renf_elem_class::is_integer() const
{
    if (nf == nullptr)
        return fmpz_is_one(fmpq_denref(b));
    else
        return renf_elem_is_integer(a, nf->get_renf());
}

inline mpz_class renf_elem_class::floor() const
{
    fmpz_t tmp;
    fmpz_init(tmp);

    if (nf == nullptr) fmpz_fdiv_q(tmp, fmpq_numref(b), fmpq_denref(b));
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

    if (nf == nullptr)
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
    if (nf == nullptr)
        return fmpq_sgn(b);
    else
        return renf_elem_sgn(a, nf->get_renf());
}

inline double renf_elem_class::get_d() const
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
        return renf_elem_get_d(a, nf->get_renf(), ARF_RND_NEAR);
}

#endif
