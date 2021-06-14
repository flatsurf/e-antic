/// Embedded number field elements
/*
    Copyright (C) 2016 Vincent Delecroix
                  2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/


#ifndef E_ANTIC_RENF_ELEM_H
#define E_ANTIC_RENF_ELEM_H

#include "e-antic.h"

#include <flint/fmpq_poly.h>
#include <arb.h>
#include <arb_poly.h>
#include <antic/nf.h>
#include <antic/nf_elem.h>

#include "renf.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EANTIC_STR_ALG 1
#define EANTIC_STR_D   2
#define EANTIC_STR_ARB 4

// Patch nf_elem_scalar_div & nf_elem_add_fmpq which have issues in the released version of ANTIC, see upstream/patched.
LIBEANTIC_API void EANTIC_nf_elem_scalar_div_fmpq(nf_elem_t a, const nf_elem_t b, const fmpq_t c, const nf_t nf);
#define nf_elem_scalar_div_fmpq(a, b, c, nf) EANTIC_nf_elem_scalar_div_fmpq(a, b, c, nf)
LIBEANTIC_API void EANTIC_nf_elem_add_fmpq(nf_elem_t a, const nf_elem_t b, const fmpq_t c, const nf_t nf);
#define nf_elem_add_fmpq(a, b, c, nf) EANTIC_nf_elem_add_fmpq(a, b, c, nf)

/// === Memory Layout ===
/// A real embedded number field element
typedef struct LIBEANTIC_API renf_elem
{
    /// An algebraic ANTIC number field element.
    nf_elem_t elem;

    /// An approximation of the embedded element as a [real ball](<> "arb_t").
    arb_t emb;
} renf_elem;

typedef renf_elem* renf_elem_srcptr;

/// An element of a real embedded number field.
///
/// Actually, this is an array of [renf_elem]() of length one.
///
/// Typically, users of e-antic should not worry about the exact definition of
/// this and just treat this as *the* type that represents a number field
/// element in e-antic.
///
/// See the documentation of [renf_t]() for why this is an array.
typedef renf_elem renf_elem_t[1];

/// Initialize the number field element `a`.
/// This function has to be called prior to any code using `a` as it performs
/// allocation. Once done with `a` the memory must be freed with
/// [renf_elem_clear]().
LIBEANTIC_API void renf_elem_init(renf_elem_t a, const renf_t nf);

/// Deallocate the memory for `a` that was allocated with [renf_elem_init]().
LIBEANTIC_API void renf_elem_clear(renf_elem_t a, const renf_t nf);

/// Swap the number field elements `a` and `b`
static __inline__
void renf_elem_swap(renf_elem_t a, renf_elem_t b)
{
    renf_elem t = *a;
    *a = *b;
    *b = t;
}

/// === Setters ===
/// Set the number field element `a` to zero.
LIBEANTIC_API void renf_elem_zero(renf_elem_t a, const renf_t nf);

/// Set the number field element `a` to one.
LIBEANTIC_API void renf_elem_one(renf_elem_t a, const renf_t nf);

/// Set the number field element `a` to the generator of the number field.
LIBEANTIC_API void renf_elem_gen(renf_elem_t a, const renf_t nf);

/// ==* Integer/Rational Setters *==
/// Set `a` to provided integer/rational as an element in the number field `nf`.
LIBEANTIC_API void renf_elem_set_si(renf_elem_t a, slong n, const renf_t nf);
LIBEANTIC_API void renf_elem_set_ui(renf_elem_t a, ulong n, const renf_t nf);
LIBEANTIC_API void renf_elem_set_nf_elem(renf_elem_t a, const nf_elem_t b, renf_t nf, slong prec);
LIBEANTIC_API void renf_elem_set(renf_elem_t a, const renf_elem_t b, const renf_t nf);
LIBEANTIC_API void renf_elem_set_fmpz(renf_elem_t a, const fmpz_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_set_mpz(renf_elem_t a, const mpz_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_set_fmpq(renf_elem_t a, const fmpq_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_set_mpq(renf_elem_t a, const mpq_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_set_fmpq_poly(renf_elem_t a, const fmpq_poly_t pol, const renf_t nf);

/// === Embedding refinement ===
/// Set the enclosure of the number field element `a` using the enclosure
/// of the generator of `nf`.
/// Computation is done at precision `prec`. This function does not make any
/// refinement of the number field generator. For that purpose see
/// [renf_refine_embedding]().
LIBEANTIC_API void renf_elem_set_evaluation(renf_elem_t a, const renf_t nf, slong prec);

/// === Properties and conversion ===
/// Return 1 if `a` is equal to zero and 0 otherwise.
LIBEANTIC_API int renf_elem_is_zero(const renf_elem_t a, const renf_t nf);

/// Return 1 if `a` is equal to one and 0 otherwise.
LIBEANTIC_API int renf_elem_is_one(const renf_elem_t a, const renf_t nf);

/// Return 1 if `a` is integral and 0 otherwise.
LIBEANTIC_API int renf_elem_is_integer(const renf_elem_t a, const renf_t nf);

/// Return 1 if `a` is rational and 0 otherwise.
LIBEANTIC_API int renf_elem_is_rational(const renf_elem_t a, const renf_t nf);

/// Return the sign of `a`. It is `1` if `a` is positive, `0` if `a` is
/// zero and `-1` if `a` is negative.
LIBEANTIC_API int renf_elem_sgn(renf_elem_t a, renf_t nf);

/// Set `a` to be the floor of `b`
LIBEANTIC_API void renf_elem_floor(fmpz_t a, renf_elem_t b, renf_t nf);

/// Set `a` to be the ceil of `b`
LIBEANTIC_API void renf_elem_ceil(fmpz_t a, renf_elem_t b, renf_t nf);

/// === Floating point approximations ===
/// Set `x` to a a real ball enclosing the element `a` that belongs to the number field `nf`
/// with `prec` bits of precision.
LIBEANTIC_API void renf_elem_get_arb(arb_t x, renf_elem_t a, renf_t nf, slong prec);

/// Return a double approximation.
LIBEANTIC_API double renf_elem_get_d(renf_elem_t a, renf_t nf, arf_rnd_t rnd);

/// ==* Comparisons *==
/// All the comparison functions `renf_elem_cmp_…` between two elements `a` and
/// `b` behave as follows. They return:
/// * `1` if `a` is greater than `b`
/// * `0` if `a` and `b` are equal
/// * `-1` if `a` is smaller than `b`
///
/// If you want to check for equality, use the faster `renf_elem_equal_…` functions.
LIBEANTIC_API int renf_elem_cmp(renf_elem_t a, renf_elem_t b, renf_t nf);
LIBEANTIC_API int _renf_elem_cmp_fmpq(renf_elem_t a, fmpz * num, fmpz * den, renf_t nf);
LIBEANTIC_API int renf_elem_cmp_fmpq(renf_elem_t a, const fmpq_t b, renf_t nf);
LIBEANTIC_API int renf_elem_cmp_fmpz(renf_elem_t a, const fmpz_t b, renf_t nf);
LIBEANTIC_API int renf_elem_cmp_si(renf_elem_t a, const slong b, renf_t nf);
LIBEANTIC_API int renf_elem_cmp_ui(renf_elem_t a, const ulong b, renf_t nf);
LIBEANTIC_API int renf_elem_equal(const renf_elem_t a, const renf_elem_t b, const renf_t nf);
LIBEANTIC_API int renf_elem_equal_si(const renf_elem_t a, const slong b, const renf_t nf);
LIBEANTIC_API int renf_elem_equal_ui(const renf_elem_t a, const ulong b, const renf_t nf);
LIBEANTIC_API int renf_elem_equal_fmpz(const renf_elem_t a, const fmpz_t b, const renf_t nf);
LIBEANTIC_API int renf_elem_equal_fmpq(const renf_elem_t a, const fmpq_t b, const renf_t nf);

/// === String Conversion and Printing ===
/// Return `a` as a string with variable name `var`.
/// The resulting string needs to be freed with `flint_free`.
LIBEANTIC_API char * renf_elem_get_str_pretty(renf_elem_t a, const char * var, renf_t nf, slong n, int flag);

/// Write `a` to the standard output.
LIBEANTIC_API void renf_elem_print_pretty(renf_elem_t a, const char * var, renf_t nf, slong n, int flag);

/// === Randomization ===
/// Set `a` to a random element in `nf`.
LIBEANTIC_API void renf_elem_randtest(renf_elem_t a, flint_rand_t state, mp_bitcnt_t bits, renf_t nf);

/// === Unary operations ===
/// Set `a` to the negative of `b`.
LIBEANTIC_API void renf_elem_neg(renf_elem_t a, const renf_elem_t b, const renf_t nf);

/// Set `a` to the inverse of `b`.
LIBEANTIC_API void renf_elem_inv(renf_elem_t a, const renf_elem_t b, const renf_t nf);

/// === Binary operations ===
/// All the binary operation functions are of the form `renf_elem_OP_TYP(a, b, c, nf)` where
/// * `OP` is the operation type
/// * `TYP` is the type of the argument `c`
/// * `a` is the argument used for returned value
/// * `b` and `c` are the operands
/// * `nf` is the parent number field of the operation
/// In short, these functions perform `a = b OP c`.
LIBEANTIC_API void renf_elem_add_si(renf_elem_t a, const renf_elem_t b, slong c, const renf_t nf);
LIBEANTIC_API void renf_elem_sub_si(renf_elem_t a, const renf_elem_t b, slong c, const renf_t nf);
LIBEANTIC_API void renf_elem_mul_si(renf_elem_t a, const renf_elem_t b, slong c, const renf_t nf);
LIBEANTIC_API void renf_elem_div_si(renf_elem_t a, const renf_elem_t b, slong c, const renf_t nf);
LIBEANTIC_API void renf_elem_add_ui(renf_elem_t a, const renf_elem_t b, ulong c, const renf_t nf);
LIBEANTIC_API void renf_elem_sub_ui(renf_elem_t a, const renf_elem_t b, ulong c, const renf_t nf);
LIBEANTIC_API void renf_elem_mul_ui(renf_elem_t a, const renf_elem_t b, ulong c, const renf_t nf);
LIBEANTIC_API void renf_elem_div_ui(renf_elem_t a, const renf_elem_t b, ulong c, const renf_t nf);
LIBEANTIC_API void renf_elem_add_fmpz(renf_elem_t a, const renf_elem_t b, const fmpz_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_sub_fmpz(renf_elem_t a, const renf_elem_t b, const fmpz_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_mul_fmpz(renf_elem_t a, const renf_elem_t b, const fmpz_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_div_fmpz(renf_elem_t a, const renf_elem_t b, const fmpz_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_add_fmpq(renf_elem_t a, const renf_elem_t b, const fmpq_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_sub_fmpq(renf_elem_t a, const renf_elem_t b, const fmpq_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_fmpq_sub(renf_elem_t a, const fmpq_t c, const renf_elem_t b, const renf_t nf);
LIBEANTIC_API void renf_elem_mul_fmpq(renf_elem_t a, const renf_elem_t b, const fmpq_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_div_fmpq(renf_elem_t a, const renf_elem_t b, const fmpq_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_fmpq_div(renf_elem_t a, const fmpq_t b, const renf_elem_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_add(renf_elem_t a, const renf_elem_t b, const renf_elem_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_sub(renf_elem_t a, const renf_elem_t b, const renf_elem_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_mul(renf_elem_t a, const renf_elem_t b, const renf_elem_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_div(renf_elem_t a, const renf_elem_t b, const renf_elem_t c, const renf_t nf);
LIBEANTIC_API void renf_elem_pow(renf_elem_t res, const renf_elem_t a, ulong e, const renf_t nf);

/// Perform the floor division of the number field elements `b` and `c` and set the
/// result in `a`.
/// The result is equivalent to a call of [renf_elem_div] followed by
/// [renf_elem_floor]. This function is much faster, though.
LIBEANTIC_API void renf_elem_fdiv(fmpz_t a, renf_elem_t b, renf_elem_t c, renf_t nf);

/// === Other functions ===
/// Set the array `c` to the `n`-th first partial quotients of the continued
/// fraction of the element `a` of `nf`.
LIBEANTIC_API slong renf_elem_get_cfrac(fmpz * c, renf_elem_t rem, renf_elem_t a, slong n, renf_t nf);

/// Check that the embedding is consistent.
LIBEANTIC_API void renf_elem_check_embedding(const renf_elem_t a, const renf_t nf, slong prec);

/// Logarithm of the condition number of `a`.
/// The return value corresponds to the number of bits of precision that will
/// be lost when evaluating `a` as a polynomial in the number field generator.
LIBEANTIC_API int renf_elem_relative_condition_number_2exp(slong * cond, renf_elem_t a, renf_t nf);


#ifdef __cplusplus
}
#endif

#endif
