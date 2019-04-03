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

// TODO: Check that I did not delete any methods but rather that everything is
// deprecated.

#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/operators.hpp>
#include <gmpxx.h>

#include <e-antic/renf.h>
#include <e-antic/renf_elem.h>
#include <e-antic/renfxx_fwd.h>

namespace eantic {

// A Real Embedded Number Field
// This class provides C++ memory management for the underlying renf_t.
class renf_class : boost::equality_comparable<renf_class> {
public:
    // The trivial number field adjoining a root of (x - 1) to the rationals
    renf_class() noexcept;
    renf_class(const renf_class &) noexcept;
    renf_class(const ::renf_t &, const std::string & gen = "a") noexcept;
    renf_class(::renf_t &&, const std::string & gen = "a") noexcept;
    renf_class(const std::string & minpoly, const std::string & gen,
        const std::string emb, const slong prec = 64);

    ~renf_class() noexcept;

    renf_class & operator=(const renf_class &) noexcept;
    renf_class & operator=(renf_class &&) noexcept;

    slong degree() const noexcept;

    // standard elements
    renf_elem_class zero() const noexcept;
    renf_elem_class one() const noexcept;
    renf_elem_class gen() const noexcept;

    bool operator==(const renf_class &) const noexcept;

    const std::string & gen_name() const noexcept { return name; }

    // Prepare an input stream to read elements living in this number field
    // from it.
    std::istream & set_pword(std::istream &) noexcept;

    // Raw access to the underlying renf_t; we do not return a const renf_t
    // since calls in the C API might need to modify it (e.g., to refine the
    // stored embedding) even though they are morally const.
    ::renf_t & renf_t() const noexcept { return nf; }

    [[deprecated("Use the constructor taking std::string "
                 "instead.")]] renf_class(const char * minpoly,
        const char * gen, const char * emb, const slong prec);
    [[deprecated("Use renf_t() instead.")]] renf * get_renf() noexcept
    {
        return nf;
    }
    [[deprecated("Use set_pword() instead.")]] std::istream & set_istream(
        std::istream &) noexcept;

private:
    // The name of the generator
    std::string name;

