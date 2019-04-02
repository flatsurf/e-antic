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

namespace {
// An index returned from std::ios_base::xalloc() that we use to store a
// pointer back to this number field.  We store a pointer to this number
// field at that index in a stream when we want to parse renf_elem_class
// elements from it.
static int xalloc = std::ios_base::xalloc();
} // end of anonymous namespace 

namespace eantic {

inline renf_class& renf_class::operator = (const renf_class& k) noexcept
{
    renf_clear(nf);
    renf_init_set(nf, k.nf);
    return *this;
}

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

inline renf_class::renf_class(const renf_t& k, const std::string& gen_name)
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
    return nf == NULL ? 1 : nf_degree(nf->nf);
}

} // end of namespace eanticw
