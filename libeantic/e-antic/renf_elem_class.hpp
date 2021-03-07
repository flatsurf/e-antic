/*  This is a -*- C++ -*- header file.

    Copyright (C) 2016-2018 Vincent Delecroix
                  2019-2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_RENF_ELEM_CLASS_HPP
#define E_ANTIC_RENF_ELEM_CLASS_HPP

#include <gmpxx.h>
#include <boost/operators.hpp>
#include <flint/fmpq.h>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <vector>

#include "renfxx_fwd.hpp"
#include "renf_elem.h"

namespace eantic {

class LIBEANTIC_API renf_elem_class : boost::ordered_field_operators<renf_elem_class>,
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
    renf_elem_class(const fmpz_t);
    renf_elem_class(const fmpq_t);
    // The zero element in k; note that all overloads that take the field as a
    // parameter hold a non-owning reference to the field, i.e., the element is
    // only valid while that reference is.
    explicit renf_elem_class(const renf_class& k);
    // An integer in the field k
    renf_elem_class(const renf_class& k, const mpz_class &);
    // A rational in the field k
    renf_elem_class(const renf_class& k, const mpq_class &);
    // An integer in the field k
    renf_elem_class(const renf_class& k, const fmpz_t);
    // A rational in the field k
    renf_elem_class(const renf_class& k, const fmpq_t);
    // An integer in the field k
    renf_elem_class(const renf_class& k, int);
    renf_elem_class(const renf_class& k, unsigned int);
    renf_elem_class(const renf_class& k, long);
    renf_elem_class(const renf_class& k, unsigned long);
    renf_elem_class(const renf_class& k, long long);
    renf_elem_class(const renf_class& k, unsigned long long);
    // Coerce number field element to the field k. Only implemented in trivial cases.
    renf_elem_class(const renf_class& k, const renf_elem_class&);
    // Parse the string into an element in the field k
    renf_elem_class(const renf_class& k, const std::string &);
    // The element Σc_i·α^i where α is the generator of the field k; the number
    // of coefficients must not exceed the degree of the field.
    renf_elem_class(const renf_class& k, const std::vector<int> &);
    renf_elem_class(const renf_class& k, const std::vector<unsigned int> &);
    renf_elem_class(const renf_class& k, const std::vector<long> &);
    renf_elem_class(const renf_class& k, const std::vector<unsigned long> &);
    renf_elem_class(const renf_class& k, const std::vector<mpz_class> &);
    renf_elem_class(const renf_class& k, const std::vector<mpq_class> &);

    ~renf_elem_class() noexcept;

    renf_elem_class & operator=(int);
    renf_elem_class & operator=(unsigned int);
    renf_elem_class & operator=(long);
    renf_elem_class & operator=(unsigned long);
    renf_elem_class & operator=(long long);
    renf_elem_class & operator=(unsigned long long);
    renf_elem_class & operator=(const mpz_class &);
    renf_elem_class & operator=(const mpq_class &);
    renf_elem_class & operator=(const fmpz_t);
    renf_elem_class & operator=(const fmpq_t);
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
    explicit operator mpz_class() const;
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

    mpz_class floordiv(const renf_elem_class &) const;

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
    boost::intrusive_ptr<const renf_class> nf;
    // The underlying element.
    // We need mutability as calls might need to refine the precision of
    // the stored embedding.
    mutable ::renf_elem_t a;

    // Serialization, see cereal.hpp
    friend cereal::access;

    LIBEANTIC_API friend void swap(renf_elem_class& lhs, renf_elem_class& rhs) noexcept;
};

// overloads for global functions
LIBEANTIC_API mpz_class floor(const renf_elem_class& x);
LIBEANTIC_API mpz_class ceil(const renf_elem_class& x);
LIBEANTIC_API renf_elem_class pow(const renf_elem_class& x, int exp);

} // end of namespace

namespace std {
template <>
struct LIBEANTIC_API hash<eantic::renf_elem_class> {
  size_t operator()(const eantic::renf_elem_class&) const;
};
}  // namespace std

#endif
