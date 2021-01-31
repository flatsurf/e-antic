/// Embedded number field elements
/*
    Copyright (C) 2016 Vincent Delecroix
                  2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
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

/// === Datastructures ===

/// A real embedded number field element
typedef struct LIBEANTIC_API
{
    nf_elem_t elem;    /* ANTIC number field element */
    arb_t emb;         /* the embedded real ball    */
} renf_elem_struct;

typedef renf_elem_struct * renf_elem_srcptr;
typedef renf_elem_struct renf_elem_t[1];

/// === Initialization, allocation, deallocation ===

LIBEANTIC_API void renf_elem_init(renf_elem_t a, const renf_t nf);

LIBEANTIC_API void renf_elem_clear(renf_elem_t a, const renf_t nf);

LIBEANTIC_API void renf_elem_set_fmpq_poly(renf_elem_t a, const fmpq_poly_t pol, const renf_t nf);

LIBEANTIC_API void renf_elem_gen(renf_elem_t a, const renf_t nf);

static __inline__
void renf_elem_swap(renf_elem_t a, renf_elem_t b)
{
    renf_elem_struct t = *a;
    *a = *b;
    *b = t;
}

LIBEANTIC_API void renf_elem_check_embedding(const renf_elem_t a, const renf_t nf, slong prec);

LIBEANTIC_API void renf_elem_zero(renf_elem_t a, const renf_t nf);

LIBEANTIC_API void renf_elem_one(renf_elem_t a, const renf_t nf);

LIBEANTIC_API void renf_elem_set_si(renf_elem_t a, slong n, const renf_t nf);

LIBEANTIC_API void renf_elem_set_ui(renf_elem_t a, ulong n, const renf_t nf);

LIBEANTIC_API void renf_elem_set_nf_elem(renf_elem_t a, const nf_elem_t b, renf_t nf, slong prec);

LIBEANTIC_API void renf_elem_set(renf_elem_t a, const renf_elem_t b, const renf_t nf);

LIBEANTIC_API void renf_elem_set_fmpz(renf_elem_t a, const fmpz_t c, const renf_t nf);

LIBEANTIC_API void renf_elem_set_mpz(renf_elem_t a, const mpz_t c, const renf_t nf);

LIBEANTIC_API void renf_elem_set_fmpq(renf_elem_t a, const fmpq_t c, const renf_t nf);

LIBEANTIC_API void renf_elem_set_mpq(renf_elem_t a, const mpq_t c, const renf_t nf);

/// === Embedding refinement ===

LIBEANTIC_API void renf_elem_set_evaluation(renf_elem_t a, const renf_t nf, slong prec);

/// === Properties and conversion ===

LIBEANTIC_API int renf_elem_is_zero(const renf_elem_t a, const renf_t nf);

LIBEANTIC_API int renf_elem_is_one(const renf_elem_t a, const renf_t nf);

LIBEANTIC_API int renf_elem_is_integer(const renf_elem_t a, const renf_t nf);

LIBEANTIC_API int renf_elem_is_rational(const renf_elem_t a, const renf_t nf);

LIBEANTIC_API int renf_elem_sgn(renf_elem_t a, renf_t nf);

LIBEANTIC_API void renf_elem_floor(fmpz_t a, renf_elem_t b, renf_t nf);

LIBEANTIC_API void renf_elem_ceil(fmpz_t a, renf_elem_t b, renf_t nf);

LIBEANTIC_API int renf_elem_relative_condition_number_2exp(slong * cond, renf_elem_t a, renf_t nf);

/// === Floating point approximations ===

/// Set `x` to a a real ball enclosing the element `a` that belongs to the number field `nf`
/// with `prec` bits of precision.
LIBEANTIC_API void renf_elem_get_arb(arb_t x, renf_elem_t a, renf_t nf, slong prec);

/// Retourn a double approximation
LIBEANTIC_API double renf_elem_get_d(renf_elem_t a, renf_t nf, arf_rnd_t rnd);

/// === Comparisons ===

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

/// === String and printing ===

LIBEANTIC_API char * renf_elem_get_str_pretty(renf_elem_t a, const char * var, renf_t nf, slong n, int flag);

LIBEANTIC_API void renf_elem_print_pretty(renf_elem_t a, const char * var, renf_t nf, slong n, int flag);

/// === Random ===

LIBEANTIC_API void renf_elem_randtest(renf_elem_t a, flint_rand_t state, mp_bitcnt_t bits, renf_t nf);

///=== Unary and binary operations ===

/// Sets `a` to the negative of `b`
LIBEANTIC_API void renf_elem_neg(renf_elem_t a, const renf_elem_t b, const renf_t nf);

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

/// Sets `a` to the inverse of `b`
LIBEANTIC_API void renf_elem_inv(renf_elem_t a, const renf_elem_t b, const renf_t nf);

LIBEANTIC_API void renf_elem_div_fmpq(renf_elem_t a, const renf_elem_t b, const fmpq_t c, const renf_t nf);

LIBEANTIC_API void renf_elem_fmpq_div(renf_elem_t a, const fmpq_t b, const renf_elem_t c, const renf_t nf);

LIBEANTIC_API void renf_elem_add(renf_elem_t a, const renf_elem_t b, const renf_elem_t c, const renf_t nf);

LIBEANTIC_API void renf_elem_sub(renf_elem_t a, const renf_elem_t b, const renf_elem_t c, const renf_t nf);

LIBEANTIC_API void renf_elem_mul(renf_elem_t a, const renf_elem_t b, const renf_elem_t c, const renf_t nf);

LIBEANTIC_API void renf_elem_div(renf_elem_t a, const renf_elem_t b, const renf_elem_t c, const renf_t nf);

LIBEANTIC_API void renf_elem_fdiv(fmpz_t a, renf_elem_t b, renf_elem_t c, renf_t nf);

LIBEANTIC_API void renf_elem_pow(renf_elem_t res, const renf_elem_t a, ulong e, const renf_t nf);

/// === Other functions ===

/// Set the array `c` to the `n`-th first partial quotients of the continued
/// fraction of the element `a` of `nf`.
LIBEANTIC_API slong renf_elem_get_cfrac(fmpz * c, renf_elem_t rem, renf_elem_t a, slong n, renf_t nf);

#ifdef __cplusplus
}
#endif

#endif
