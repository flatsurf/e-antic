/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef NF_EMB_H
#define NF_EMB_H

#include <flint/fmpq_poly.h>
#include <e-antic/e-antic.h>
#include <e-antic/nf.h>
#include <e-antic/nf_elem.h>
#include <arb.h>
#include <arb_poly.h>

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct
{
  nf_t nf;
  fmpz_poly_t der;  /* derivative (without denominator)         */
  slong prec;       /* default precision for arb computations   */
  arb_t emb;        /* embedding of generator as a real ball    */
} renf;

typedef renf * renf_ptr;
typedef const renf * renf_srcptr;
typedef renf renf_t[1];

void renf_init(renf_t nf, fmpq_poly_t pol, arb_t emb, slong prec);
void renf_init_nth_root_fmpq(renf_t nf, fmpq_t d, ulong n, slong prec);

void renf_init_set(renf_t dest, const renf_t src);

static __inline__
void renf_clear(renf_t nf)
{
    nf_clear(nf->nf);
    fmpz_poly_clear(nf->der);
    arb_clear(nf->emb);
}
void renf_refine_embedding(renf_t nf, slong prec);

slong renf_set_embeddings_fmpz_poly(renf * nf, fmpz_poly_t pol, slong lim, slong prec);


static __inline__
int nf_equal(const nf_t nf1, const nf_t nf2)
{
    return fmpq_poly_equal(nf1->pol, nf2->pol);
}

static __inline__
int renf_equal(const renf_t nf1, const renf_t nf2)
{
    return nf_equal(nf1->nf, nf2->nf) && arb_overlaps(nf1->emb, nf2->emb);
}

/* Randomisation */

void renf_randtest(renf_t nf, flint_rand_t state, slong len, mp_bitcnt_t bits);

void renf_print(const renf_t nf);

#ifdef __cplusplus
}
#endif

#endif
