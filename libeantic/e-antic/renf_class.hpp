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

#include <memory>
#include <boost/operators.hpp>
#include <string>

#include "e-antic.h"
#include "renfxx_fwd.hpp"
#include "renf.h"

namespace eantic {

// A Real Embedded Number Field
// This class provides C++ memory management for the underlying renf_t.
class LIBEANTIC_API renf_class : public std::enable_shared_from_this<renf_class>, boost::equality_comparable<renf_class> {
    // The trivial number field adjoining a root of (x - 1) to the rationals
    renf_class();
    renf_class(const ::renf_t, const std::string &);
    renf_class(const std::string &, const std::string &, const std::string &, const slong);

public:
    // The trivial number field adjoining a root of (x - 1) to the rationals
    static std::shared_ptr<const renf_class> make();
    static std::shared_ptr<const renf_class> make(const ::renf_t, const std::string & gen = "a");
    static std::shared_ptr<const renf_class> make(const std::string & minpoly, const std::string & gen, const std::string &emb, const slong prec = 64);

    ~renf_class() noexcept;

    // Assignment is not possible since one renf_class uniquely corresponds to
    // an embedded number field.
    renf_class & operator=(const renf_class &) = delete;

    slong degree() const;

    // standard elements
    renf_elem_class zero() const;
    renf_elem_class one() const;
    renf_elem_class gen() const;

    // Return the parameters (minpoly, gen, emb, prec) that can be used to
    // construct this field with renf_class::make().
    std::tuple<std::string, std::string, std::string, slong> construction() const;

    friend bool operator==(const renf_class &, const renf_class &);

    const std::string & gen_name() const { return name; }

    // Prepare an input stream to read elements living in this number field
    // from it.
    [[deprecated("use renfxx_cereal.h or renf_elem_class's constructor taking a string instead.")]]
    std::istream & set_pword(std::istream &) const;

    // Extract the number field stored with set_pword from an input stream.
    [[deprecated("use renfxx_cereal.h or renf_elem_class's constructor taking a string instead.")]]
    static std::shared_ptr<const renf_class> get_pword(std::istream &);

    std::string to_string() const;
    friend std::ostream & operator<<(std::ostream &, const renf_class &);

    // Raw access to the underlying renf_t; we do not return a const renf_t
    // since calls in the C API might need to modify it (e.g., to refine the
    // stored embedding) even though they are morally const.
    ::renf_t & renf_t() const { return nf; }

    [[deprecated("Use renf_t() instead.")]] renf * get_renf() const { return nf; }
    [[deprecated("Use set_pword() instead.")]] std::istream & set_istream(std::istream &) const;

private:
    // The name of the generator
    std::string name;

    // The actual underlying renf_t
    mutable ::renf_t nf;

    // Serialization, see renfxx_cereal.h
    friend cereal::access;
};

} // end of namespace

namespace std {
template <>
struct LIBEANTIC_API hash<eantic::renf_class> {
  size_t operator()(const eantic::renf_class&) const;
};
}  // namespace std

#endif

