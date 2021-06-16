/*
    Copyright (C) 2018 Vincent Delecroix
    Copyright (C) 2019-2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <atomic>
#include <mutex>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>

#include "../e-antic/renf_class.hpp"
#include "../e-antic/renf_elem_class.hpp"
#include "../e-antic/fmpq_poly_extra.h"

namespace eantic {

namespace {
// An index returned from std::ios_base::xalloc() that we use to store a
// pointer back to this number field.  We store a pointer to this number
// field at that index in a stream when we want to parse renf_elem_class
// elements from it.
static int xalloc = std::ios_base::xalloc();

// Computes a hash value for a field. This is only used to deduplicate fields
// and is not the std::hash implementation we use normally: once we know that
// fields are unique, we can hash much faster by using their address in the
// std::hash implementation at the bottom of this file.
struct fields_hasher {
    size_t operator()(const renf_class* nf) const { return std::hash<eantic::renf_elem_class>()(nf->gen()); }
};

// Compare two fields. This is only used to deduplicate fields and is not the
// usual operator== implementation we use normally: once we know that fields
// are unique, we can just compare their addresses in memory which is much
// faster.
struct fields_equality {
    bool operator()(const renf_class* lhs, const renf_class* rhs) const {
        return renf_equal(lhs->renf_t(), rhs->renf_t()) && lhs->gen_name() == rhs->gen_name();
    }
};

// A global registry of all fields that are currently around. We use this to
// make sure that fields are unique parents.
static std::unordered_set<const renf_class*, fields_hasher, fields_equality> fields;

// A mutex to lock any access to the global field registry.
static std::mutex fields_mutex;

}

static_assert(!std::is_default_constructible<renf_class>::value, "renf_class should have implicitly deleted its default constructor; since there can only be one default renf_class() we must not allow for a default constructor.");
static_assert(!std::is_copy_constructible<renf_class>::value, "renf_class should have implicitly deleted its copy constructor; since it is only accessible through intrusive pointers, there must be no copies at other memory addresses");
static_assert(!std::is_move_constructible<renf_class>::value, "renf_class should have implicitly deleted its copy constructor; since it is only accessible through intrusive pointers, the address must be stable for the entire lifetime");
static_assert(!std::is_copy_assignable<renf_class>::value, "renf_class should have implicitly deleted its assignment operator; since it is only accessible through intrusive pointers, there must be no copies at other memory addresses");
static_assert(!std::is_move_assignable<renf_class>::value, "renf_class should have implicitly deleted its assignment operator; since it is only accessible through intrusive pointers, the address must be stable for the entire lifetime");

renf_class::renf_class(const ::renf_t k, const std::string & gen_name) : name(gen_name), refcount(0)
{
    renf_init_set(nf, k);

    // We prepare a cache of 0,1, and the generator of this field.
    cache = new renf_elem_class[3] {
        renf_elem_class(*this),
        renf_elem_class(*this, 1),
        renf_elem_class(*this),
    };
    renf_elem_gen(cache[2].renf_elem_t(), renf_t());
}

const renf_class& renf_class::make()
{
    // We cache our standard rational field in a static variable.
    // Note that the destructor of this shared pointer gets called before the
    // destructor of the factory, i.e., the factory does not complain about a
    // pending object, because in C++ static objects are destructed in the
    // reverse order of their construction:
    // https://stackoverflow.com/questions/469597/destruction-order-of-static-objects-in-c
    static const boost::intrusive_ptr<const renf_class> trivial = make("a - 1", "a", "1 +/- 1", 64);

    return *trivial;
}

boost::intrusive_ptr<const renf_class> renf_class::make(const ::renf_t k, const std::string & gen_name)
{
    const auto* key = new renf_class(k, gen_name);

    std::lock_guard<std::mutex> lock(fields_mutex);

    const auto entry = fields.find(key);

    if (entry == fields.end())
    {
        fields.insert(key);
        return key;
    }
    else
    {
        delete key;
        return *entry;
    }
}

boost::intrusive_ptr<const renf_class> renf_class::make(const std::string & minpoly, const std::string & gen, const std::string & emb, const slong prec)
{
    arb_t e;
    fmpq_poly_t p;

    fmpq_poly_init(p);
    if (fmpq_poly_set_str_pretty(p, minpoly.c_str(), gen.c_str()))
    {
        fmpq_poly_clear(p);
        throw std::invalid_argument("renf_class: can not read polynomial from string");
    }

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

    ::renf_t nf;
    renf_init(nf, p, e, prec);

    fmpq_poly_clear(p);
    arb_clear(e);

    auto field = make(nf, gen);

    renf_clear(nf);

    return field;
}

renf_class::~renf_class() noexcept
{
    delete[] cache;
    assert(refcount == 0 && "All references to this number field must have been destroyed when this field is deleted. There seems to be an error in the reference counting.");
    renf_clear(nf);
}

slong renf_class::degree() const { return fmpq_poly_degree(nf->nf->pol); }

const renf_elem_class& renf_class::zero() const
{
    return cache[0];
}

const renf_elem_class& renf_class::one() const
{
    return cache[1];
}

const renf_elem_class& renf_class::gen() const
{
    return cache[2];
}

std::tuple<std::string, std::string, std::string, slong> renf_class::construction() const {
  char * emb = arb_get_str(renf_t()->emb, arf_bits(arb_midref(renf_t()->emb)), 0);
  char * pol = fmpq_poly_get_str_pretty(renf_t()->nf->pol, gen_name().c_str());

  const auto construction = std::make_tuple(std::string(pol), gen_name(), std::string(emb), renf_t()->prec);

  flint_free(pol);
  flint_free(emb);

  return construction;
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

boost::intrusive_ptr<const renf_class> renf_class::get_pword(std::istream& is)
{
    return reinterpret_cast<renf_class *>(is.pword(xalloc));
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

    a = renf_elem_class(*nf, s);

    return is;
}

// Track a pointer to the field `nf` (and keep the field alive until this
// pointer is around.)
void intrusive_ptr_add_ref(const renf_class* nf) {
    nf->refcount.fetch_add(1, std::memory_order_relaxed);
}

// Stop tracking a pointer to the field `nf`.
void intrusive_ptr_release(const renf_class* nf) {
    if (nf->refcount.fetch_sub(1, std::memory_order_release) == 4)
    {
        // All references to this field have been released, i.e., all
        // intrusive_ptr<const renf_class*> are gone except for the ones
        // internal to the renf_class itself.
        // We remove this field from the registry of fields and then free the
        // memory it is occupying.

        std::lock_guard<std::mutex> lock(fields_mutex);

        assert(fields.find(nf) != fields.end());
        fields.erase(nf);
        delete(nf);
    }
}

} // end of namespace eantic

namespace std {
size_t hash<eantic::renf_class>::operator()(const eantic::renf_class& nf) const
{
    return reinterpret_cast<size_t>(&nf);
}
}
