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

#include "flint/flint.h"
#include "flint/arb.h"
#include "flint/fmpz_poly.h"
#include "flint/fmpq_poly.h"

#ifdef __cplusplus
 extern "C" {
#endif

void arb_add_fmpq(arb_t a, const arb_t b, const fmpq_t c, slong prec);
void arb_sub_fmpq(arb_t a, const arb_t b, const fmpq_t c, slong prec);
void arb_mul_fmpq(arb_t a, const arb_t b, const fmpq_t c, slong prec);
void arb_div_fmpq(arb_t a, const arb_t b, const fmpq_t c, slong prec);

/* TODO: submit to FLINT */
void _fmpz_poly_scale_0_1_fmpq(fmpz * pol, slong len, fmpq_t a, fmpq_t b);

void fmpz_poly_randtest_irreducible(fmpz_poly_t p, flint_rand_t state, slong len, mp_bitcnt_t bits);

/* polynomial evaluation */

static __inline__
void fmpz_poly_evaluate_at_one(fmpz_t res, fmpz * p, slong len)
{
    slong i;

    fmpz_set(res, p);
    for (i = 1; i < len; i++)
        fmpz_add(res, res, p + i);
}

void _fmpz_poly_evaluate_arb(arb_t res, const fmpz * pol, slong len, const arb_t a, slong prec);

void fmpz_poly_evaluate_arb(arb_t b, const fmpz_poly_t pol, const arb_t a, slong prec);

void fmpq_poly_evaluate_arb(arb_t b, const fmpq_poly_t pol, const arb_t a, slong prec);

void _fmpz_poly_evaluate_arf(arf_t res, const fmpz * pol, slong len, const arf_t a, slong prec);

void fmpz_poly_evaluate_arf(arf_t res, const fmpz_poly_t pol, const arf_t a, slong prec);

void fmpq_poly_evaluate_arf(arf_t b, const fmpq_poly_t pol, const arf_t a, slong prec);

/* root refinement */
int fmpq_poly_check_unique_real_root(const fmpq_poly_t pol, const arb_t a, slong prec);

int _fmpz_poly_newton_step_arb(arb_t res, const fmpz * pol, const fmpz * der, slong len, arb_t a, slong prec);

int _fmpz_poly_bisection_step_arb(arb_t res, fmpz * pol, slong len, arb_t a, slong prec);

int fmpz_poly_newton_step_arb(arb_t res, const fmpz_poly_t pol, const fmpz_poly_t der, arb_t a, slong prec);

void _fmpz_poly_bisection_step_arf(arf_t l, arf_t r, const fmpz * pol, slong len, int sl, int sr, slong prec);

int fmpz_poly_bisection_step_arb(arb_t res, const fmpz_poly_t pol, arb_t a, slong prec);

void fmpz_poly_squarefree_part(fmpz_poly_t res, fmpz_poly_t poly);

/* root isolation */

/* TODO: submit to FLINT */
int _fmpz_poly_has_real_root(fmpz * pol, slong len);

/* TODO: submit to FLINT */
int fmpz_poly_has_complex_root(fmpz_poly_t pol);

static __inline__
int fmpz_poly_has_real_root(fmpz_poly_t pol)
{
    return _fmpz_poly_has_real_root(pol->coeffs, pol->length);
}


/* TODO: submit to FLINT */
slong fmpz_poly_positive_root_upper_bound_2exp(fmpz_poly_t pol);

/* TODO: submit to FLINT */
slong _fmpz_poly_positive_root_upper_bound_2exp(fmpz * pol, slong len);

/* TODO: submit to FLINT */
slong _fmpz_poly_descartes_bound_0_1(fmpz * p, slong len, slong bound);
slong _fmpz_poly_descartes_bound(fmpz * p, slong len, slong bound);

/* TODO: submit to FLINT */
slong fmpz_poly_num_real_roots_upper_bound(fmpz_poly_t pol);

/* TODO: submit to FLINT */
void _fmpz_poly_isolate_real_roots_0_1_vca(fmpq * exact_roots, slong * n_exact_roots, fmpz * c_array, slong * k_array, slong * n_intervals, fmpz * pol, slong len);

/* TODO: submit to FLINT */
void fmpz_poly_isolate_real_roots(fmpq * exact_roots, slong * n_exact, fmpz * c_array, slong * k_array, slong * n_interval, fmpz_poly_t pol);

/* TODO: submit to FLINT */
slong fmpz_poly_num_real_roots_vca(fmpz_poly_t pol);

slong fmpz_poly_num_real_roots_0_1_sturm(fmpz_poly_t pol);
slong fmpz_poly_num_real_roots_0_1_vca(fmpz_poly_t pol);
static __inline__
slong fmpz_poly_num_real_roots_0_1(fmpz_poly_t pol)
{
    return fmpz_poly_num_real_roots_0_1_vca(pol);
}

/* slong fmpz_poly_num_real_roots_interval_fmpq_sturm(fmpz_poly_t p, fmpq_t a, fmpq_t b);*/
/* slong fmpz_poly_num_real_roots_interval_fmpq_vca(fmpz_poly_t p, fmpq_t a, fmpq_t b);*/
/* slong fmpz_poly_num_real_roots_interval_fmpq(fmpz_poly_t p, fmpq_t a, fmpq_t b); */



/* OLD ****************************************************************/

/* submitted to FLINT (#308) */
/* slong _fmpz_poly_remove_content_2exp(fmpz * pol, slong len); */
/* void _fmpz_poly_scale_2exp(fmpz * pol, slong len, slong k);  */

/* merged in FLINT (#282) */
/* void _fmpq_vec_sort(fmpq * vec, slong len); */
/* void _fmpq_vec_randtest_uniq_sorted(fmpq * vec, flint_rand_t state, slong len, mp_bitcnt_t bits); */
/* void fmpz_poly_randtest_no_real_root(fmpz_poly_t p, flint_rand_t state, slong len, mp_bitcnt_t bits); */
/* void _fmpz_poly_num_real_roots_sturm(slong * n_neg, slong * n_pos, const fmpz * pol, slong len); */
/* slong fmpz_poly_num_real_roots_sturm(const fmpz_poly_t poly); */
/* slong _fmpz_poly_num_real_roots(const fmpz * pol, slong len); */
/* slong fmpz_poly_num_real_roots(const fmpz_poly_t poly); */

/* merged in FLINT (#279 and #295) */
/*void fmpz_poly_product_roots_fmpq_vec(fmpz_poly_t pol, fmpq * vec, slong len);*/

/* merged in FLINT (#278) */
/*int _fmpq_vec_fprint(FILE * file, const fmpq * vec, slong len);*/
/*void _fmpq_vec_print(const fmpq * vec, slong len);*/

#ifdef __cplusplus
 }
#endif

#endif
