/*  This is a -*- C++ -*- header file.

    Copyright (C) 2016-2018 Vincent Delecroix
                  2019-2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

/// Real Embedded Number Fields

#ifndef E_ANTIC_RENF_CLASS_HPP
#define E_ANTIC_RENF_CLASS_HPP

#include <boost/operators.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <string>
#include <atomic>

#include "renfxx_fwd.hpp"
#include "renf.h"

namespace eantic {

/// A Real Embedded Number Field.
///
/// All number fields are simple extensions of the rational numbers whose
/// defining minimal polynomial has a real root.
///
/// To create a number field, use one of the [factory functions](<> "make");
/// typically, by specifying the minimal polynomial of the number field and
/// selecting a real root:
/// ```
/// #include <e-antic/renf_class.hpp>
/// 
/// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
/// ```
///
/// These factories return an [intrusive pointer](https://www.boost.org/doc/libs/1_75_0/libs/smart_ptr/doc/html/smart_ptr.html#intrusive_ptr)
/// to a number field which takes care of reference counting, i.e., you do not
/// need to worry about dangling references; any element of the number field
/// keeps the number field itself alive.
///
/// All number fields are unique. If you are familiar with SageMath, they are
/// "unique parents". If you create the same number field twice, you get a
/// reference to the same field:
/// ```
/// #include <e-antic/renf_class.hpp>
/// #include <e-antic/renf_elem_class.hpp>
///
/// auto K = eantic::renf_class::make("x^2 - 2", "x", "1.4 +/- 1");
/// auto L = eantic::renf_class::make("x^2 - 2", "x", "1.41 +/- 1");
///
/// L->gen().parent() == *K
/// // -> true
///
/// &L->gen().parent() == &*K
/// // -> true
/// ```
///
/// Number fields are hashable but not ordered, i.e., they can be used in
/// containers such as `std::unordered_set` but not in `std::set`.
class LIBEANTIC_API renf_class : boost::equality_comparable<renf_class> {
    renf_class(const ::renf_t, const std::string &);

public:
    /// Return the trivial number field obtained by adjoining a root of $x - 1$ to the rationals.
    static const renf_class& make();

    /// Return the number field created from a [::renf_t](), the C type underlying a `renf_class`.
    static boost::intrusive_ptr<const renf_class> make(const ::renf_t, const std::string & gen = "a");

    /// Return the number field obtained by adjoining the root of [minpoly]()
    /// which is approximately [emb]().
    ///
    /// :param:`minpoly` An irreducible polynomial in the variable [gen]().
    /// This minimal polynomial does not have to be totally real or monic.
    /// ```
    /// #include <e-antic/renf_class.hpp>
    /// 
    /// auto K = eantic::renf_class::make("2*x^4 - 4", "x", "1 +/- 1");
    /// ```
    ///
    /// :param:`gen` The name of the variable used in [minpoly]().
    /// Note that two fields that differ only in the name of the generator are
    /// treated as being distinct fields.
    ///
    /// :param:`emb` An approximation of the root of [minpoly]().
    /// It makes no difference to what precision this approximation is provided
    /// but it must uniquely determine a single root of the polynomial.
    ///
    /// :param:`prec` The default precision for all arithmetic.
    /// When performing arithmetic in this field, all operations are performed
    /// to that precision. When necessary, the precision is dynamically
    /// increased. This only affects the performance and inner workings of
    /// libeantic, the results of arithmetic operations and comparisons are not
    /// affected by this.
    static boost::intrusive_ptr<const renf_class> make(const std::string & minpoly, const std::string & gen, const std::string &emb, slong prec = 64);

    ~renf_class() noexcept;

    /// Return the absolute degree of this number field.
    slong degree() const;

    /// Return the zero element of this number field.
    const renf_elem_class& zero() const;

    /// Return the one element of this number field.
    const renf_elem_class& one() const;

    /// Return the generator of this number field.
    /// ```
    /// #include <e-antic/renf_class.hpp>
    /// 
    /// auto& K = eantic::renf_class::make();
    /// std::cout << K.gen();
    /// // -> 1
    ///
    /// auto L = eantic::renf_class::make("x^2 - 2", "x", "1.41 +/- 1");
    /// std::cout << L->gen();
    /// // -> (x ~ 1.4142136)
    /// ```
    const renf_elem_class& gen() const;

    /// Return the parameters `(minpoly, gen, emb, prec)` that can be used to
    /// construct this field with [make](<> "make(minpoly, gen, emb, prec)").
    /// ```
    /// #include <e-antic/renf_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("2*x^4 - 4", "x", "1 +/- 1");
    /// 
    /// auto construction = K->construction();
    /// auto L = eantic::renf_class::make(
    ///   std::get<0>(construction),
    ///   std::get<1>(construction),
    ///   std::get<2>(construction),
    ///   std::get<3>(construction));
    /// K == L
    /// // -> true
    /// ```
    std::tuple<std::string, std::string, std::string, slong> construction() const;

    /// Return whether two number fields are indistinguishable.
    /// ```
    /// #include <e-antic/renf_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1 +/- 1");
    /// auto L = eantic::renf_class::make("x^2 - 2", "x", "1.41 +/- 0.1");
    /// K == L
    /// // -> true
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1 +/- 1");
    /// auto L = eantic::renf_class::make("2*x^2 - 4", "x", "1 +/- 1");
    /// K == L
    /// // -> false
    ///
    /// auto K = eantic::renf_class::make("x^2 - 2", "x", "1 +/- 1");
    /// auto L = eantic::renf_class::make("y^2 - 2", "y", "1 +/- 1");
    /// K == L
    /// // -> false
    ///
    /// K != L
    /// // -> true
    /// ```
    friend bool operator==(const renf_class &, const renf_class &);

    /// Return the name of the generator of this field.
    const std::string & gen_name() const { return name; }

    [[deprecated("Use cereal.hpp or the renf_elem_class constructor taking a string instead.")]]
    std::istream & set_pword(std::istream &) const;

    // Extract the number field stored with set_pword from an input stream.
    [[deprecated("Use cereal.hpp or the renf_elem_class constructor taking a string instead.")]]
    static boost::intrusive_ptr<const renf_class> get_pword(std::istream &);

    /// Return a human-readable representation of this field.
    /// ```
    /// #include <e-antic/renf_class.hpp>
    ///
    /// auto K = eantic::renf_class::make("2*x^4 - 4", "x", "1 +/- 1");
    /// K->to_string()
    /// // -> NumberField(2*x^4 - 4, [1.18920711500272106671749997056 +/- 1.11e-30])
    /// 
    /// // There is also an operator<< which gives the same output.
    /// std::cout << *K;
    /// // -> NumberField(2*x^4 - 4, [1.18920711500272106671749997056 +/- 1.11e-30])
    /// ```
    std::string to_string() const;

    /// Write a human-readable representation of this field to the output stream.
    friend std::ostream & operator<<(std::ostream &, const renf_class &);

    /// Return the underlying [renf_t]().
    /// We do not return a const `renf_t` since calls in the C API might need to
    /// modify it (e.g., to refine the stored embedding) even though they are
    /// morally const.
    std::add_lvalue_reference_t<::renf_t> renf_t() const { return nf; }

    [[deprecated("Use renf_t() instead.")]]
    renf * get_renf() const { return nf; }

    [[deprecated("Use set_pword() instead.")]]
    std::istream & set_istream(std::istream &) const;

private:
    // The name of the generator
    std::string name;

    // The actual underlying renf_t
    mutable ::renf_t nf;

    // The number of references to this number field for the intrusive pointer
    // reference counting.
    mutable std::atomic<size_t> refcount;

    // A cache of frequently used elements such as zero, one, and the
    // generator.
    renf_elem_class* cache;

    // Serialization, see [cereal.hpp]().
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
