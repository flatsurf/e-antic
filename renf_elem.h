/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#ifndef NF_EMB_ELEM_H
#define NF_EMB_ELEM_H

#include "gmp.h"
#include "flint.h"
#include "fmpq_poly.h"
#include "nf.h"
#include "nf_elem.h"
#include "arb.h"
#include "arb_poly.h"

#include "renf.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct
{
    nf_elem_t elem;    /* ANTIC number field element */
    arb_t emb;         /* the embedded real ball    */
} renf_elem_struct;


typedef renf_elem_struct renf_elem_t[1];

void renf_elem_init(renf_elem_t a, const renf_t nf);
void renf_elem_clear(renf_elem_t a, const renf_t nf);

void renf_elem_set_si(renf_elem_t a, slong c, const renf_t nf);
void renf_elem_set_fmpq_poly(renf_elem_t a, const fmpq_poly_t pol, const renf_t nf);

void renf_elem_floor(fmpz_t a, renf_elem_t b, renf_t nf);
void renf_elem_ceil(fmpz_t a, renf_elem_t b, renf_t nf);
int renf_elem_cmp(renf_elem_t a, renf_elem_t b, renf_t nf);

void renf_elem_set_evaluation(renf_elem_t a, const renf_t nf, slong prec);

void renf_elem_print_pretty(const renf_elem_t a, const renf_t nf, const char * var, slong prec);

/* inline set and binary operations */

static __inline__
void renf_elem_set_fmpz(renf_elem_t a, const fmpz_t c, const renf_t nf)
{
    nf_elem_set_fmpz(a->elem, c, nf->nf);
    arb_set_fmpz(a->emb, c);
}

static __inline__
void renf_elem_set_fmpq(renf_elem_t a, const fmpq_t c, const renf_t nf)
{
    nf_elem_set_fmpq(a->elem, c, nf->nf);
    arb_set_fmpq(a->emb, c, nf->prec);
}

static __inline__
void renf_elem_add_si(renf_elem_t a, const renf_elem_t b, slong c, const renf_t nf)
{
    nf_elem_add_si(a->elem, b->elem, c, nf->nf);
    arb_add_si(a->emb, b->emb, c, nf->prec);
}

static __inline__
void renf_elem_sub_si(renf_elem_t a, const renf_elem_t b, slong c, const renf_t nf)
{
    nf_elem_sub_si(a->elem, b->elem, c, nf->nf);
    arb_sub_si(a->emb, b->emb, c, nf->prec);
}

static __inline__
void renf_elem_mul_si(renf_elem_t a, const renf_elem_t b, slong c, const renf_t nf)
{
    nf_elem_scalar_mul_si(a->elem, b->elem, c, nf->nf);
    arb_mul_si(a->emb, b->emb, c, nf->prec);
}

static __inline__
void renf_elem_div_si(renf_elem_t a, const renf_elem_t b, slong c, const renf_t nf)
{
    nf_elem_scalar_div_si(a->elem, b->elem, c, nf->nf);
    arb_div_si(a->emb, b->emb, c, nf->prec);
}


static __inline__
void renf_elem_add_fmpz(renf_elem_t a, const renf_elem_t b, fmpz_t c, const renf_t nf)
{
    nf_elem_add_fmpz(a->elem, b->elem, c, nf->nf);
    arb_add_fmpz(a->emb, b->emb, c, nf->prec);
}

static __inline__
void renf_elem_sub_fmpz(renf_elem_t a, const renf_elem_t b, fmpz_t c, const renf_t nf)
{
    nf_elem_sub_fmpz(a->elem, b->elem, c, nf->nf);
    arb_sub_fmpz(a->emb, b->emb, c, nf->prec);
}

static __inline__
void renf_elem_mul_fmpz(renf_elem_t a, const renf_elem_t b, fmpz_t c, const renf_t nf)
{
    nf_elem_scalar_mul_fmpz(a->elem, b->elem, c, nf->nf);
    arb_mul_fmpz(a->emb, b->emb, c, nf->prec);
}

static __inline__
void renf_elem_div_fmpz(renf_elem_t a, const renf_elem_t b, fmpz_t c, const renf_t nf)
{
    nf_elem_scalar_div_fmpz(a->elem, b->elem, c, nf->nf);
    arb_div_fmpz(a->emb, b->emb, c, nf->prec);
}

static __inline__ 
void renf_elem_add(renf_elem_t a, const renf_elem_t b, const renf_elem_t c, const renf_t nf)
{
    nf_elem_add(a->elem, b->elem, c->elem, nf->nf);
    arb_add(a->emb, b->emb, c->emb, nf->prec);
}

static __inline__
void renf_elem_sub(renf_elem_t a, const renf_elem_t b, const renf_elem_t c, const renf_t nf)
{
    nf_elem_sub(a->elem, b->elem, c->elem, nf->nf);
    arb_sub(a->emb, b->emb, c->emb, nf->prec);
}

static __inline__
void renf_elem_mul(renf_elem_t a, const renf_elem_t b, const renf_elem_t c, const renf_t nf)
{
    nf_elem_mul(a->elem, b->elem, c->elem, nf->nf);
    arb_mul(a->emb, b->emb, c->emb, nf->prec);
}

static __inline__
void renf_elem_div(renf_elem_t a, const renf_elem_t b, const renf_elem_t c, const renf_t nf)
{
    nf_elem_div(a->elem, b->elem, c->elem, nf->nf);
    arb_div(a->emb, b->emb, c->emb, nf->prec);
}

static __inline__
void renf_elem_inv(renf_elem_t a, const renf_elem_t b, renf_t nf)
{
    nf_elem_inv(a->elem, b->elem, nf->nf);
    arb_inv(a->emb, b->emb, nf->prec);
}

#ifdef __cplusplus
}
#endif

#endif
