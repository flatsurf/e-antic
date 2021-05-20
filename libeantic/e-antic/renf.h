/// Real embedded number fields
/*
    Copyright (C)        2016 Vincent Delecroix
                  2020 - 2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
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
/// Data structure for real embedded number field.
/// The attributes are not part of the API and should be accessed via
/// the provided methods. The datastructure contains an immutability
/// flag that, when set, forbid any refinement of the floating point
/// embedding.
typedef struct LIBEANTIC_API renf_struct
{
  nf_t nf;
  fmpz_poly_t der;   //< derivative (without denominator)
  slong prec;        //< precision used for binary operations on enclosures
  arb_t emb;         //< embedding of generator as a real ball
  int immutable;     //< whether emb cannot be refined currently
} renf;

/// === Type Aliases ===
/// Pointer to a renf structure
typedef renf * renf_ptr;

/// Constant pointer to a renf structure
typedef const renf * renf_srcptr;

/// Array of size one (TODO)
typedef renf renf_t[1];

/// === Initialization, allocation, deallocation ===
/// Set `nf` to be the real embedded number field define by the minimal polynomial `pol`
/// and the (approximate) embedding `emb`. The parameter `prec` is used as the default
/// precision used for binary operations on enclosures.
LIBEANTIC_API void renf_init(renf_t nf, fmpq_poly_t pol, arb_t emb, slong prec);

/// Set `nf` to be the real embedded number field define as the `n`-th root of the rational
/// number `d`. `prec` is the default precision.
LIBEANTIC_API void renf_init_nth_root_fmpq(renf_t nf, fmpq_t d, ulong n, slong prec);

/// Set `dest` to `src`
LIBEANTIC_API void renf_init_set(renf_t dest, const renf_t src);

/// Deallocate `nf`.
LIBEANTIC_API void renf_clear(renf_t nf);

/// Set the `lim` first elements of the array `nf` to the real embeddings
/// of the irreducible polynomial `pol`. Each number field is initialized
/// with a default `prec` of bit precision for binary operation.
LIBEANTIC_API slong renf_set_embeddings_fmpz_poly(renf * nf, fmpz_poly_t pol, slong lim, slong prec);

/// Refine the embedding to `prec` bits of precision.
LIBEANTIC_API void renf_refine_embedding(renf_t nf, slong prec);

/// Make the number field immutable. Once called, it is not possible to refine
/// further the embedding.
LIBEANTIC_API int renf_set_immutable(renf_t nf, int immutable);

/// Return whether `nf1` and `nf2` are equal. Two embedded number field are
/// considered equal if they define the same subfield of the real numbers.
/// The precision of the embedding or the precision of bit operations do not
/// matter in this comparison.
LIBEANTIC_API int renf_equal(const renf_t nf1, const renf_t nf2);

/// Return the degree of the number field.
LIBEANTIC_API slong renf_degree(renf_t nf);

/// === Randomisation ===
LIBEANTIC_API void renf_randtest(renf_t nf, flint_rand_t state, slong len, slong prec, mp_bitcnt_t bits);

/// === Printing ===
/// Return a C string which represent the number field `nf` where `var` is used for variable
/// name of the polynomial and `prec` for the precision of the embedding
/// The returned string has to be deallocated with flint_free after usage.
LIBEANTIC_API char * renf_get_str(const renf_t nf, const char * var, slong prec);

/// Print the number field `nf` on stdout
LIBEANTIC_API void renf_print(const renf_t nf);

#ifdef __cplusplus
}
#endif

#endif
