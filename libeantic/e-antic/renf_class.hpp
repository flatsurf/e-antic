/*  This is a -*- C++ -*- header file.

    Copyright (C) 2016-2018 Vincent Delecroix
                  2019-2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_RENF_CLASS_HPP
#define E_ANTIC_RENF_CLASS_HPP

#include <boost/operators.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <string>
#include <atomic>

#include "renfxx_fwd.hpp"
#include "renf.h"

namespace eantic {

// A Real Embedded Number Field
// This class provides C++ memory management for the underlying renf_t.
class LIBEANTIC_API renf_class : boost::equality_comparable<renf_class> {
    renf_class(const ::renf_t, const std::string &);

public:
    // The trivial number field adjoining a root of (x - 1) to the rationals
    static const renf_class& make();
    static boost::intrusive_ptr<const renf_class> make(const ::renf_t, const std::string & gen = "a");
    static boost::intrusive_ptr<const renf_class> make(const std::string & minpoly, const std::string & gen, const std::string &emb, const slong prec = 64);

    ~renf_class() noexcept;

    slong degree() const;

    // standard elements
    const renf_elem_class& zero() const;
    const renf_elem_class& one() const;
    const renf_elem_class& gen() const;

    // Return the parameters (minpoly, gen, emb, prec) that can be used to
    // construct this field with renf_class::make().
    std::tuple<std::string, std::string, std::string, slong> construction() const;

    friend bool operator==(const renf_class &, const renf_class &);

    const std::string & gen_name() const { return name; }

    // Prepare an input stream to read elements living in this number field
    // from it.
    [[deprecated("use cereal.hpp or the renf_elem_class constructor taking a string instead.")]]
    std::istream & set_pword(std::istream &) const;

    // Extract the number field stored with set_pword from an input stream.
    [[deprecated("use cereal.hpp or the renf_elem_class constructor taking a string instead.")]]
    static boost::intrusive_ptr<const renf_class> get_pword(std::istream &);

    std::string to_string() const;
    friend std::ostream & operator<<(std::ostream &, const renf_class &);

    // Raw access to the underlying renf_t; we do not return a const renf_t
    // since calls in the C API might need to modify it (e.g., to refine the
    // stored embedding) even though they are morally const.
    std::add_lvalue_reference_t<::renf_t> renf_t() const { return nf; }

    [[deprecated("Use renf_t() instead.")]] renf * get_renf() const { return nf; }
    [[deprecated("Use set_pword() instead.")]] std::istream & set_istream(std::istream &) const;

private:
    // The name of the generator
    std::string name;

    // The actual underlying renf_t
    mutable ::renf_t nf;

    mutable std::atomic<size_t> refcount;

    renf_elem_class* cache;

    // Serialization, see cereal.hpp
    friend cereal::access;

    LIBEANTIC_API friend void intrusive_ptr_add_ref(const renf_class*);
    LIBEANTIC_API friend void intrusive_ptr_release(const renf_class*);
};


} // end of namespace

namespace std {
template <>
struct LIBEANTIC_API hash<eantic::renf_class> {
  size_t operator()(const eantic::renf_class&) const;
};
}  // namespace std

#endif
