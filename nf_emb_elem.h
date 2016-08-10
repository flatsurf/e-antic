/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of nf_emb

    nf_emb is free software: you can redistribute it and/or modify it under
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

#include "nf_emb.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct
{
	nf_elem_t elem;    /* ANTIC number field element */
	arb_t emb;         /* the embedded ball */
} nf_emb_elem_struct;

typedef nf_emb_elem_struct nf_emb_elem_t[1];

void nf_emb_elem_init(nf_emb_elem_t a, const nf_emb_t nf);
void nf_emb_elem_clear(nf_emb_elem_t a, const nf_emb_t nf);

void nf_emb_elem_set_si(nf_emb_elem_t a, slong c, const nf_emb_t nf);
void nf_emb_elem_set_fmpq_poly(nf_emb_elem_t a, const fmpq_poly_t pol, const nf_emb_t nf);

void nf_emb_elem_floor(fmpz_t a, nf_emb_elem_t b, nf_emb_t nf);
void nf_emb_elem_ceil(fmpz_t a, nf_emb_elem_t b, nf_emb_t nf);
int nf_emb_elem_cmp(nf_emb_elem_t a, nf_emb_elem_t b, nf_emb_t nf);

void nf_emb_elem_set_evaluation(nf_emb_elem_t a, const nf_emb_t nf, slong prec);

void nf_emb_elem_print_pretty(const nf_emb_elem_t a, const nf_emb_t nf, const char * var);

/* inline set and binary operations */

static __inline__
void nf_emb_elem_set_fmpz(nf_emb_elem_t a, const fmpz_t c, const nf_emb_t nf)
{
	nf_elem_set_fmpz(a->elem, c, nf->nf);
	arb_set_fmpz(a->emb, c);
}

static __inline__
void nf_emb_elem_set_fmpq(nf_emb_elem_t a, const fmpq_t c, const nf_emb_t nf)
{
	nf_elem_set_fmpq(a->elem, c, nf->nf);
	arb_set_fmpq(a->emb, c, DEFAULT_PREC);
}

static __inline__
void nf_emb_elem_add_si(nf_emb_elem_t a, const nf_emb_elem_t b, slong c, const nf_emb_t nf)
{
	nf_elem_add_si(a->elem, b->elem, c, nf->nf);
	arb_add_si(a->emb, b->emb, c, DEFAULT_PREC);
}

static __inline__
void nf_emb_elem_sub_si(nf_emb_elem_t a, const nf_emb_elem_t b, slong c, const nf_emb_t nf)
{
	nf_elem_sub_si(a->elem, b->elem, c, nf->nf);
	arb_sub_si(a->emb, b->emb, c, DEFAULT_PREC);
}

static __inline__
void nf_emb_elem_mul_si(nf_emb_elem_t a, const nf_emb_elem_t b, slong c, const nf_emb_t nf)
{
	nf_elem_scalar_mul_si(a->elem, b->elem, c, nf->nf);
	arb_mul_si(a->emb, b->emb, c, DEFAULT_PREC);
}

static __inline__
void nf_emb_elem_div_si(nf_emb_elem_t a, const nf_emb_elem_t b, slong c, const nf_emb_t nf)
{
	nf_elem_scalar_div_si(a->elem, b->elem, c, nf->nf);
	arb_div_si(a->emb, b->emb, c, DEFAULT_PREC);
}


static __inline__
void nf_emb_elem_add_fmpz(nf_emb_elem_t a, const nf_emb_elem_t b, fmpz_t c, const nf_emb_t nf)
{
	nf_elem_add_fmpz(a->elem, b->elem, c, nf->nf);
	arb_add_fmpz(a->emb, b->emb, c, DEFAULT_PREC);
}

static __inline__
void nf_emb_elem_sub_fmpz(nf_emb_elem_t a, const nf_emb_elem_t b, fmpz_t c, const nf_emb_t nf)
{
	nf_elem_sub_fmpz(a->elem, b->elem, c, nf->nf);
	arb_sub_fmpz(a->emb, b->emb, c, DEFAULT_PREC);
}

static __inline__
void nf_emb_elem_mul_fmpz(nf_emb_elem_t a, const nf_emb_elem_t b, fmpz_t c, const nf_emb_t nf)
{
	nf_elem_scalar_mul_fmpz(a->elem, b->elem, c, nf->nf);
	arb_mul_fmpz(a->emb, b->emb, c, DEFAULT_PREC);
}

static __inline__
void nf_emb_elem_div_fmpz(nf_emb_elem_t a, const nf_emb_elem_t b, fmpz_t c, const nf_emb_t nf)
{
	nf_elem_scalar_div_fmpz(a->elem, b->elem, c, nf->nf);
	arb_div_fmpz(a->emb, b->emb, c, DEFAULT_PREC);
}

static __inline__ 
void nf_emb_elem_add(nf_emb_elem_t a, const nf_emb_elem_t b, const nf_emb_elem_t c, const nf_emb_t nf)
{
	nf_elem_add(a->elem, b->elem, c->elem, nf->nf);
	arb_add(a->emb, b->emb, c->emb, DEFAULT_PREC);
}

static __inline__
void nf_emb_elem_sub(nf_emb_elem_t a, const nf_emb_elem_t b, const nf_emb_elem_t c, const nf_emb_t nf)
{
	nf_elem_sub(a->elem, b->elem, c->elem, nf->nf);
	arb_sub(a->emb, b->emb, c->emb, DEFAULT_PREC);
}

static __inline__
void nf_emb_elem_mul(nf_emb_elem_t a, const nf_emb_elem_t b, const nf_emb_elem_t c, const nf_emb_t nf)
{
	nf_elem_mul(a->elem, b->elem, c->elem, nf->nf);
	arb_mul(a->emb, b->emb, c->emb, DEFAULT_PREC);
}

static __inline__
void nf_emb_elem_div(nf_emb_elem_t a, const nf_emb_elem_t b, const nf_emb_elem_t c, const nf_emb_t nf)
{
	nf_elem_div(a->elem, b->elem, c->elem, nf->nf);
	arb_div(a->emb, b->emb, c->emb, DEFAULT_PREC);
}

static __inline__
void nf_emb_elem_inv(nf_emb_elem_t a, const nf_emb_elem_t b, nf_emb_t nf)
{
	nf_elem_inv(a->elem, b->elem, nf->nf);
	arb_inv(a->emb, b->emb, DEFAULT_PREC);
}

#ifdef __cplusplus
}
#endif


#endif
