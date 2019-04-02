/*  This is a -*- C++ -*- header file.

    Copyright (C) 2016-2018 Vincent Delecroix
    Copyright (C) 2019 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef RENFXX_H
#define RENFXX_H

#include <memory>
#include <vector>

#include <boost/operators.hpp>
#include <gmpxx.h>

#include <e-antic/renf.h>
#include <e-antic/renf_elem.h>
#include <e-antic/renfxx_fwd.h>

namespace eantic {

// A Real Embedded Number Field
// This class provides C++ memory management for the underlying renf_t.
struct renf_class : boost::equality_comparable<renf_class>
{
    // The trivial number field adjoining a root of (x - 1) to the rationals
    renf_class() noexcept;
    renf_class(const renf_t&, const std::string& gen = "a") noexcept;
    renf_class(renf_t&&, const std::string& gen = "a") noexcept;
    renf_class(const std::string& minpoly, const std::string& gen, const std::string emb, const slong prec=64);

    ~renf_class() noexcept;

    renf_class& operator = (const renf_class&) noexcept;
    renf_class& operator = (renf_class&&) noexcept;

    slong degree() const noexcept;

    // standard elements
    renf_elem_class zero() const noexcept;
    renf_elem_class one() const noexcept;
    renf_elem_class gen() const noexcept;

    bool operator == (const renf_class&) const;

    const std::string& gen_name() const;

    // Prepare an input stream to read elements living in this number field from it.
    std::istream& set_pword(std::istream&);

    // Raw access to the underlying renf_t
    ::renf_t& renf_t() noexcept;
    const ::renf_t& renf_t() const noexcept;

    [[deprecated("Use renf_t() instead.")]]
    renf * get_renf() noexcept { return nf; }
  private:
    // The name of the generator
    std::string name;

    // The actual underlying renf_t
    ::renf_t nf;
};

struct renf_elem_class :
    boost::ordered_field_operators<renf_elem_class>,
    boost::ordered_field_operators<renf_elem_class, signed char>,
    boost::ordered_field_operators<renf_elem_class, unsigned char>,
    boost::ordered_field_operators<renf_elem_class, signed int>,
    boost::ordered_field_operators<renf_elem_class, unsigned int>,
    boost::ordered_field_operators<renf_elem_class, signed short int>,
    boost::ordered_field_operators<renf_elem_class, unsigned short int>,
    boost::ordered_field_operators<renf_elem_class, signed long int>,
    boost::ordered_field_operators<renf_elem_class, mpz_class>,
    boost::ordered_field_operators<renf_elem_class, mpq_class>
{
    // The zero element of the rationals
    renf_elem_class() noexcept;
    renf_elem_class(const renf_elem_class&) noexcept;
    renf_elem_class(renf_elem_class&&) noexcept;
    // The zero element of the field k
    explicit renf_elem_class(std::shared_ptr<renf_class> k) noexcept;
    template <typename T>
    explicit renf_elem_class(T&&) noexcept;
    template <typename T>
    renf_elem_class(std::shared_ptr<renf_class> k, T&&) noexcept;

    ~renf_elem_class() noexcept;

    renf_elem_class& operator = (const renf_elem_class&) noexcept;
    renf_elem_class& operator = (renf_elem_class&&) noexcept;
    template <typename T>
    renf_elem_class& operator = (T&&) noexcept;

    // containing number field; holds a nullptr if this is a rational number
    std::shared_ptr<renf_class>& parent() noexcept;
    const std::shared_ptr<renf_class>& parent() const noexcept;

    // testing
    bool is_fmpq() const noexcept;
    bool is_zero() const noexcept;
    bool is_one() const noexcept;
    bool is_integer() const noexcept;
    bool is_rational() const noexcept;

    // raw data access
    ::fmpq_t& fmpq_t() noexcept;
    const ::fmpq_t& fmpq_t() const noexcept;
    ::renf_elem_t& renf_elem_t() noexcept;
    const ::renf_elem_t& renf_elem_t() const noexcept;

    // data conversion
    mpz_class den() const;
    mpz_class num() const;
    explicit operator mpq_class() const;
    std::vector<mpz_class> num_vector() const noexcept;

    // floor, ceil, round, approximation
    mpz_class floor() const noexcept;
    mpz_class ceil() const noexcept;
    int sgn() const noexcept;
    explicit operator double() const noexcept;

    // unary operations
    renf_elem_class operator - () const noexcept;
    renf_elem_class operator + () const noexcept;

    // string output
    std::string str(int flag=EANTIC_STR_ALG|EANTIC_STR_D) const noexcept;

    // binary operations
    renf_elem_class& operator += (const renf_elem_class&) noexcept;
    renf_elem_class& operator -= (const renf_elem_class&) noexcept;
    renf_elem_class& operator *= (const renf_elem_class&) noexcept;
    renf_elem_class& operator /= (const renf_elem_class&);
    bool operator == (const renf_elem_class&) const noexcept;
    bool operator < (const renf_elem_class&) const noexcept;

    template <typename T>
    renf_elem_class& operator += (T&&) noexcept;
    template <typename T>
    renf_elem_class& operator -= (T&&) noexcept;
    template <typename T>
    renf_elem_class& operator *= (T&&) noexcept;
    template <typename T>
    renf_elem_class& operator /= (T&&);
    template <typename T>
    bool operator == (T&&) const noexcept;
    template <typename T>
    bool operator < (T&&) const noexcept;

    // deprecated pre-1.0 methods
    [[deprecated("use str() instead")]]
    std::string get_str(int flag=EANTIC_STR_ALG|EANTIC_STR_D) const noexcept;
    [[deprecated("use fmpq_t() instead")]]
    ::fmpq * get_fmpq() const noexcept;
    [[deprecated("use renf_elem_t() instead")]]
    renf_elem_srcptr get_renf_elem() const;
    [[deprecated("use den() instead")]]
    mpz_class get_den() const;
    [[deprecated("use num() instead")]]
    mpz_class get_num() const;
    [[deprecated("use static_cast<mpq_class>() instead")]]
    mpq_class get_rational() const;
    [[deprecated("use num_vector() instead")]]
    std::vector<mpz_class> get_num_vector() const noexcept;
    [[deprecated("use static_cast<double>() instead")]]
    double get_d() const noexcept;

    /* I/O */
    friend std::ostream& operator << (std::ostream &, const renf_elem_class&);
    friend std::istream& operator >> (std::istream &, renf_elem_class&);

  private:
    // the parent number field; holds a nullptr if the element is rational
    std::shared_ptr<renf_class> nf;
    // the underlying element when nf != nullptr
    mutable ::renf_elem_t a;
    // the underlying element when nf == nullptr
    mutable ::fmpq_t b;

};

} // end of namespace

#endif
