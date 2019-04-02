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

// TODO: Check that I did not delete any methods but rather that everything is deprecated.

#include <vector>

#include <boost/operators.hpp>
#include <gmpxx.h>

#include <e-antic/renf.h>
#include <e-antic/renf_elem.h>
#include <e-antic/renfxx_fwd.h>

namespace eantic {

// A Real Embedded Number Field
// This class provides C++ memory management for the underlying renf_t.
class renf_class : boost::equality_comparable<renf_class>
{
  public:
    // The trivial number field adjoining a root of (x - 1) to the rationals
    renf_class() noexcept;
    renf_class(const ::renf_t&, const std::string& gen = "a") noexcept;
    renf_class(::renf_t&&, const std::string& gen = "a") noexcept;
    renf_class(const std::string& minpoly, const std::string& gen, const std::string emb, const slong prec=64);

    ~renf_class() noexcept;

    renf_class& operator = (const renf_class&) noexcept;
    renf_class& operator = (renf_class&&) noexcept;

    slong degree() const noexcept;

    // standard elements
    renf_elem_class zero() const noexcept;
    renf_elem_class one() const noexcept;
    renf_elem_class gen() const noexcept;

    bool operator == (const renf_class&) const noexcept;

    const std::string& gen_name() const noexcept { return name; }

    // Prepare an input stream to read elements living in this number field from it.
    std::istream& set_pword(std::istream&) noexcept;

    // Raw access to the underlying renf_t; we do not return a const renf_t
    // since calls in the C API might need to modify it (e.g., to refine the
    // stored embedding) even though they are morally const.
    ::renf_t& renf_t() const noexcept { return nf; }

    [[deprecated("Use the constructor taking std::string instead.")]]
    renf_class(const char * minpoly, const char * gen, const char * emb, const slong prec);
    [[deprecated("Use renf_t() instead.")]]
    renf * get_renf() noexcept { return nf; }
    [[deprecated("Use set_pword() instead.")]]
    std::istream& set_istream(std::istream&) noexcept;
  private:
    // The name of the generator
    std::string name;

    // The actual underlying renf_t
    mutable ::renf_t nf;
};

class renf_elem_class :
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
  public:
    // The zero element of the rationals
    renf_elem_class() noexcept;
    renf_elem_class(const renf_elem_class&) noexcept;
    renf_elem_class(renf_elem_class&&) noexcept;
    // The zero element of the field k
    // The constructed element holds a non-owning reference to the renf_class
    // passed in, i.e., it is only valid while that reference is valid.
    explicit renf_elem_class(const renf_class& k) noexcept;
    template <typename T>
    explicit renf_elem_class(T&&) noexcept;
    // The constructed element holds a non-owning reference to the renf_class
    // passed in, i.e., it is only valid while that reference is valid.
    template <typename T>
    renf_elem_class(const renf_class& k, T&&) noexcept;

    ~renf_elem_class() noexcept;

    renf_elem_class& operator = (const renf_elem_class&) noexcept;
    renf_elem_class& operator = (renf_elem_class&&) noexcept;
    template <typename T>
    renf_elem_class& operator = (T&&) noexcept;

    // containing number field; holds a nullptr if this is a rational number
    const renf_class& parent() const noexcept { return *nf; }

    // testing
    bool is_fmpq() const noexcept;
    bool is_zero() const noexcept;
    bool is_one() const noexcept;
    bool is_integer() const noexcept;
    bool is_rational() const noexcept;

    // raw data access
    // TODO: All these should guard access with an assert (is_rational?) I
    // don't think the pattern of get_fmpq is good where an explicit exception
    // is thrown which indicates that it could be caught. This is low level, so
    // calling this in an invalid state is a hard error.
    ::fmpq_t& fmpq_t() noexcept;
    const ::fmpq_t& fmpq_t() const noexcept;
    // We do not return a const renf_elem_t. Parts of the C API might need a non-const one to refine the
    // underlying representation even though they are morally treating this as a const.
    ::renf_elem_t& renf_elem_t() const noexcept;

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
    ::fmpq * get_fmpq() const;
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
    // The parent number field; a nullptr if the element is rational.
    renf_class const * nf;
    // The underlying element when nf != nullptr.
    // We need mutability as calls might need to refine the precision of
    // the stored embedding.
    mutable ::renf_elem_t a;
    // the underlying element when nf == nullptr
    ::fmpq_t b;
};

// overloads for global functions

inline mpz_class floor(renf_elem_class x) { return x.floor(); }
inline mpz_class ceil(renf_elem_class x) { return x.ceil(); }

} // end of namespace

#endif
