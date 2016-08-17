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

void fmpz_poly_randtest_rational_roots(fmpz_poly_t p, flint_rand_t state, mp_bitcnt_t bits, int real_pos_roots, int real_neg_roots, int complex_roots);

void _fmpz_poly_evaluate_arb(arb_t res, const fmpz * pol, slong len, const arb_t a, slong prec);
void fmpz_poly_evaluate_arb(arb_t b, const fmpz_poly_t pol, const arb_t a, slong prec);
void fmpq_poly_evaluate_arb(arb_t b, const fmpq_poly_t pol, const arb_t a, slong prec);


void _fmpz_poly_evaluate_acb(acb_t b, const fmpz * pol, slong len, const acb_t a, slong prec);
void fmpz_poly_evaluate_acb(acb_t b, const fmpz_poly_t pol, const acb_t a, slong prec);
void fmpq_poly_evaluate_acb(acb_t b, const fmpq_poly_t pol, const acb_t a, slong prec);

void _fmpz_poly_evaluate_arf(arf_t res, const fmpz * pol, slong len, const arf_t a, slong prec);
void fmpz_poly_evaluate_arf(arf_t res, const fmpz_poly_t pol, const arf_t a, slong prec);
void fmpq_poly_evaluate_arf(arf_t b, const fmpq_poly_t pol, const arf_t a, slong prec);

int fmpq_poly_check_unique_real_root(const fmpq_poly_t pol, const arb_t a, slong prec);
int fmpq_poly_check_unique_complex_root(const fmpq_poly_t pol, const acb_t a, slong prec);

int _fmpz_poly_newton_step_arb(arb_t res, const fmpz * pol, const fmpz * der, slong len, arb_t a, slong prec);
int fmpz_poly_newton_step_arb(arb_t res, const fmpz_poly_t pol, const fmpz_poly_t der, arb_t a, slong prec);

int _fmpz_poly_newton_step_acb(acb_t res, const fmpz * pol, const fmpz * der, slong len, acb_t a, slong prec);
int fmpz_poly_newton_step_acb(acb_t res, const fmpz_poly_t pol, const fmpz_poly_t der, acb_t a, slong prec);

void _fmpz_poly_bisection_step_arb(arb_t res, const fmpz * pol, slong len, arb_t a, slong prec);
int fmpz_poly_bisection_step_arb(arb_t res, const fmpz_poly_t pol, arb_t a, slong prec);

int fmpz_poly_num_real_roots(arb_t res, const fmpz_t pol, slong len);

slong fmpz_poly_positive_root_upper_bound_2exp(fmpz_poly_t pol);
slong _fmpz_poly_positive_root_upper_bound_2exp(fmpz * pol, slong len);

slong _fmpz_poly_descartes_bound(fmpz * p, fmpz * q, slong len);
slong fmpz_poly_descartes_bound(fmpz_poly_t pol);

#ifdef __cplusplus
 }
#endif

#endif