    // The actual underlying renf_t
    mutable ::renf_t nf;
};

class renf_elem_class
    : boost::ordered_field_operators<renf_elem_class>,
      boost::ordered_field_operators<renf_elem_class, signed char>,
      boost::ordered_field_operators<renf_elem_class, unsigned char>,
      boost::ordered_field_operators<renf_elem_class, signed int>,
      boost::ordered_field_operators<renf_elem_class, unsigned int>,
      boost::ordered_field_operators<renf_elem_class, signed short int>,
      boost::ordered_field_operators<renf_elem_class, unsigned short int>,
      boost::ordered_field_operators<renf_elem_class, signed long int>,
      boost::ordered_field_operators<renf_elem_class, unsigned long int>,
      boost::ordered_field_operators<renf_elem_class, mpz_class>,
      boost::ordered_field_operators<renf_elem_class, mpq_class> {
public:
    // The zero element of the rationals
    renf_elem_class() noexcept;
    renf_elem_class(const renf_elem_class &) noexcept;
    renf_elem_class(renf_elem_class &&) noexcept;
    template <typename Integer,
        typename std::enable_if_t<std::is_integral_v<Integer>, int> = 0>
    renf_elem_class(Integer) noexcept;
    renf_elem_class(const mpz_class &) noexcept;
    renf_elem_class(const mpq_class &) noexcept;
    // Parse the string into a rational number
    explicit renf_elem_class(const std::string &);
    // The zero element in the field k; holds a non-owning reference to the
    // field, i.e., this element is only valid while that reference is.
    explicit renf_elem_class(const renf_class & k) noexcept;
    // Create a renf_elem_class in the field k by invoking the operator= on the
    // parameter. The constructed element holds a non-owning reference to the
    // renf_class passed in, i.e., it is only valid while that reference is.
    template <typename T> renf_elem_class(const renf_class & k, T &&) noexcept;

    ~renf_elem_class() noexcept;

    renf_elem_class & operator=(const renf_elem_class &) noexcept;
    renf_elem_class & operator=(renf_elem_class &&) noexcept;
    renf_elem_class & operator=(const mpz_class &) noexcept;
    renf_elem_class & operator=(const mpq_class &) noexcept;
    renf_elem_class & operator=(const fmpq_t &) noexcept;
    // TODO: With the new semantics, this assignment operator should probably
    // go away. Or we keep it for rationals?
    renf_elem_class & operator=(const std::string &);
    template <typename Integer>
    std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &> operator=(
        Integer) noexcept;
    template <typename Coefficient>
    renf_elem_class & operator=(const std::vector<Coefficient> &) noexcept;

    // containing number field; holds a nullptr if this is a rational number
    const renf_class & parent() const noexcept { return *nf; }

    // testing
    bool is_fmpq() const noexcept;
    bool is_renf_elem() const noexcept { return !is_fmpq(); }
    bool is_zero() const noexcept;
    bool is_one() const noexcept;
    bool is_integer() const noexcept;
    bool is_rational() const noexcept;

    // raw data access
    ::fmpq_t & fmpq_t() noexcept;
    const ::fmpq_t & fmpq_t() const noexcept;
    // We do not return a const renf_elem_t. Parts of the C API might need a
    // non-const one to refine the underlying representation even though they
    // are morally treating this as a const.
    ::renf_elem_t & renf_elem_t() const noexcept;

    // data conversion
    mpz_class den() const;
    mpz_class num() const;
    explicit operator mpq_class() const;
    std::vector<mpz_class> num_vector() const noexcept;
    explicit operator std::string() const noexcept;
    std::string to_string(int flags = EANTIC_STR_ALG | EANTIC_STR_D) const
        noexcept;

    // floor, ceil, round, approximation
    mpz_class floor() const noexcept;
    mpz_class ceil() const noexcept;
    int sgn() const noexcept;
    explicit operator double() const noexcept;

    // unary operations
    renf_elem_class operator-() const noexcept;
    renf_elem_class operator+() const noexcept;
    explicit operator bool() const noexcept;

    // binary operations between elements in the same number field
    renf_elem_class & operator+=(const renf_elem_class &) noexcept;
    renf_elem_class & operator-=(const renf_elem_class &) noexcept;
    renf_elem_class & operator*=(const renf_elem_class &) noexcept;
    renf_elem_class & operator/=(const renf_elem_class &);
    bool operator==(const renf_elem_class &) const noexcept;
    bool operator<(const renf_elem_class &) const noexcept;

    // binary operations with primitive integer types
    template <typename Integer>
    std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &> operator+=(
        Integer) noexcept;
    template <typename Integer>
    std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &> operator-=(
        Integer) noexcept;
    template <typename Integer>
    std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &> operator*=(
        Integer) noexcept;
    template <typename Integer>
    std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &> operator/=(
        Integer);
    template <typename Integer>
    std::enable_if_t<std::is_integral_v<Integer>, bool> operator==(
        Integer) const noexcept;
    template <typename Integer>
    std::enable_if_t<std::is_integral_v<Integer>, bool> operator<(Integer) const
        noexcept;
    template <typename Integer>
    std::enable_if_t<std::is_integral_v<Integer>, bool> operator>(Integer) const
        noexcept;

    // deprecated pre-1.0 methods
    [[deprecated("use to_string() instead")]] std::string get_str(
        int flag = EANTIC_STR_ALG | EANTIC_STR_D) const noexcept;
    [[deprecated("use fmpq_t() instead")]] ::fmpq * get_fmpq() const;
    [[deprecated("use renf_elem_t() instead")]] renf_elem_srcptr
    get_renf_elem() const;
    [[deprecated("use den() instead")]] mpz_class get_den() const;
    [[deprecated("use num() instead")]] mpz_class get_num() const;
    [[deprecated("use static_cast<mpq_class>() instead")]] mpq_class
    get_rational() const;
    [[deprecated("use num_vector() instead")]] std::vector<mpz_class>
    get_num_vector() const noexcept;
    [[deprecated("use static_cast<double>() instead")]] double get_d() const
        noexcept;

    /* I/O */
    friend std::ostream & operator<<(std::ostream &, const renf_elem_class &);
    friend std::istream & operator>>(std::istream &, renf_elem_class &);

private:
    // The parent number field; a nullptr if the element is rational.
    renf_class const * nf;
    // The underlying element when nf != nullptr.
    // We need mutability as calls might need to refine the precision of
    // the stored embedding.
    mutable ::renf_elem_t a;
    // the underlying element when nf == nullptr
    ::fmpq_t b;

    // Make this->nf == nf; when this->nf != nullptr, only implemented in
    // trivial cases
    void promote(const renf_class & nf) noexcept;
    template <typename T, typename fmpq_op, typename renf_op>
    int cmp(T && rhs, fmpq_op fmpq, renf_op renf) const noexcept;
    template <typename T, typename fmpq_op, typename renf_op>
    void inplace_binop(T && rhs, fmpq_op fmpq, renf_op renf);
};

// overloads for global functions

inline mpz_class floor(renf_elem_class x) { return x.floor(); }
inline mpz_class ceil(renf_elem_class x) { return x.ceil(); }

// generic construction and assignment

template <auto = 0> constexpr bool false_v = false;
template <typename = void> constexpr bool false_t = false;

template <typename Integer,
    typename std::enable_if_t<std::is_integral_v<Integer>, int>>
renf_elem_class::renf_elem_class(Integer value) noexcept
{
    nf = nullptr;
    fmpq_init(b);
    *this = value;
}

template <typename T>
renf_elem_class::renf_elem_class(const renf_class & k, T && value) noexcept
{
    nf = &k;
    renf_elem_init(a, nf->renf_t());
    *this = std::forward<T>(value);
}

template <bool fallback_to_string, typename Integer>
auto to_supported_integer(Integer value) noexcept
{
    using S = std::remove_cv_t<std::remove_reference_t<Integer>>;

    // This integer type is not natively understood, so we need to convert it
    // first
    using Supported =
        std::conditional_t<std::numeric_limits<S>::is_signed, slong, ulong>;
    if constexpr (std::numeric_limits<Supported>::min() <=
            std::numeric_limits<S>::min() &&
        std::numeric_limits<Supported>::max() >= std::numeric_limits<S>::max())
    {
        // We can safely cast to a supported type without overflow
        return static_cast<Supported>(value);
    }
    else if constexpr (fallback_to_string)
    {
        // The cast might not work but we can still try
        try
        {
            return boost::numeric_cast<Supported>(value);
        }
        catch (boost::bad_numeric_cast &)
        {
            // The cast did not work, convert to a string and parse that (slow
            // of course)
            return boost::lexical_cast<std::string>(value);
        }
    }
    else
    {
        static_assert(false_t<Integer>,
            "Integer type is too wide to be safely converted to slong or "
            "ulong. You should probably cast it to an mpz_class() "
            "explicitly.");
        return 0;
    }
}

template <typename Integer>
std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &>
renf_elem_class::operator=(const Integer value) noexcept
{
    using S = std::remove_cv_t<std::remove_reference_t<Integer>>;

    static_assert(
        !std::is_same_v<S, mpz_class> && !std::is_same_v<S, mpq_class>,
        "Specialized operators should be used for mpz/mpq instead.");
    static_assert(
        !std::is_same_v<bool, S>, "Cannot create renf_elem_class from bool.");

    constexpr bool is_slong = std::is_same_v<slong, S>;
    constexpr bool is_ulong = std::is_same_v<ulong, S>;

    if constexpr (is_slong || is_ulong)
    {
        if (is_fmpq())
        {
            fmpz_one(fmpq_denref(b));
            if constexpr (is_slong)
            {
                fmpz_set_si(fmpq_numref(b), value);
            }
            else
            {
                fmpz_set_ui(fmpq_numref(b), value);
            }
        }
        else
        {
            if constexpr (is_slong)
            {
                renf_elem_set_si(a, value, nf->renf_t());
            }
            else
            {
                renf_elem_set_ui(a, value, nf->renf_t());
            }
        }
        return *this;
    }
    else
    {
        return *this = to_supported_integer<true>(value);
    }
}

template <typename Coefficient>
renf_elem_class & renf_elem_class::operator=(
    const std::vector<Coefficient> & coefficients) noexcept
{
    if (coefficients.size() == 0)
    {
        return *this = 0;
    }
    else if (coefficients.size() == 1)
    {
        return *this = coefficients[0];
    }
    else
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wterminate"
        if (nf == nullptr)
            throw std::invalid_argument(
                "can not assign renf_elem_class from vector of coefficients "
                "when no number field has been set");
        if (coefficients.size() > nf->degree())
            throw std::invalid_argument(
                "can not assign renf_elem_class from vector whose size "
                "exceeds number field degree");
#pragma GCC dignostic pop

        using S = std::remove_cv_t<std::remove_reference_t<Coefficient>>;

        fmpq_poly_t p;
        fmpq_poly_init(p);
        for (size_t i = 0; i < coefficients.size(); i++)
        {
            if constexpr (std::is_same_v<S, mpz_class>)
                fmpq_poly_set_coeff_mpz(p, i, coefficients[i].__get_mp());
            else if constexpr (std::is_same_v<S, mpq_class>)
                fmpq_poly_set_coeff_mpq(p, i, coefficients[i].__get_mp());
            else
            {
                auto c = to_supported_integer<false>(coefficients[i]);
                if constexpr (std::is_same_v<decltype(c), slong>)
                {
                    fmpq_poly_set_coeff_si(p, i, c);
                }
                else
                {
                    fmpq_poly_set_coeff_ui(p, i, c);
                }
            }
        }

        renf_elem_set_fmpq_poly(a, p, nf->renf_t());
        fmpq_poly_clear(p);

        return *this;
    }
}

