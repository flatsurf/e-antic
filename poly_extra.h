/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#ifndef FMPQ_POLY_EXTRA_H
#define FMPQ_POLY_EXTRA_H

#include "arb.h"
#include "acb.h"
#include "fmpz_poly.h"
#include "fmpq_poly.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* extra vector functions */

void _fmpq_vec_sort(fmpq * vec, slong len);

void _fmpq_vec_randtest_uniq_sorted(fmpq * vec, flint_rand_t state, slong len, mp_bitcnt_t bits);

int _fmpq_vec_fprint(FILE * file, const fmpq * vec, slong len);

void _fmpq_vec_print(const fmpq * vec, slong len);

/* extra polynomial functions */

void fmpz_poly_set_rational_roots(fmpz_poly_t pol, fmpq * vec, slong len);

void fmpz_poly_randtest_no_real_root(fmpz_poly_t p, flint_rand_t state, slong len, mp_bitcnt_t bits);

void _fmpz_poly_scale_2exp(fmpz * pol, slong len, slong k);

/* polynomial evaluation */

void _fmpz_poly_evaluate_arb(arb_t res, const fmpz * pol, slong len, const arb_t a, slong prec);

void fmpz_poly_evaluate_arb(arb_t b, const fmpz_poly_t pol, const arb_t a, slong prec);

void fmpq_poly_evaluate_arb(arb_t b, const fmpq_poly_t pol, const arb_t a, slong prec);

void _fmpz_poly_evaluate_acb(acb_t b, const fmpz * pol, slong len, const acb_t a, slong prec);

void fmpz_poly_evaluate_acb(acb_t b, const fmpz_poly_t pol, const acb_t a, slong prec);

void fmpq_poly_evaluate_acb(acb_t b, const fmpq_poly_t pol, const acb_t a, slong prec);

void _fmpz_poly_evaluate_arf(arf_t res, const fmpz * pol, slong len, const arf_t a, slong prec);

void fmpz_poly_evaluate_arf(arf_t res, const fmpz_poly_t pol, const arf_t a, slong prec);

void fmpq_poly_evaluate_arf(arf_t b, const fmpq_poly_t pol, const arf_t a, slong prec);

/* root refinement */
int fmpq_poly_check_unique_real_root(const fmpq_poly_t pol, const arb_t a, slong prec);

int fmpq_poly_check_unique_complex_root(const fmpq_poly_t pol, const acb_t a, slong prec);

int _fmpz_poly_newton_step_arb(arb_t res, const fmpz * pol, const fmpz * der, slong len, arb_t a, slong prec);

int fmpz_poly_newton_step_arb(arb_t res, const fmpz_poly_t pol, const fmpz_poly_t der, arb_t a, slong prec);

int _fmpz_poly_newton_step_acb(acb_t res, const fmpz * pol, const fmpz * der, slong len, acb_t a, slong prec);

int fmpz_poly_newton_step_acb(acb_t res, const fmpz_poly_t pol, const fmpz_poly_t der, acb_t a, slong prec);

void _fmpz_poly_bisection_step_arb(arb_t res, const fmpz * pol, slong len, arb_t a, slong prec);

int fmpz_poly_bisection_step_arb(arb_t res, const fmpz_poly_t pol, arb_t a, slong prec);

/* root isolation */

slong fmpz_poly_positive_root_upper_bound_2exp(fmpz_poly_t pol);

slong _fmpz_poly_positive_root_upper_bound_2exp(fmpz * pol, slong len);

slong _fmpz_poly_descartes_bound_0_1(fmpz * p, slong len, slong bound);

slong fmpz_poly_num_real_roots_upper_bound(fmpz_poly_t pol);

void _fmpz_poly_isolate_real_roots_0_1_vca(fmpq * exact_roots, slong * n_exact_roots, fmpz * c_array, slong * k_array, slong * n_intervals, fmpz * pol, slong len);

void fmpz_poly_isolate_real_roots(fmpq * exact_roots, slong * n_exact, fmpz * c_array, slong * k_array, slong * n_interval, fmpz_poly_t pol);

#ifdef __cplusplus
 }
#endif

#endif
