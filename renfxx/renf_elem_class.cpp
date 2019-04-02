/*
    Copyright (C) 2018 Vincent Delecroix
    Copyright (C) 2019 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renfxx.h>

namespace eantic {

    /*
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
    explicit __RENFXX_construct(std::vector<mpz_class>&, assign_mpz_vector);
    explicit __RENFXX_construct(std::vector<mpq_class>&, assign_mpq_vector);
    explicit __RENFXX_construct(std::string&, assign_string)
    #undef __constructor
    */

    /*
    void reset_parent(std::shared_ptr<renf_class> p);

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
    */
std::vector<mpz_class> renf_elem_class::get_num_vector(void) const noexcept
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

const std::shared_ptr<renf_class>& renf_elem_class::parent() const noexcept
{
  return nf;
}

std::shared_ptr<renf_class>& renf_elem_class::parent() noexcept
{
  return nf;
}

inline renf_elem_class& renf_elem_class::operator = (const renf_elem_class& x)
{
    assign_renf_elem_class(x);
    return *this;
}

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
    if (v.size() > nf->degree())
        throw std::invalid_argument("vector too long");

    fmpq_poly_init(p);
    for (slong i = 0; i < v.size(); i++)
        fmpq_poly_set_coeff_mpz(p, i, v[i].__get_mp());
    assign_fmpq_poly(p);
    fmpq_poly_clear(p);
}

inline void renf_elem_class::assign_mpq_vector(const std::vector<mpq_class>& v)
{
    fmpq_poly_t p;

    if (nf == nullptr)
        throw std::invalid_argument("renf_elem_class: can not assign from std::vector<mpz_class> if number field not set");
    if (v.size() > nf->degree())
        throw std::invalid_argument("vector too long");

    fmpq_poly_init(p);
    for (slong i = 0; i < v.size(); i++)
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
}
