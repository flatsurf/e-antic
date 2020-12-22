/*
   Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_FMPQ_POLY_EXTRA_H
#define E_ANTIC_FMPQ_POLY_EXTRA_H

#include "e-antic.h"

#include <flint/fmpq_poly.h>
#include <arb.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************/
/* Real root isolation for integer polynomials (to be moved to FLINT) */

int fmpq_poly_set_str_pretty(fmpq_poly_t p, const char * s, const char * var);

/****************************************************************************/
/* FLINT/ARB extra                                                          */

void fmpq_poly_evaluate_arb(arb_t b, const fmpq_poly_t pol, const arb_t a, slong prec);

void fmpq_poly_evaluate_arf(arf_t b, const fmpq_poly_t pol, const arf_t a, slong prec);

/* root refinement */
int fmpq_poly_check_unique_real_root(const fmpq_poly_t pol, const arb_t a, slong prec);

#ifdef __cplusplus
}
#endif

#endif
