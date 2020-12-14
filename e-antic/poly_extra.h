/*
   Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_POLY_EXTRA_H
#define E_ANTIC_POLY_EXTRA_H

#include <e-antic/e-antic.h>

#include <flint/fmpz_poly.h>
#include <flint/fmpq_poly.h>
#include <arb.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************/
/* Real root isolation for integer polynomials (to be moved to FLINT) */

void _fmpz_poly_scale_0_1_fmpq(fmpz * pol, slong len, fmpq_t a, fmpq_t b);
void fmpz_poly_randtest_irreducible(fmpz_poly_t p, flint_rand_t state, slong len, mp_bitcnt_t bits);

int fmpz_poly_set_str_pretty(fmpz_poly_t p, const char * s, const char * var);
int fmpq_poly_set_str_pretty(fmpq_poly_t p, const char * s, const char * var);

int _fmpz_poly_has_real_root(fmpz * pol, slong len);

int fmpz_poly_has_complex_root(fmpz_poly_t pol);

int fmpz_poly_has_real_root(fmpz_poly_t pol);

slong fmpz_poly_positive_root_upper_bound_2exp(const fmpz_poly_t pol);
slong _fmpz_poly_positive_root_upper_bound_2exp(const fmpz * pol, slong len);
slong _fmpz_poly_positive_root_upper_bound_2exp_local_max(const fmpz * pol, slong len);

slong fmpz_poly_num_real_roots_upper_bound(fmpz_poly_t pol);
slong _fmpz_poly_descartes_bound_0_1(fmpz * p, slong len, slong bound);
slong _fmpz_poly_descartes_bound(fmpz * p, slong len, slong bound);

void _fmpz_poly_isolate_real_roots_0_1_vca(fmpq * exact_roots, slong * n_exact_roots, fmpz * c_array, slong * k_array, slong * n_intervals, fmpz * pol, slong len);

void fmpz_poly_isolate_real_roots(fmpq * exact_roots, slong * n_exact, fmpz * c_array, slong * k_array, slong * n_interval, fmpz_poly_t pol);

slong fmpz_poly_num_real_roots_vca(fmpz_poly_t pol);

slong fmpz_poly_num_real_roots_0_1_sturm(fmpz_poly_t pol);
slong fmpz_poly_num_real_roots_0_1_vca(fmpz_poly_t pol);
slong fmpz_poly_num_real_roots_0_1(fmpz_poly_t pol);

void fmpz_poly_abs(fmpz_poly_t res, fmpz_poly_t p);

/****************************************************************************/
/* FLINT/ARB extra                                                          */

void _fmpz_poly_evaluate_arb(arb_t res, const fmpz * pol, slong len, const arb_t a, slong prec);

void fmpz_poly_evaluate_arb(arb_t b, const fmpz_poly_t pol, const arb_t a, slong prec);

void fmpq_poly_evaluate_arb(arb_t b, const fmpq_poly_t pol, const arb_t a, slong prec);

void _fmpz_poly_evaluate_arf(arf_t res, const fmpz * pol, slong len, const arf_t a, slong prec);

void fmpz_poly_evaluate_arf(arf_t res, const fmpz_poly_t pol, const arf_t a, slong prec);

void fmpq_poly_evaluate_arf(arf_t b, const fmpq_poly_t pol, const arf_t a, slong prec);

/* condition number of a_0 + a_1 x + ... + a_d x^d at a number t is: */
/*  |a_0| + |a_1 t| + ... + |a_d t^d|                                */
int _fmpz_poly_relative_condition_number_2exp(slong * cond, fmpz * p, slong len, arb_t x, slong prec);
int fmpz_poly_relative_condition_number_2exp(slong * cond, fmpz_poly_t p, arb_t x, slong prec);

/* root refinement */
int fmpq_poly_check_unique_real_root(const fmpq_poly_t pol, const arb_t a, slong prec);

int _fmpz_poly_newton_step_arb(arb_t res, const fmpz * pol, const fmpz * der, slong len, arb_t a, slong prec);

int _fmpz_poly_bisection_step_arb(arb_t res, fmpz * pol, slong len, arb_t a, slong prec);

int fmpz_poly_newton_step_arb(arb_t res, const fmpz_poly_t pol, const fmpz_poly_t der, arb_t a, slong prec);

void _fmpz_poly_bisection_step_arf(arf_t l, arf_t r, const fmpz * pol, slong len, int sl, int sr, slong prec);

int fmpz_poly_bisection_step_arb(arb_t res, const fmpz_poly_t pol, arb_t a, slong prec);

void fmpz_poly_squarefree_part(fmpz_poly_t res, fmpz_poly_t poly);


void fmpz_poly_evaluate_at_one(fmpz_t res, fmpz * p, slong len);

#ifdef __cplusplus
}
#endif

#endif
