/*
   Copyright (C) 2016 Vincent Delecroix
                 2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_FMPZ_POLY_EXTRA_H
#define E_ANTIC_FMPZ_POLY_EXTRA_H

#include "e-antic.h"

#include <flint/fmpz_poly.h>
#include <arb.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************/
/* Real root isolation for integer polynomials (to be moved to FLINT) */

LIBEANTIC_API void _fmpz_poly_scale_0_1_fmpq(fmpz * pol, slong len, fmpq_t a, fmpq_t b);
LIBEANTIC_API void fmpz_poly_randtest_irreducible(fmpz_poly_t p, flint_rand_t state, slong len, mp_bitcnt_t bits);

LIBEANTIC_API int fmpz_poly_set_str_pretty(fmpz_poly_t p, const char * s, const char * var);

LIBEANTIC_API int _fmpz_poly_has_real_root(fmpz * pol, slong len);
LIBEANTIC_API int fmpz_poly_has_real_root(fmpz_poly_t pol);

// Gives an upper bound on the bitsize of largest real root of \code{pol}. 
LIBEANTIC_API slong fmpz_poly_positive_root_upper_bound_2exp(const fmpz_poly_t pol);

// Gives an upper bound on the bitsize of largest real root of \code{(pol, len)}.
LIBEANTIC_API slong _fmpz_poly_positive_root_upper_bound_2exp(const fmpz * pol, slong len);

LIBEANTIC_API slong _fmpz_poly_positive_root_upper_bound_2exp_local_max(const fmpz * pol, slong len);

// Gives an upper bound on the number of real roots of the polynomial
// \code{pol} (currently using Decartes rule of sign).
LIBEANTIC_API slong fmpz_poly_num_real_roots_upper_bound(fmpz_poly_t pol);

// Gives an upper bound on the number of real roots between 0 and 1
// of the polynomial \code{(p, len)} using Decartes rule of sign. If
// the result is larger than \code{bound} then \code{WORD_MAX} is
// returned.
LIBEANTIC_API slong _fmpz_poly_descartes_bound_0_1(fmpz * p, slong len, slong bound);

LIBEANTIC_API slong _fmpz_poly_descartes_bound(fmpz * p, slong len, slong bound);

// Isolate the real roots of \code{(pol, len)} contained in the
// interval $(0, 1)$. The array \code{exact_roots} will be set by
// the exact diadic roots found by the algorithm and
// \code{n_exact_roots} updated accordingly. The arrays
// \code{c_array} and \code{k_array} are set to be interval data
// that enclose the remaining roots and \code{n_interval} is
// updated accordingly. A data \code{c = c_array + i} and
// \code{k = k_array[i]} represents the open interval
// $(c 2^k, (c + 1) 2^k)$.
LIBEANTIC_API void _fmpz_poly_isolate_real_roots_0_1_vca(fmpq * exact_roots, slong * n_exact_roots, fmpz * c_array, slong * k_array, slong * n_intervals, fmpz * pol, slong len);

// Isolate the real roots of \code{pol}. The array
// \code{exact_roots} will be set by the exact diadic roots found
// by the algorithm and \code{n_exact_roots} updated accordingly.
// The arrays \code{c_array} and \code{k_array} are set to be
// interval data that enclose the remaining roots and
// \code{n_interval} is updated accordingly. A data
// \code{c = c_array + i} and \code{k = k_array[i]} represents the
// open interval $(c 2^k, (c + 1) 2^k)$.
LIBEANTIC_API void fmpz_poly_isolate_real_roots(fmpq * exact_roots, slong * n_exact, fmpz * c_array, slong * k_array, slong * n_interval, fmpz_poly_t pol);

LIBEANTIC_API slong fmpz_poly_num_real_roots_vca(fmpz_poly_t pol);

LIBEANTIC_API slong fmpz_poly_num_real_roots_0_1_sturm(fmpz_poly_t pol);
LIBEANTIC_API slong fmpz_poly_num_real_roots_0_1_vca(fmpz_poly_t pol);
LIBEANTIC_API slong fmpz_poly_num_real_roots_0_1(fmpz_poly_t pol);

LIBEANTIC_API void fmpz_poly_abs(fmpz_poly_t res, fmpz_poly_t p);

/****************************************************************************/
/* FLINT/ARB extra                                                          */

LIBEANTIC_API void _fmpz_poly_evaluate_arb(arb_t res, const fmpz * pol, slong len, const arb_t a, slong prec);

LIBEANTIC_API void fmpz_poly_evaluate_arb(arb_t b, const fmpz_poly_t pol, const arb_t a, slong prec);

LIBEANTIC_API void _fmpz_poly_evaluate_arf(arf_t res, const fmpz * pol, slong len, const arf_t a, slong prec);

LIBEANTIC_API void fmpz_poly_evaluate_arf(arf_t res, const fmpz_poly_t pol, const arf_t a, slong prec);

/* condition number of a_0 + a_1 x + ... + a_d x^d at a number t is: */
/*  |a_0| + |a_1 t| + ... + |a_d t^d|                                */
LIBEANTIC_API int _fmpz_poly_relative_condition_number_2exp(slong * cond, fmpz * p, slong len, arb_t x, slong prec);
LIBEANTIC_API int fmpz_poly_relative_condition_number_2exp(slong * cond, fmpz_poly_t p, arb_t x, slong prec);

/* root refinement */
LIBEANTIC_API int _fmpz_poly_newton_step_arb(arb_t res, const fmpz * pol, const fmpz * der, slong len, arb_t a, slong prec);

LIBEANTIC_API int _fmpz_poly_bisection_step_arb(arb_t res, fmpz * pol, slong len, arb_t a, slong prec);

LIBEANTIC_API int fmpz_poly_newton_step_arb(arb_t res, const fmpz_poly_t pol, const fmpz_poly_t der, arb_t a, slong prec);

LIBEANTIC_API void _fmpz_poly_bisection_step_arf(arf_t l, arf_t r, const fmpz * pol, slong len, int sl, int sr, slong prec);

LIBEANTIC_API int fmpz_poly_bisection_step_arb(arb_t res, const fmpz_poly_t pol, arb_t a, slong prec);

LIBEANTIC_API void fmpz_poly_squarefree_part(fmpz_poly_t res, fmpz_poly_t poly);


LIBEANTIC_API void fmpz_poly_evaluate_at_one(fmpz_t res, fmpz * p, slong len);

#ifdef __cplusplus
}
#endif

#endif