// generic operators

template <typename T, typename fmpq_op, typename renf_op>
int renf_elem_class::cmp(T && rhs, fmpq_op fmpq, renf_op renf) const noexcept
{
    return is_fmpq() ? fmpq(b, std::forward<T>(rhs))
                     : renf(a, std::forward<T>(rhs), nf->renf_t());
}

template <typename Integer>
std::enable_if_t<std::is_integral_v<Integer>, bool> renf_elem_class::operator<(
    Integer rhs) const noexcept
{
    auto other = to_supported_integer<false>(rhs);
    if constexpr (std::is_same_v<decltype(other), slong>)
        return cmp(other, fmpq_cmp_si, renf_elem_cmp_si) < 0;
    else
        return cmp(other, fmpq_cmp_ui, renf_elem_cmp_ui) < 0;
}

template <typename Integer>
std::enable_if_t<std::is_integral_v<Integer>, bool> renf_elem_class::operator>(
    Integer rhs) const noexcept
{
    auto other = to_supported_integer<false>(rhs);
    if constexpr (std::is_same_v<decltype(other), slong>)
        return cmp(other, fmpq_cmp_si, renf_elem_cmp_si) > 0;
    else
        return cmp(other, fmpq_cmp_ui, renf_elem_cmp_ui) > 0;
}

