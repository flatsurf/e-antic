/*  This is a -*- C++ -*- header file.

    Copyright (C) 2016-2018 Vincent Delecroix
                  2019-2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

/// Elements of Real Embedded Number Fields

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

/// An element of a Real Embedded Number Field.
///
/// Each element lives in a fixed number field. If the number field is an
/// extension of the rationals of degree $d$, its elements are represented by
/// rational polynomials of degree $d - 1$.
/// ```
/// #include <e-antic/renf_class.hpp>
/// #include <e-antic/renf_elem_class.hpp>
///
/// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
/// auto a = eantic::renf_elem_class(*K, std::vector{-1, 1});
/// std::cout << a;
/// // -> (x-1 ~ 0.41421356)
/// ```
///
/// The underlying data can be accessed directly, e.g., with
/// [num_vector]() and [den]().
///
/// Note that elements do not need to worry about the lifetime of the number
/// field they are contained in. The number field will be kept alive by smart
/// pointers as long as elements in it are around:
/// ```
/// #include <e-antic/renf_class.hpp>
/// #include <e-antic/renf_elem_class.hpp>
///
/// auto gen = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1")->gen();
/// std::cout << gen;
/// // -> (x ~ 1.4142136)
/// ```
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
    /// Create the zero element of the rationals.
    renf_elem_class() noexcept;

    renf_elem_class(const renf_elem_class &);
    renf_elem_class(renf_elem_class &&) noexcept;

    /// ==* `renf_elem_class(integer/rational)` *==
    /// Create an element in the rationals.
    /// These are shortcuts equivalent to calling
    /// `renf_elem_class(*renf_class::make(), value)`.
    renf_elem_class(int);
    renf_elem_class(unsigned int);
    renf_elem_class(long);
    renf_elem_class(unsigned long);
    renf_elem_class(long long);
    renf_elem_class(unsigned long long);
    renf_elem_class(const mpz_class &);
    renf_elem_class(const fmpz_t);
    renf_elem_class(const mpq_class &);
    renf_elem_class(const fmpq_t);

    /// Create the zero element in the field `k`.
    explicit renf_elem_class(const renf_class& k);

    /// ==* `renf_elem_class(const renf_class&, integer/rational)` *==
    /// Create a rational element in the field `k`.
    renf_elem_class(const renf_class& k, int);
    renf_elem_class(const renf_class& k, unsigned int);
    renf_elem_class(const renf_class& k, long);
    renf_elem_class(const renf_class& k, unsigned long);
    renf_elem_class(const renf_class& k, long long);
    renf_elem_class(const renf_class& k, unsigned long long);
    renf_elem_class(const renf_class& k, const mpz_class &);
    renf_elem_class(const renf_class& k, const fmpz_t);
    renf_elem_class(const renf_class& k, const mpq_class &);
    renf_elem_class(const renf_class& k, const fmpq_t);

    /// Create an element in the field `k` from an existing element.
    /// This attempts to coerce the element into the field `k`. Currently, this
    /// is only implemented in trivial cases.
    renf_elem_class(const renf_class& k, const renf_elem_class&);

    /// Create an element in the field `k` from a string representation of its
    /// underlying polynomial.
    /// ```
    /// #include <e-antic/renf_class.hpp>
    /// #include <e-antic/renf_elem_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
    /// eantic::renf_elem_class(*K, "x") == K->gen()
    /// // -> true
    /// ```
    renf_elem_class(const renf_class& k, const std::string &);

    /// ==* `renf_elem_class(vector<integer/rational>)` *==
    /// Create the element $\sum c_i a^i$ where $a$ is the generator.
    /// The number of coefficients must not exceed the degree of the field.
    renf_elem_class(const renf_class& k, const std::vector<int> &);
    renf_elem_class(const renf_class& k, const std::vector<unsigned int> &);
    renf_elem_class(const renf_class& k, const std::vector<long> &);
    renf_elem_class(const renf_class& k, const std::vector<unsigned long> &);
    renf_elem_class(const renf_class& k, const std::vector<mpz_class> &);
    renf_elem_class(const renf_class& k, const std::vector<mpq_class> &);

    ~renf_elem_class() noexcept;

    /// ==* `operator=(integer/rational)` *==
    /// Reset this element to a rational number.
    /// The [parent]() of the element is not preserved, i.e., it's always the
    /// rational field. To create a rational element in the number field, use
    /// the corresponding constructor.
    /// ```
    /// #include <e-antic/renf_class.hpp>
    /// #include <e-antic/renf_elem_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
    /// auto a = K->gen();
    /// a.parent() == *K
    /// // -> true
    /// a = 1;
    /// a.parent() == *K
    /// // -> false
    /// a = eantic::renf_elem_class(*K, 1);
    /// a.parent() == *K
    /// // -> true
    /// ```
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

    /// ==* `operator=(number field element)` *==
    /// Reset this element to another element.
    /// The [parent]() of the element is not preserved, i.e., the resulting
    /// parent is always the parent of the argument.
    renf_elem_class & operator=(const renf_elem_class &);
    renf_elem_class & operator=(renf_elem_class &&) noexcept;

    /// Return the containing number field.
    const renf_class& parent() const { return *nf; }

    /// Return whether this element is the zero element in its field.
    bool is_zero() const;

    /// Return whether this element is the one element in its field.
    bool is_one() const;

    /// Return whether this element is a rational integer.
    /// ```
    /// #include <e-antic/renf_class.hpp>
    /// #include <e-antic/renf_elem_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
    /// K->gen().is_integer()
    /// // -> false
    ///
    /// K->one().is_integer()
    /// // -> true
    /// ```
    bool is_integer() const;

    /// Return whether this element is a rational number.
    /// ```
    /// #include <e-antic/renf_class.hpp>
    /// #include <e-antic/renf_elem_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
    /// K->gen().is_rational()
    /// // -> false
    ///
    /// K->one().is_rational()
    /// // -> true
    /// ```
    bool is_rational() const;

    // We do not return a const renf_elem_t. Parts of the C API might need a
    // non-const one to refine the underlying representation even though they
    // are morally treating this as a const.
    std::add_lvalue_reference_t<::renf_elem_t> renf_elem_t() const;

    /// Return the numerator of this element.
    /// The element must be rational for this to work, see [num_content]() for
    /// non-rational elements.
    mpz_class num() const;

    /// Return the denominator of this element.
    /// For non-rational elements, this returns the least common multiple of
    /// the denominators of the coefficients when written as $\sum c_i a^i$
    /// where $a$ is the field generator.
    mpz_class den() const;

    /// Convert this element to a [GMP integer](https://gmplib.org/manual/C_002b_002b-Interface-Integers).
    /// The element must be [an integer](<> "is_integer").
    /// ```
    /// #include <e-antic/renf_class.hpp>
    /// #include <e-antic/renf_elem_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
    /// std::cout << static_cast<mpz_class>(K->one());
    /// // -> 1
    /// ```
    explicit operator mpz_class() const;

    /// Convert this element to a [GMP rational number](https://gmplib.org/manual/C_002b_002b-Interface-Rationals).
    /// The element must be [a rational](<> "is_rational").
    /// ```
    /// #include <e-antic/renf_class.hpp>
    /// #include <e-antic/renf_elem_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
    /// std::cout << static_cast<mpq_class>(K->one());
    /// // -> 1
    /// ```
    explicit operator mpq_class() const;

    /// Return the numerators of the coefficients of this element when written
    /// as $\sum c_i a^i$ where $a$ is the field generator.
    std::vector<mpz_class> num_vector() const;

    /// ==* `string to_string() const` *==
    /// Return a string representation of this element.
    /// The optional flag of `to_string()` is passed on to
    /// [renf_elem_get_str_pretty]() internally. Other than that, `to_string()`
    /// and the explicit string conversion are just aliases of each other:
    /// ```
    /// #include <e-antic/renf_class.hpp>
    /// #include <e-antic/renf_elem_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
    /// K->gen().to_string() == static_cast<std::string>(K->gen())
    /// // -> true
    /// ```
    std::string to_string(int flags = EANTIC_STR_ALG | EANTIC_STR_D) const;
    explicit operator std::string() const;

    /// Return the greatest common divisor of [num_vector]().
    /// ```
    /// #include <e-antic/renf_class.hpp>
    /// #include <e-antic/renf_elem_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
    /// std::cout << (4*K->gen() + 2).num_content();
    /// // -> 2
    /// std::cout << K->zero().num_content();
    /// // -> 0
    /// ```
    mpz_class num_content() const;

    /// Return the integer floor of this element.
    /// ```
    /// #include <e-antic/renf_class.hpp>
    /// #include <e-antic/renf_elem_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
    /// std::cout << K->gen().floor();
    /// // -> 1
    /// std::cout << (-K->gen()).floor();
    /// // -> -2
    /// ```
    mpz_class floor() const;

    /// Return the integer ceil of this element.
    /// ```
    /// #include <e-antic/renf_class.hpp>
    /// #include <e-antic/renf_elem_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
    /// std::cout << K->gen().ceil();
    /// // -> 2
    /// std::cout << (-K->gen()).ceil();
    /// // -> -1
    /// ```
    mpz_class ceil() const;

    /// Return the sign of this element.
    /// Returns one of `-1`, `0` or `+1`.
    int sgn() const;

    /// Return the double closest to this element.
    /// Ties are broken with [ARF_RND_NEAR]().
    /// ```
    /// #include <e-antic/renf_class.hpp>
    /// #include <e-antic/renf_elem_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
    /// std::cout << static_cast<double>(K->gen());
    /// // -> 1.41421
    /// ```
    explicit operator double() const;

    /// Return the negative of this element.
    /// ```
    /// #include <e-antic/renf_class.hpp>
    /// #include <e-antic/renf_elem_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
    /// std::cout << -K->gen();
    /// // -> (-x ~ -1.4142136)
    /// ```
    renf_elem_class operator-() const;

    /// Return the positive of this element.
    /// Returns the element unchanged.
    renf_elem_class operator+() const;

    /// Return whether this element is non-zero.
    /// ```
    /// #include <e-antic/renf_class.hpp>
    /// #include <e-antic/renf_elem_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
    /// bool(K->gen())
    /// // -> true
    /// ```
    explicit operator bool() const;

    /// ==* Arithmetic in the Number Field *==
    /// ELements in a number field support the usual arithmetic operators `+`, `-`, `*`, `/`.
    /// Internally, these operators are derived from the corresponding inplace
    /// operators `+=`, `-=`, `*=`, `/=`. Note that arithmetic between elements
    /// in different number fields is currently only supported in trivial
    /// cases.
    renf_elem_class & operator+=(const renf_elem_class &);
    renf_elem_class & operator-=(const renf_elem_class &);
    renf_elem_class & operator*=(const renf_elem_class &);
    renf_elem_class & operator/=(const renf_elem_class &);

    /// ==* Relational Operators *==
    /// Elements can be compared with the usual operators `==`, `!=`, `<`,
    /// `<=`, `>=`, `>`.
    /// Internally, these operators are all derived from the definition of `==` and `<`.
    friend bool operator==(const renf_elem_class &, const renf_elem_class &);
    friend bool operator<(const renf_elem_class &, const renf_elem_class &);

    /// Return the integer floor of the division of this element by `rhs`.
    mpz_class floordiv(const renf_elem_class & rhs) const;

    /// Return an integer power of this element.
    renf_elem_class pow(int exponent) const;

    /// ==* Arithmetic with Integers & Rationals *==
    /// Elements in a number field and integers/rationals can be combined with
    /// the usual arithmetic operators `+`, `-`, `*`, `/` and the corresponding
    /// inplace operators `+=`, `-=`, `*=`, `/=`.
    renf_elem_class & operator+=(int);
    renf_elem_class & operator-=(int);
    renf_elem_class & operator*=(int);
    renf_elem_class & operator/=(int);
    renf_elem_class & operator+=(unsigned int);
    renf_elem_class & operator-=(unsigned int);
    renf_elem_class & operator*=(unsigned int);
    renf_elem_class & operator/=(unsigned int);
    renf_elem_class & operator+=(long);
    renf_elem_class & operator-=(long);
    renf_elem_class & operator*=(long);
    renf_elem_class & operator/=(long);
    renf_elem_class & operator+=(unsigned long);
    renf_elem_class & operator-=(unsigned long);
    renf_elem_class & operator*=(unsigned long);
    renf_elem_class & operator/=(unsigned long);
    renf_elem_class & operator+=(long long);
    renf_elem_class & operator-=(long long);
    renf_elem_class & operator*=(long long);
    renf_elem_class & operator/=(long long);
    renf_elem_class & operator+=(unsigned long long);
    renf_elem_class & operator-=(unsigned long long);
    renf_elem_class & operator*=(unsigned long long);
    renf_elem_class & operator/=(unsigned long long);
    renf_elem_class& operator+=(const mpz_class&);
    renf_elem_class& operator-=(const mpz_class&);
    renf_elem_class& operator*=(const mpz_class&);
    renf_elem_class& operator/=(const mpz_class&);
    renf_elem_class& operator+=(const mpq_class&);
    renf_elem_class& operator-=(const mpq_class&);
    renf_elem_class& operator*=(const mpq_class&);
    renf_elem_class& operator/=(const mpq_class&);

    /// ==* Relational Operators with Integers & Rationals *==
    /// Elements in a number field and integers/rationals can be compared with
    /// the usual relational operators `==`, `!=`, `<`, `<=`, `>=`, `>`.
    friend bool operator==(const renf_elem_class&, int);
    friend bool operator<(const renf_elem_class&, int);
    friend bool operator>(const renf_elem_class&, int);
    friend bool operator==(const renf_elem_class&, unsigned int);
    friend bool operator<(const renf_elem_class&, unsigned int);
    friend bool operator>(const renf_elem_class&, unsigned int);
    friend bool operator==(const renf_elem_class&, long);
    friend bool operator<(const renf_elem_class&, long);
    friend bool operator>(const renf_elem_class&, long);
    friend bool operator==(const renf_elem_class&, unsigned long);
    friend bool operator<(const renf_elem_class&, unsigned long);
    friend bool operator>(const renf_elem_class&, unsigned long);
    friend bool operator==(const renf_elem_class&, long long);
    friend bool operator<(const renf_elem_class&, long long);
    friend bool operator>(const renf_elem_class&, long long);
    friend bool operator==(const renf_elem_class&, unsigned long long);
    friend bool operator<(const renf_elem_class&, unsigned long long);
    friend bool operator>(const renf_elem_class&, unsigned long long);
    friend bool operator==(const renf_elem_class&, const mpz_class&);
    friend bool operator<(const renf_elem_class&, const mpz_class&);
    friend bool operator>(const renf_elem_class&, const mpz_class&);
    friend bool operator==(const renf_elem_class&, const mpq_class&);
    friend bool operator<(const renf_elem_class&, const mpq_class&);
    friend bool operator>(const renf_elem_class&, const mpq_class&);

    /// Deprecated methods from e-antic releases before 1.0.0.
    /// \exclude
    [[deprecated("use to_string() instead")]] std::string get_str(int flag = EANTIC_STR_ALG | EANTIC_STR_D) const;
    [[deprecated("use renf_elem_t() instead")]] renf_elem_srcptr get_renf_elem() const;
    [[deprecated("use den() instead")]] mpz_class get_den() const;
    [[deprecated("use num() instead")]] mpz_class get_num() const;
    [[deprecated("use static_cast<mpq_class>() instead")]] mpq_class get_rational() const;
    [[deprecated("use num_vector() instead")]] std::vector<mpz_class> get_num_vector() const;
    [[deprecated("use static_cast<double>() instead")]] double get_d() const;

    /// ==* I/O *==
    /// Elements can be written to streams and read from streams.
    /// However, reading non-rational elements from a stream is discouraged as
    /// it relies on the deprecated `set_pword()` method.
    /// ```
    /// #include <strstream>
    /// #include <e-antic/renf_class.hpp>
    /// #include <e-antic/renf_elem_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
    /// auto a = K->gen();
    /// std::strstream s;
    /// s << a;
    /// std::cout << s.str();
    /// // -> (x ~ 1.4142136)
    ///
    /// s.seekp(0);
    /// // Before we can read from the stream, we must attach the number field to it.
    /// K->set_pword(s);
    /// s >> a;
    /// ```
    friend std::ostream & operator<<(std::ostream &, const renf_elem_class &);
    friend std::istream & operator>>(std::istream &, renf_elem_class &);

    /// Efficiently swap two number field elements.
    LIBEANTIC_API friend void swap(renf_elem_class& lhs, renf_elem_class& rhs) noexcept;

private:
    // The parent number field.
    boost::intrusive_ptr<const renf_class> nf;

    // The underlying element.
    // We need mutability as calls might need to refine the precision of the
    // stored embedding.
    mutable ::renf_elem_t a;

    // Allow access by serialization, see cereal.hpp
    friend cereal::access;
};

// Return the integer floor of the element.
// Identical to calling `x.floor()`.
LIBEANTIC_API mpz_class floor(const renf_elem_class& x);

// Return the integer ceil of the element.
// Identical to calling `x.ceil()`.
LIBEANTIC_API mpz_class ceil(const renf_elem_class& x);

// Return an integer power of this element.
// Identical to calling `x.pow(exp)`.
LIBEANTIC_API renf_elem_class pow(const renf_elem_class& x, int exp);

} // end of namespace

namespace std {
template <>
struct LIBEANTIC_API hash<eantic::renf_elem_class> {
  size_t operator()(const eantic::renf_elem_class&) const;
};
}  // namespace std

#endif
