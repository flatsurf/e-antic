/*
    Copyright (C) 2016 Vincent Delecroix
                  2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_RENF_H
#define E_ANTIC_RENF_H

#include <flint/fmpq_poly.h>
#include <antic/nf.h>
#include <antic/nf_elem.h>
#include <e-antic/e-antic.h>
#include <arb.h>
#include <arb_poly.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  nf_t nf;
  fmpz_poly_t der;   /* derivative (without denominator)                   */
  slong prec;        /* precision used for binary operations on enclosures */
  arb_t emb;         /* embedding of generator as a real ball              */
  int immutable;     /* whether emb cannot be refined currently            */
} renf;

typedef renf * renf_ptr;
typedef const renf * renf_srcptr;
typedef renf renf_t[1];

void renf_init(renf_t nf, fmpq_poly_t pol, arb_t emb, slong prec);
void renf_init_nth_root_fmpq(renf_t nf, fmpq_t d, ulong n, slong prec);

void renf_init_set(renf_t dest, const renf_t src);

void renf_clear(renf_t nf);

slong renf_set_embeddings_fmpz_poly(renf * nf, fmpz_poly_t pol, slong lim, slong prec);

void renf_refine_embedding(renf_t nf, slong prec);

int renf_set_immutable(renf_t nf, int immutable);

int renf_equal(const renf_t nf1, const renf_t nf2);

slong renf_degree(renf_t nf);

/* Randomisation */

void renf_randtest(renf_t nf, flint_rand_t state, slong len, slong prec, mp_bitcnt_t bits);

/* Printing */
char * renf_get_str(const renf_t nf, const char * var, slong prec);
void renf_print(const renf_t nf);

#ifdef __cplusplus
}
#endif

#endif
