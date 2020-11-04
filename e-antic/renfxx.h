/*  This is a -*- C++ -*- header file.

    Copyright (C) 2016-2018 Vincent Delecroix
    Copyright (C) 2019-2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_RENFXX_H
#define E_ANTIC_RENFXX_H

#include <cassert>
#include <memory>
#include <type_traits>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/operators.hpp>
#include <boost/variant.hpp>
#include <gmpxx.h>

#include <e-antic/renf.h>
#include <e-antic/renf_elem.h>
#include <e-antic/renfxx_fwd.h>

namespace eantic {

// A Real Embedded Number Field
// This class provides C++ memory management for the underlying renf_t.
class renf_class : public std::enable_shared_from_this<renf_class>, boost::equality_comparable<renf_class> {
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

    bool operator==(const renf_class &) const;

    const std::string & gen_name() const { return name; }

    // Prepare an input stream to read elements living in this number field
    // from it.
    [[deprecated("use renfxx_cereal.h instead.")]]
    std::istream & set_pword(std::istream &) const;

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

class renf_elem_class : boost::ordered_field_operators<renf_elem_class>,
                        boost::ordered_field_operators<renf_elem_class, int>,
                        boost::ordered_field_operators<renf_elem_class, unsigned int>,
                        boost::ordered_field_operators<renf_elem_class, long>,
                        boost::ordered_field_operators<renf_elem_class, unsigned long>,
                        boost::ordered_field_operators<renf_elem_class, long long>,
                        boost::ordered_field_operators<renf_elem_class, unsigned long long>,
                        boost::ordered_field_operators<renf_elem_class, mpz_class>,
                        boost::ordered_field_operators<renf_elem_class, mpq_class> {
public:
    // The zero element of the rationals
    renf_elem_class() noexcept;
    renf_elem_class(const renf_elem_class &);
    renf_elem_class(renf_elem_class &&) noexcept;
    renf_elem_class(int);
    renf_elem_class(unsigned int);
    renf_elem_class(long);
    renf_elem_class(unsigned long);
    renf_elem_class(long long);
    renf_elem_class(unsigned long long);
    renf_elem_class(const mpz_class &);
    renf_elem_class(const mpq_class &);
    renf_elem_class(const fmpq_t);
    // The zero element in k; note that all overloads that take the field as a
    // parameter hold a non-owning reference to the field, i.e., the element is
    // only valid while that reference is.
    explicit renf_elem_class(std::shared_ptr<const renf_class> k);
    // An integer in the field k
    renf_elem_class(std::shared_ptr<const renf_class> k, const mpz_class &);
    // A rational in the field k
    renf_elem_class(std::shared_ptr<const renf_class> k, const mpq_class &);
    // A rational in the field k
    renf_elem_class(std::shared_ptr<const renf_class> k, const fmpq_t);
    // An integer in the field k
    renf_elem_class(std::shared_ptr<const renf_class> k, const int);
    renf_elem_class(std::shared_ptr<const renf_class> k, const unsigned int);
    renf_elem_class(std::shared_ptr<const renf_class> k, const long);
    renf_elem_class(std::shared_ptr<const renf_class> k, const unsigned long);
    renf_elem_class(std::shared_ptr<const renf_class> k, const long long);
    renf_elem_class(std::shared_ptr<const renf_class> k, const unsigned long long);
    // Coerce number field element to the field k. Only implemented in trivial cases.
    renf_elem_class(std::shared_ptr<const renf_class> k, const renf_elem_class&);
    // Parse the string into an element in the field k
    renf_elem_class(std::shared_ptr<const renf_class> k, const std::string &);
    // The element Σc_i·α^i where α is the generator of the field k; the number
    // of coefficients must not exceed the degree of the field.
    renf_elem_class(std::shared_ptr<const renf_class> k, const std::vector<int> &);
    renf_elem_class(std::shared_ptr<const renf_class> k, const std::vector<unsigned int> &);
    renf_elem_class(std::shared_ptr<const renf_class> k, const std::vector<long> &);
    renf_elem_class(std::shared_ptr<const renf_class> k, const std::vector<unsigned long> &);
    renf_elem_class(std::shared_ptr<const renf_class> k, const std::vector<mpz_class> &);
    renf_elem_class(std::shared_ptr<const renf_class> k, const std::vector<mpq_class> &);

    ~renf_elem_class() noexcept;

    // Note that we do not implement any operator= for other types explicitly
    // but funnel everything through the implicit constructors above and the
    // move assignment here.
    renf_elem_class & operator=(const renf_elem_class &);
    renf_elem_class & operator=(renf_elem_class &&) noexcept;

    // containing number field
    const renf_class& parent() const { return *nf; }

    // testing
    bool is_zero() const;
    bool is_one() const;
    bool is_integer() const;
    bool is_rational() const;

    // We do not return a const renf_elem_t. Parts of the C API might need a
    // non-const one to refine the underlying representation even though they
    // are morally treating this as a const.
    ::renf_elem_t & renf_elem_t() const;

    // data conversion
    mpz_class num() const;
    mpz_class den() const;
    explicit operator mpq_class() const;
    std::vector<mpz_class> num_vector() const;
    explicit operator std::string() const;
    std::string to_string(int flags = EANTIC_STR_ALG | EANTIC_STR_D) const;

    // gcd of numerator
    mpz_class num_content() const;

    // floor, ceil, round, approximation
    mpz_class floor() const;
    mpz_class ceil() const;
    int sgn() const;
    explicit operator double() const;

    // unary operations
    renf_elem_class operator-() const;
    renf_elem_class operator+() const;
    explicit operator bool() const;

    // binary operations between elements in the same number field
    renf_elem_class & operator+=(const renf_elem_class &);
    renf_elem_class & operator-=(const renf_elem_class &);
    renf_elem_class & operator*=(const renf_elem_class &);
    renf_elem_class & operator/=(const renf_elem_class &);
    friend bool operator==(const renf_elem_class &, const renf_elem_class &);
    friend bool operator<(const renf_elem_class &, const renf_elem_class &);

    // powering
    renf_elem_class pow(int) const;

    // binary operations with primitive integer types
    renf_elem_class & operator+=(int);
    renf_elem_class & operator-=(int);
    renf_elem_class & operator*=(int);
    renf_elem_class & operator/=(int);
    friend bool operator==(const renf_elem_class&, int);
    friend bool operator<(const renf_elem_class&, int);
    friend bool operator>(const renf_elem_class&, int);

    renf_elem_class & operator+=(unsigned int);
    renf_elem_class & operator-=(unsigned int);
    renf_elem_class & operator*=(unsigned int);
    renf_elem_class & operator/=(unsigned int);
    friend bool operator==(const renf_elem_class&, unsigned int);
    friend bool operator<(const renf_elem_class&, unsigned int);
    friend bool operator>(const renf_elem_class&, unsigned int);

    renf_elem_class & operator+=(long);
    renf_elem_class & operator-=(long);
    renf_elem_class & operator*=(long);
    renf_elem_class & operator/=(long);
    friend bool operator==(const renf_elem_class&, long);
    friend bool operator<(const renf_elem_class&, long);
    friend bool operator>(const renf_elem_class&, long);

    renf_elem_class & operator+=(unsigned long);
    renf_elem_class & operator-=(unsigned long);
    renf_elem_class & operator*=(unsigned long);
    renf_elem_class & operator/=(unsigned long);
    friend bool operator==(const renf_elem_class&, unsigned long);
    friend bool operator<(const renf_elem_class&, unsigned long);
    friend bool operator>(const renf_elem_class&, unsigned long);

    renf_elem_class & operator+=(long long);
    renf_elem_class & operator-=(long long);
    renf_elem_class & operator*=(long long);
    renf_elem_class & operator/=(long long);
    friend bool operator==(const renf_elem_class&, long long);
    friend bool operator<(const renf_elem_class&, long long);
    friend bool operator>(const renf_elem_class&, long long);

    renf_elem_class & operator+=(unsigned long long);
    renf_elem_class & operator-=(unsigned long long);
    renf_elem_class & operator*=(unsigned long long);
    renf_elem_class & operator/=(unsigned long long);
    friend bool operator==(const renf_elem_class&, unsigned long long);
    friend bool operator<(const renf_elem_class&, unsigned long long);
    friend bool operator>(const renf_elem_class&, unsigned long long);

    // binary operations with GMP integers
    renf_elem_class& operator+=(const mpz_class&);
    renf_elem_class& operator-=(const mpz_class&);
    renf_elem_class& operator*=(const mpz_class&);
    renf_elem_class& operator/=(const mpz_class&);
    friend bool operator==(const renf_elem_class&, const mpz_class&);
    friend bool operator<(const renf_elem_class&, const mpz_class&);
    friend bool operator>(const renf_elem_class&, const mpz_class&);

    // binary operations with rational numbers
    renf_elem_class& operator+=(const mpq_class&);
    renf_elem_class& operator-=(const mpq_class&);
    renf_elem_class& operator*=(const mpq_class&);
    renf_elem_class& operator/=(const mpq_class&);
    friend bool operator==(const renf_elem_class&, const mpq_class&);
    friend bool operator<(const renf_elem_class&, const mpq_class&);
    friend bool operator>(const renf_elem_class&, const mpq_class&);

    // deprecated pre-1.0 methods
    [[deprecated("use to_string() instead")]] std::string get_str(int flag = EANTIC_STR_ALG | EANTIC_STR_D) const;
    [[deprecated("use renf_elem_t() instead")]] renf_elem_srcptr get_renf_elem() const;
    [[deprecated("use den() instead")]] mpz_class get_den() const;
    [[deprecated("use num() instead")]] mpz_class get_num() const;
    [[deprecated("use static_cast<mpq_class>() instead")]] mpq_class get_rational() const;
    [[deprecated("use num_vector() instead")]] std::vector<mpz_class> get_num_vector() const;
    [[deprecated("use static_cast<double>() instead")]] double get_d() const;

    // I/O
    friend std::ostream & operator<<(std::ostream &, const renf_elem_class &);
    friend std::istream & operator>>(std::istream &, renf_elem_class &);

private:
    // The parent number field.
    std::shared_ptr<const renf_class> nf;
    // The underlying element.
    // We need mutability as calls might need to refine the precision of
    // the stored embedding.
    mutable ::renf_elem_t a;

    // Serialization, see renfxx_cereal.h
    friend cereal::access;
};

// overloads for global functions
mpz_class floor(const renf_elem_class& x);
mpz_class ceil(const renf_elem_class& x);
renf_elem_class pow(const renf_elem_class& x, int exp);

} // end of namespace

namespace std {
template <>
struct hash<eantic::renf_class> {
  size_t operator()(const eantic::renf_class&) const;
};

template <>
struct hash<eantic::renf_elem_class> {
  size_t operator()(const eantic::renf_elem_class&) const;
};
}  // namespace std

#endif