template <typename Integer>
std::enable_if_t<std::is_integral_v<Integer>, bool> renf_elem_class::operator==(
    Integer rhs) const noexcept
{
    auto other = to_supported_integer<false>(rhs);
    if constexpr (std::is_same_v<decltype(other), slong>)
        return cmp(other, fmpq_cmp_si, renf_elem_cmp_si) == 0;
    else
        return cmp(other, fmpq_cmp_ui, renf_elem_cmp_ui) == 0;
}

template <typename T, typename fmpq_op, typename renf_op>
void renf_elem_class::inplace_binop(T && rhs, fmpq_op fmpq, renf_op renf)
{
    using S = std::remove_cv_t<std::remove_reference_t<T>>;
    is_fmpq() ? fmpq(b, b, std::forward<T>(rhs))
              : renf(a, a, std::forward<T>(rhs), nf->renf_t());
}

template <typename Integer>
std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &>
renf_elem_class::operator+=(Integer rhs) noexcept
{
    auto other = to_supported_integer<false>(rhs);
    if constexpr (std::is_same_v<decltype(other), slong>)
        inplace_binop(other, fmpq_add_si, renf_elem_add_si);
    else
        inplace_binop(other, fmpq_add_ui, renf_elem_add_ui);
    return *this;
}

template <typename Integer>
std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &>
renf_elem_class::operator-=(Integer rhs) noexcept
{
    auto other = to_supported_integer<false>(rhs);
    if constexpr (std::is_same_v<decltype(other), slong>)
        inplace_binop(other, fmpq_sub_si, renf_elem_sub_si);
    else
        inplace_binop(other, fmpq_sub_ui, renf_elem_sub_ui);
    return *this;
}

template <typename Integer>
std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &>
renf_elem_class::operator*=(Integer rhs) noexcept
{
    auto other = to_supported_integer<false>(rhs);
    if constexpr (std::is_same_v<decltype(other), slong>)
        inplace_binop(other, fmpq_mul_si, renf_elem_mul_si);
    else
        inplace_binop(other, fmpq_mul_ui, renf_elem_mul_ui);
    return *this;
}

template <typename Integer>
std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &>
renf_elem_class::operator/=(Integer rhs)
{
    auto other = to_supported_integer<false>(rhs);
    if constexpr (std::is_same_v<decltype(other), slong>)
        inplace_binop(other, fmpq_div_si, renf_elem_div_si);
    else
        inplace_binop(other, fmpq_div_ui, renf_elem_div_ui);
    return *this;
}

} // end of namespace

#endif
