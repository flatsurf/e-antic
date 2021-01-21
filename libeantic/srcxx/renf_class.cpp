/*
    Copyright (C) 2018 Vincent Delecroix
    Copyright (C) 2019-2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <stdexcept>

#include "../e-antic/renfxx.h"
#include "../e-antic/fmpq_poly_extra.h"
#include "external/unique-factory/unique_factory.hpp"

namespace eantic {

namespace {
// An index returned from std::ios_base::xalloc() that we use to store a
// pointer back to this number field.  We store a pointer to this number
// field at that index in a stream when we want to parse renf_elem_class
// elements from it.
static int xalloc = std::ios_base::xalloc();

// A key identifying a renf_class for deduplication in the unique_factory.
struct renf_class_key
{
    renf_class_key(std::shared_ptr<renf_class>&& key) : key(std::move(key)) {}
    bool operator==(const renf_class_key & rhs) const {
      return renf_equal(key->renf_t(), rhs.key->renf_t()) && key->gen_name() == rhs.key->gen_name();
    }

    std::shared_ptr<renf_class> key;
};

} // end of anonymous namespace
} // end of namespace eantic

namespace std {

template <> struct hash<eantic::renf_class_key> {
    size_t operator()(const eantic::renf_class_key &) const;
};

} // end of namespace std

namespace eantic {
namespace {

// Deduplicating factory so that all renf_class are guaranteed to be unique
// parents.
static unique_factory::UniqueFactory<renf_class_key, renf_class> factory;

} // end of anonymous namespace

renf_class::renf_class()
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

    name = "a";
}

renf_class::renf_class(const ::renf_t k, const std::string & gen_name)
{
    renf_init_set(nf, k);
    this->name = gen_name;
}

renf_class::renf_class(const std::string & minpoly, const std::string & gen, const std::string & emb, const slong prec)
{
    arb_t e;
    fmpq_poly_t p;

    fmpq_poly_init(p);
    if (fmpq_poly_set_str_pretty(p, minpoly.c_str(), gen.c_str()))
    {
        fmpq_poly_clear(p);
        throw std::invalid_argument("renf_class: can not read polynomial from string");
    }
    name = gen;

    arb_init(e);
    if (arb_set_str(e, emb.c_str(), prec))
    {
        fmpq_poly_clear(p);
        arb_clear(e);
        throw std::invalid_argument("renf_class: can not read ball from string");
    }

    if (!fmpq_poly_check_unique_real_root(p, e, prec))
    {
        fmpq_poly_clear(p);
        arb_clear(e);
        throw std::invalid_argument("the given polynomial does not have a unique such root");
    }

    renf_init(nf, p, e, prec);

    fmpq_poly_clear(p);
    arb_clear(e);
}

std::shared_ptr<const renf_class> renf_class::make()
{
    static auto trivial = factory.get(std::shared_ptr<renf_class>(new renf_class()), [&]() { return new renf_class; });
    return trivial;
}

std::shared_ptr<const renf_class> renf_class::make(const ::renf_t k, const std::string & gen_name)
{
    return factory.get(std::shared_ptr<renf_class>(new renf_class(k, gen_name)), [&]() { return new renf_class(k, gen_name); });
}

std::shared_ptr<const renf_class> renf_class::make(const std::string & minpoly, const std::string & gen, const std::string & emb, const slong prec)
{
    return factory.get(
        std::shared_ptr<renf_class>(new renf_class(minpoly, gen, emb, prec)), [&]() { return new renf_class(minpoly, gen, emb, prec); });
}

renf_class::~renf_class() noexcept
{
    renf_clear(nf);
}

slong renf_class::degree() const { return fmpq_poly_degree(nf->nf->pol); }

renf_elem_class renf_class::zero() const
{
    renf_elem_class a(this->shared_from_this(), 0);
    return a;
}

renf_elem_class renf_class::one() const
{
    renf_elem_class a(this->shared_from_this(), 1);
    return a;
}

renf_elem_class renf_class::gen() const
{
    renf_elem_class a(this->shared_from_this());
    renf_elem_gen(a.renf_elem_t(), this->renf_t());
    return a;
}

bool operator==(const renf_class & self, const renf_class & other)
{
    return &self == &other;
}

std::istream & renf_class::set_pword(std::istream & is) const
{
    is.pword(xalloc) = const_cast<void*>(reinterpret_cast<const void*>(this));
    return is;
}

std::istream & renf_class::set_istream(std::istream & is) const { return set_pword(is); }

std::shared_ptr<const renf_class> renf_class::get_pword(std::istream& is)
{
    return reinterpret_cast<renf_class *>(is.pword(xalloc))->shared_from_this();
}

std::string renf_class::to_string() const
{
    char * u = renf_get_str(renf_t(), gen_name().c_str(), 64);
    std::string s = u;
    flint_free(u);
    return s;
}

std::ostream & operator<<(std::ostream & os, const renf_class & nf)
{
    os << nf.to_string();
    return os;
}

std::ostream & operator<<(std::ostream & os, const renf_elem_class & a)
{
    if (a.is_integer())
        os << a.to_string(EANTIC_STR_ALG);
    else
        os << a.to_string(EANTIC_STR_ALG | EANTIC_STR_D);
    return os;
}

std::istream & operator>>(std::istream & is, renf_elem_class & a)
{
    auto nf = renf_class::get_pword(is);

    std::string s; // part of the stream to use

    if (is.eof()) throw std::invalid_argument("empty stream");

    bool paren = is.peek() == std::char_traits<char>::to_int_type('(');
    if (paren)
    {
        // read until ")"
        is.get();
        while (!is.eof() && is.peek() != std::char_traits<char>::to_int_type(')') && is.peek() != EOF) {
            char c;
            is >> c;
            s += c;
        }
        if (is.eof()) throw std::invalid_argument("invalid stream");
        is.get(); // remove ) from the stream
    }
    else
    {
        if (paren) is.get();
        // read until space or EOF
        while (!is.eof() && !isspace(is.peek()) && is.peek() != EOF) {
            char c;
            is >> c;
            s += c;
        }
    }

    a = renf_elem_class(nf, s);

    return is;
}

} // end of namespace eantic

namespace std {
size_t hash<eantic::renf_class>::operator()(const eantic::renf_class& nf) const
{
    return hash<eantic::renf_elem_class>()(nf.gen());
}

size_t hash<eantic::renf_class_key>::operator()(const eantic::renf_class_key& nf) const
{
    return hash<eantic::renf_class>()(*nf.key);
}
}
