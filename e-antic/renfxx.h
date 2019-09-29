/*  This is a -*- C++ -*- header file.

    Copyright (C) 2016-2018 Vincent Delecroix
    Copyright (C) 2019 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_RENFXX_H
#define E_ANTIC_RENFXX_H

#include <cassert>
#include <vector>
#include <type_traits>
#include <memory>

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
class renf_class : public std::enable_shared_from_this<renf_class>, boost::equality_comparable<renf_class> {
    // The trivial number field adjoining a root of (x - 1) to the rationals
    renf_class() noexcept;
    renf_class(const ::renf_t, const std::string &) noexcept;
    renf_class(const std::string &, const std::string &, const std::string &, const slong);

public:
    // The trivial number field adjoining a root of (x - 1) to the rationals
    static std::shared_ptr<const renf_class> make() noexcept;
    static std::shared_ptr<const renf_class> make(const ::renf_t, const std::string & gen = "a") noexcept;
    static std::shared_ptr<const renf_class> make(const std::string & minpoly, const std::string & gen, const std::string &emb, const slong prec = 64);

    ~renf_class() noexcept;

    // Assignment is not possible since one renf_class uniquely corresponds to
    // an embedded number field.
    renf_class & operator=(const renf_class &) = delete;

    slong degree() const noexcept;

    // standard elements
    renf_elem_class zero() const noexcept;
    renf_elem_class one() const noexcept;
    renf_elem_class gen() const noexcept;

    bool operator==(const renf_class &) const noexcept;

    const std::string & gen_name() const noexcept { return name; }

    // Prepare an input stream to read elements living in this number field
    // from it.
    [[deprecated("use renfxx_cereal.h instead.")]]
    std::istream & set_pword(std::istream &) const noexcept;

    std::string to_string() const noexcept;
    friend std::ostream & operator<<(std::ostream &, const renf_class &);

    // Raw access to the underlying renf_t; we do not return a const renf_t
    // since calls in the C API might need to modify it (e.g., to refine the
    // stored embedding) even though they are morally const.
    ::renf_t & renf_t() const noexcept { return nf; }

    [[deprecated("Use renf_t() instead.")]] renf * get_renf() const noexcept { return nf; }
    [[deprecated("Use set_pword() instead.")]] std::istream & set_istream(std::istream &) const noexcept;

private:
    // The name of the generator
    std::string name;

    // The actual underlying renf_t
    mutable ::renf_t nf;

    // Serialization, see renfxx_cereal.h
    friend cereal::access;
};

class renf_elem_class : boost::ordered_field_operators<renf_elem_class>,
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
    template <typename Integer, typename std::enable_if_t<std::is_integral_v<Integer>, int> = 0>
    renf_elem_class(Integer) noexcept;
    renf_elem_class(const mpz_class &) noexcept;
    renf_elem_class(const mpq_class &) noexcept;
    renf_elem_class(const fmpq_t) noexcept;
    // The zero element in k; note that all overloads that take the field as a
    // parameter hold a non-owning reference to the field, i.e., the element is
    // only valid while that reference is.
    explicit renf_elem_class(std::shared_ptr<const renf_class> k) noexcept;
    // An integer in the field k
    renf_elem_class(std::shared_ptr<const renf_class> k, const mpz_class &) noexcept;
    // A rational in the field k
    renf_elem_class(std::shared_ptr<const renf_class> k, const mpq_class &) noexcept;
    // A rational in the field k
    renf_elem_class(std::shared_ptr<const renf_class> k, const fmpq_t) noexcept;
    // An integer in the field k
    template <typename Integer, typename std::enable_if_t<std::is_integral_v<Integer>, int> = 0>
    renf_elem_class(std::shared_ptr<const renf_class> k, const Integer) noexcept;
    // Parse the string into an element in the field k
    renf_elem_class(std::shared_ptr<const renf_class> k, const std::string &);
    // The element Σc_i·α^i where α is the generator of the field k; the number
    // of coefficients must not exceed the degree of the field.
    template <typename C> renf_elem_class(std::shared_ptr<const renf_class> k, const std::vector<C> &) noexcept;

    ~renf_elem_class() noexcept;

    // Note that we do not implement any operator= for other types explicitly
    // but funnel everything through the implicit constructors above and the
    // move assignment here.
    renf_elem_class & operator=(const renf_elem_class &) noexcept;
    renf_elem_class & operator=(renf_elem_class &&) noexcept;

    // containing number field; holds a nullptr if this is a rational number
    const std::shared_ptr<const renf_class>& parent() const noexcept { return nf; }

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
    mpz_class num() const noexcept;
    mpz_class den() const noexcept;
    explicit operator mpq_class() const noexcept;
    std::vector<mpz_class> num_vector() const noexcept;
    explicit operator std::string() const noexcept;
    std::string to_string(int flags = EANTIC_STR_ALG | EANTIC_STR_D) const noexcept;

    // gcd of numerator
    mpz_class num_content() const noexcept;

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

    // powering
    renf_elem_class pow(int) const noexcept;

    // binary operations with primitive integer types
    template <typename Integer>
    std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &> operator+=(Integer) noexcept;
    template <typename Integer>
    std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &> operator-=(Integer) noexcept;
    template <typename Integer>
    std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &> operator*=(Integer) noexcept;
    template <typename Integer> std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &> operator/=(Integer);
    template <typename Integer> std::enable_if_t<std::is_integral_v<Integer>, bool> operator==(Integer) const noexcept;
    template <typename Integer> std::enable_if_t<std::is_integral_v<Integer>, bool> operator<(Integer) const noexcept;
    template <typename Integer> std::enable_if_t<std::is_integral_v<Integer>, bool> operator>(Integer) const noexcept;

    // deprecated pre-1.0 methods
    [[deprecated("use to_string() instead")]] std::string get_str(int flag = EANTIC_STR_ALG | EANTIC_STR_D) const noexcept;
    [[deprecated("use fmpq_t() instead")]] ::fmpq * get_fmpq() const;
    [[deprecated("use renf_elem_t() instead")]] renf_elem_srcptr get_renf_elem() const;
    [[deprecated("use den() instead")]] mpz_class get_den() const;
    [[deprecated("use num() instead")]] mpz_class get_num() const;
    [[deprecated("use static_cast<mpq_class>() instead")]] mpq_class get_rational() const;
    [[deprecated("use num_vector() instead")]] std::vector<mpz_class> get_num_vector() const noexcept;
    [[deprecated("use static_cast<double>() instead")]] double get_d() const noexcept;

    // I/O
    friend std::ostream & operator<<(std::ostream &, const renf_elem_class &);
    friend std::istream & operator>>(std::istream &, renf_elem_class &);

private:
    // The parent number field; a nullptr if the element is rational.
    std::shared_ptr<const renf_class> nf;
    // The underlying element when nf != nullptr.
    // We need mutability as calls might need to refine the precision of
    // the stored embedding.
    mutable ::renf_elem_t a;
    // the underlying element when nf == nullptr
    ::fmpq_t b;

    // Make this->nf == nf; when this->nf != nullptr, only implemented in
    // trivial cases
    void promote(std::shared_ptr<const renf_class> nf) noexcept;
    template <typename T, typename fmpq_op, typename renf_op>
    int cmp(T && rhs, fmpq_op fmpq, renf_op renf) const noexcept;
    template <typename T, typename fmpq_op, typename renf_op> void inplace_binop(T && rhs, fmpq_op fmpq, renf_op renf);
    // Assign value to this element without changing this->nf.
    void assign(slong) noexcept;
    void assign(ulong) noexcept;
    void assign(const ::fmpq_t) noexcept;
    void assign(const mpz_class &) noexcept;
    void assign(const mpq_class &) noexcept;

    // Serialization, see renfxx_cereal.h
    friend cereal::access;
};

// overloads for global functions

inline mpz_class floor(renf_elem_class x) { return x.floor(); }
inline mpz_class ceil(renf_elem_class x) { return x.ceil(); }
inline renf_elem_class pow(renf_elem_class x, int exp) { return x.pow(exp); }

// generic construction and assignment

template <auto = 0> constexpr bool false_v = false;
template <typename = void> constexpr bool false_t = false;

template <bool fallback_to_string, typename Integer> auto to_supported_integer(Integer value) noexcept
{
    using S = std::remove_cv_t<std::remove_reference_t<Integer>>;

    static_assert(!std::is_same_v<S, mpz_class> && !std::is_same_v<S, mpq_class>,
        "Specialized operators should be used for mpz/mpq instead.");

    static_assert(!std::is_same_v<bool, S>, "Cannot create renf_elem_class from bool.");

    // This integer type is not natively understood, so we need to convert it
    // first
    using Supported = std::conditional_t<std::numeric_limits<S>::is_signed, slong, ulong>;
    if constexpr (std::numeric_limits<Supported>::min() <= std::numeric_limits<S>::min() &&
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

template <typename Integer, typename std::enable_if_t<std::is_integral_v<Integer>, int>>
renf_elem_class::renf_elem_class(Integer value) noexcept
{
    nf = nullptr;
    fmpq_init(b);

    assign(to_supported_integer<true>(value));
}

template <typename Coefficient>
renf_elem_class::renf_elem_class(const std::shared_ptr<const renf_class> k, const std::vector<Coefficient> & coefficients) noexcept
    : renf_elem_class(std::move(k))
{
    assert(coefficients.size() <= nf->degree() &&
        "can not assign renf_elem_class from vector whose size exceeds number field degree");

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
                fmpq_poly_set_coeff_si(p, i, c);
            else
                fmpq_poly_set_coeff_ui(p, i, c);
        }
    }

    renf_elem_set_fmpq_poly(a, p, nf->renf_t());
    fmpq_poly_clear(p);
}

template <typename Integer, typename std::enable_if_t<std::is_integral_v<Integer>, int>>
renf_elem_class::renf_elem_class(std::shared_ptr<const renf_class> k, Integer value) noexcept
{
    nf = std::move(k);
    renf_elem_init(a, nf->renf_t());

    assign(to_supported_integer<true>(value));
}

// generic operators

template <typename T, typename fmpq_op, typename renf_op>
int renf_elem_class::cmp(T && rhs, fmpq_op fmpq, renf_op renf) const noexcept
{
    return is_fmpq() ? fmpq(b, std::forward<T>(rhs)) : renf(a, std::forward<T>(rhs), nf->renf_t());
}

template <typename Integer>
std::enable_if_t<std::is_integral_v<Integer>, bool> renf_elem_class::operator<(Integer rhs) const noexcept
{
    auto other = to_supported_integer<false>(rhs);
    if constexpr (std::is_same_v<decltype(other), slong>)
        return cmp(other, fmpq_cmp_si, renf_elem_cmp_si) < 0;
    else
        return cmp(other, fmpq_cmp_ui, renf_elem_cmp_ui) < 0;
}

template <typename Integer>
std::enable_if_t<std::is_integral_v<Integer>, bool> renf_elem_class::operator>(Integer rhs) const noexcept
{
    auto other = to_supported_integer<false>(rhs);
    if constexpr (std::is_same_v<decltype(other), slong>)
        return cmp(other, fmpq_cmp_si, renf_elem_cmp_si) > 0;
    else
        return cmp(other, fmpq_cmp_ui, renf_elem_cmp_ui) > 0;
}

template <typename Integer>
std::enable_if_t<std::is_integral_v<Integer>, bool> renf_elem_class::operator==(Integer rhs) const noexcept
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
    is_fmpq() ? fmpq(b, b, std::forward<T>(rhs)) : renf(a, a, std::forward<T>(rhs), nf->renf_t());
}

template <typename Integer>
std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &> renf_elem_class::operator+=(Integer rhs) noexcept
{
    auto other = to_supported_integer<false>(rhs);
    if constexpr (std::is_same_v<decltype(other), slong>)
        inplace_binop(other, fmpq_add_si, renf_elem_add_si);
    else
        inplace_binop(other, fmpq_add_ui, renf_elem_add_ui);
    return *this;
}

template <typename Integer>
std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &> renf_elem_class::operator-=(Integer rhs) noexcept
{
    auto other = to_supported_integer<false>(rhs);
    if constexpr (std::is_same_v<decltype(other), slong>)
        inplace_binop(other, fmpq_sub_si, renf_elem_sub_si);
    else
        inplace_binop(other, fmpq_sub_ui, renf_elem_sub_ui);
    return *this;
}

template <typename Integer>
std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &> renf_elem_class::operator*=(Integer rhs) noexcept
{
    auto other = to_supported_integer<false>(rhs);
    if constexpr (std::is_same_v<decltype(other), slong>)
        inplace_binop(other, fmpq_mul_si, renf_elem_mul_si);
    else
        inplace_binop(other, fmpq_mul_ui, renf_elem_mul_ui);
    return *this;
}

template <typename Integer>
std::enable_if_t<std::is_integral_v<Integer>, renf_elem_class &> renf_elem_class::operator/=(Integer rhs)
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
