/// Real Embedded Number Fields
///
/// This header defines one of the basic objects of e-antic, [renf_t]() a real
/// embedded number field and the functions that interact with it.
/*
    Copyright (C)        2016 Vincent Delecroix
                  2020 - 2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/


#ifndef E_ANTIC_RENF_H
#define E_ANTIC_RENF_H

#include "e-antic.h"

#include <flint/fmpq_poly.h>
#include <antic/nf.h>
#include <antic/nf_elem.h>
#include <arb.h>
#include <arb_poly.h>

#ifdef __cplusplus
extern "C" {
#endif

/// === Memory Layout ===
/// Internal representation of a real embedded number field.
///
/// The attributes are not part of the API and should be accessed directly. The
/// exact structure here is only of interest for those who want to work on
/// e-antic itself. To interact with a real embedded number field, use the
/// `renf_` functions described below.
///
/// Typical users of e-antic will never instantiate a `renf`
/// directly but instead work with a [renf_t]().
typedef struct LIBEANTIC_API renf
{
  /// An algebraic number field, provided by ANTIC.
  nf_t nf;

  /// The derivative of the defining polynomial (without denominator) as a
  /// FLINT polynomial over the integers.
  fmpz_poly_t der;

  /// Precision (number of bits) used for binary operations on enclosures.
  slong prec;

  /// The real embeddeding of the generator of this number field, given as ball
  /// in the reals.
  arb_t emb;

  /// A flag used during refinements to ensure thread-safety.
  int immutable;
} renf;

typedef renf * renf_ptr;
typedef const renf * renf_srcptr;

/// A real embedded number field.
///
/// Actually, this is an array of [renf]() of length one.
///
/// Typically, users of e-antic should not worry about the exact definition of
/// this and just treat this as *the* type that represents a number field in
/// e-antic.
///
/// Using an array of length one instead of a plain `renf` or a `renf*` is a
/// common trick in C which is probably best known from MPFR and GMP but also
/// used in the FLINT and Arb libraries that we build upon.
/// Essentially, this gives much better semantics than a plain `renf`. E.g., a
/// `renf_t` is not assignable. (Directly assigning a `renf` to another `renf`
/// does not do what one would expect as the heap-allocated parts of a `renf`
/// are not duplicated correctly.) Also, calling a function that accepts a
/// `renf_t` passes the value with reference semantics, as if it were a
/// `renf*`, i.e., without copying the underlying value. Since a `renf_t` is
/// always stack allocated and cannot be returned from a function, it is also
/// much harder to produce dangling pointers which one would get when using
/// `renf*` everywhere instead.
typedef renf renf_t[1];

/// === Initialization, Allocation, Deallocation ===
/// Set `nf` to be the real embedded number field define by the minimal
/// polynomial `pol` and the (approximate) embedding `emb`. The parameter
/// `prec` is used as the default precision for binary operations on
/// enclosures.
LIBEANTIC_API void renf_init(renf_t nf, fmpq_poly_t pol, arb_t emb, slong prec);

/// Set `nf` to be the real embedded number field defined as the `n`-th root of
/// the rational number `d`. `prec` is the default precision.
LIBEANTIC_API void renf_init_nth_root_fmpq(renf_t nf, fmpq_t d, ulong n, slong prec);

/// Copy `src` to `dest`.
LIBEANTIC_API void renf_init_set(renf_t dest, const renf_t src);

/// Deallocate the number field `nf` originally allocated with one of the
/// `renf_init` above.
LIBEANTIC_API void renf_clear(renf_t nf);

/// Set the `lim` first elements of the array `nf` to the real embeddings of
/// the irreducible polynomial `pol`. Each number field is initialized with a
/// default `prec` of bit precision for binary operations.
LIBEANTIC_API slong renf_set_embeddings_fmpz_poly(renf * nf, fmpz_poly_t pol, slong lim, slong prec);

/// === Other Functions ===
/// Refine the embedding of `nf` to `prec` bits of precision.
LIBEANTIC_API void renf_refine_embedding(renf_t nf, slong prec);

/// Change the mutability of the number field.
/// If `immutable` is zero, make the number field mutable.
/// Otherwise, make the number field immutable.
/// Returns the previous value.
/// It is not possible to [refine](<> "renf_refine_embedding") the embedding of
/// an immutable number field.
/// This method is used internally as a sanity-check in multi-threaded code.
LIBEANTIC_API int renf_set_immutable(renf_t nf, int immutable);

/// Return whether the two number fields are equal.
/// Two embedded number field are considered equal if they define the same
/// subfield of the real numbers.  The precision of the embedding or the
/// precision of bit operations do not matter in this comparison.
LIBEANTIC_API int renf_equal(const renf_t nf1, const renf_t nf2);

/// Return the degree of the number field.
LIBEANTIC_API slong renf_degree(renf_t nf);

/// Set `nf` to a random number field.
/// Used internally in unit tests.
LIBEANTIC_API void renf_randtest(renf_t nf, flint_rand_t state, slong len, slong prec, mp_bitcnt_t bits);

/// Return a printable representation of this number field.
/// Returns a C string which uses `var` as the name of the variable and the
/// precision is given with `prec` bits.  The returned string has to be
/// deallocated with `flint_free` after usage.
LIBEANTIC_API char * renf_get_str(const renf_t nf, const char * var, slong prec);

/// Print the number field on the standard output.
LIBEANTIC_API void renf_print(const renf_t nf);

#ifdef __cplusplus
}
#endif

#endif
