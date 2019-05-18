/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#ifndef NF_EMB_ELEM_H
#define NF_EMB_ELEM_H

#include <flint/fmpq_poly.h>
#include <arb.h>
#include <arb_poly.h>
#include <e-antic/e-antic.h>
#include <e-antic/nf.h>
#include <e-antic/nf_elem.h>
#include <e-antic/poly_extra.h>
#include <e-antic/renf.h>

#ifdef __cplusplus
 extern "C" {
#endif

#define EANTIC_STR_ALG 1
#define EANTIC_STR_D   2
#define EANTIC_STR_ARB 4

typedef struct
{
    nf_elem_t elem;    /* ANTIC number field element */
    arb_t emb;         /* the embedded real ball    */
} renf_elem_struct;

typedef renf_elem_struct * renf_elem_srcptr;
typedef renf_elem_struct renf_elem_t[1];

void renf_elem_init(renf_elem_t a, const renf_t nf);
void renf_elem_clear(renf_elem_t a, const renf_t nf);

void renf_elem_set_fmpq_poly(renf_elem_t a, const fmpq_poly_t pol, const renf_t nf);
void renf_elem_gen(renf_elem_t a, const renf_t nf);

int renf_elem_sgn(renf_elem_t a, renf_t nf);
void renf_elem_floor(fmpz_t a, renf_elem_t b, renf_t nf);
void renf_elem_ceil(fmpz_t a, renf_elem_t b, renf_t nf);

int renf_elem_cmp(renf_elem_t a, renf_elem_t b, renf_t nf);
int _renf_elem_cmp_fmpq(renf_elem_t a, fmpz * num, fmpz * den, renf_t nf);
int renf_elem_cmp_fmpq(renf_elem_t a, const fmpq_t b, renf_t nf);

static __inline__
int renf_elem_cmp_fmpz(renf_elem_t a, const fmpz_t b, renf_t nf)
{
    int s;
    fmpq_t bq;

    fmpq_init(bq);
    fmpz_set(fmpq_numref(bq), b);  /* TODO: avoid copy! */
    fmpz_one(fmpq_denref(bq));

    s = renf_elem_cmp_fmpq(a, bq, nf);
    fmpq_clear(bq);
    return s;
}

static __inline__
int renf_elem_cmp_si(renf_elem_t a, const slong b, renf_t nf)
{
    int s;
    fmpq_t bq;

    fmpq_init(bq);
    fmpz_set_si(fmpq_numref(bq), b);  /* TODO: avoid copy! */
    fmpz_one(fmpq_denref(bq));

    s = renf_elem_cmp_fmpq(a, bq, nf);
    fmpq_clear(bq);
    return s;
}

static __inline__
int renf_elem_cmp_ui(renf_elem_t a, const slong b, renf_t nf)
{
    int s;
    fmpq_t bq;

    fmpq_init(bq);
    fmpz_set_si(fmpq_numref(bq), b);
    fmpz_one(fmpq_denref(bq));

    s = renf_elem_cmp_fmpq(a, bq, nf);
    fmpq_clear(bq);
    return s;
}

static __inline__
void renf_elem_swap(renf_elem_t a, renf_elem_t b)
{
    renf_elem_struct t = *a;
    *a = *b;
    *b = t;
}

double renf_elem_get_d(renf_elem_t a, renf_t nf, arf_rnd_t rnd);

/* return an arb approximation of a with a relative precision at least prec */
void renf_elem_get_arb(arb_t x, renf_elem_t a, renf_t nf, slong prec);

/* set c to the continued fraction of a */
slong renf_elem_get_cfrac(fmpz * c, renf_elem_t rem, renf_elem_t a, slong n, renf_t nf);

void renf_elem_set_evaluation(renf_elem_t a, const renf_t nf, slong prec);

char * renf_elem_get_str_pretty(renf_elem_t a, const char * var, renf_t nf, slong n, int flag);
void renf_elem_print_pretty(renf_elem_t a, const char * var, renf_t nf, slong n, int flag);

void renf_elem_randtest(renf_elem_t a, flint_rand_t state, mp_bitcnt_t bits, renf_t nf);

void renf_elem_check_embedding(const renf_elem_t a, const renf_t nf, slong prec);

int renf_elem_relative_condition_number_2exp(slong * cond, renf_elem_t a, renf_t nf);

/* inline set and binary operations */

static __inline__
int renf_elem_is_zero(const renf_elem_t a, const renf_t nf)
{
    return nf_elem_is_zero(a->elem, nf->nf);
}

static __inline__
int renf_elem_is_one(const renf_elem_t a, const renf_t nf)
{
    return nf_elem_is_one(a->elem, nf->nf);
}

static __inline__
void renf_elem_neg(renf_elem_t a, const renf_elem_t b, const renf_t nf)
{
    nf_elem_neg(a->elem, b->elem, nf->nf);
    arb_neg(a->emb, b->emb);
}

static __inline__
int renf_elem_equal(const renf_elem_t a, const renf_elem_t b, const renf_t nf)
{
    return nf_elem_equal(a->elem, b->elem, nf->nf);
}

static __inline__
int renf_elem_is_integer(const renf_elem_t a, const renf_t nf)
{
    return nf_elem_is_integer(a->elem, nf->nf);
}

static __inline__
int renf_elem_is_rational(const renf_elem_t a, const renf_t nf)
{
    return nf_elem_is_rational(a->elem, nf->nf);
}

static __inline__
int renf_elem_equal_si(const renf_elem_t a, const slong b, const renf_t nf)
{
    return nf_elem_equal_si(a->elem, b, nf->nf);
}

static __inline__
int renf_elem_equal_ui(const renf_elem_t a, const ulong b, const renf_t nf)
{
    return nf_elem_equal_ui(a->elem, b, nf->nf);
}

static __inline__
int renf_elem_equal_fmpz(const renf_elem_t a, const fmpz_t b, const renf_t nf)
{
    return nf_elem_equal_fmpz(a->elem, b, nf->nf);
}

static __inline__
int renf_elem_equal_fmpq(const renf_elem_t a, const fmpq_t b, const renf_t nf)
{
    return nf_elem_equal_fmpq(a->elem, b, nf->nf);
}

static __inline__
void renf_elem_zero(renf_elem_t a, const renf_t nf)
{
    nf_elem_zero(a->elem, nf->nf);
    arb_zero(a->emb);
}

static __inline__
void renf_elem_one(renf_elem_t a, const renf_t nf)
{
    nf_elem_one(a->elem, nf->nf);
    arb_one(a->emb);
}

static __inline__
void renf_elem_set_si(renf_elem_t a, slong n, const renf_t nf)
{
    nf_elem_set_si(a->elem, n, nf->nf);
    arb_set_si(a->emb, n);
}

static __inline__
void renf_elem_set_ui(renf_elem_t a, ulong n, const renf_t nf)
{
    nf_elem_set_ui(a->elem, n, nf->nf);
    arb_set_ui(a->emb, n);
}

static __inline__
void renf_elem_set_nf_elem(renf_elem_t a, const nf_elem_t b, renf_t nf, slong prec)
{
    nf_elem_set(a->elem, b, nf->nf);
    renf_elem_set_evaluation(a, nf, prec);
}

static __inline__
void renf_elem_set(renf_elem_t a, const renf_elem_t b, const renf_t nf)
{
    nf_elem_set(a->elem, b->elem, nf->nf);
    arb_set(a->emb, b->emb);
}

static __inline__
void renf_elem_set_fmpz(renf_elem_t a, const fmpz_t c, const renf_t nf)
{
    nf_elem_set_fmpz(a->elem, c, nf->nf);
    arb_set_fmpz(a->emb, c);
}

static __inline__
void renf_elem_set_mpz(renf_elem_t a, const mpz_t c, const renf_t nf)
{
    fmpz_t x;
    fmpz_init_set_readonly(x, c);
    renf_elem_set_fmpz(a, x, nf);
    fmpz_clear_readonly(x);
}

static __inline__
void renf_elem_set_fmpq(renf_elem_t a, const fmpq_t c, const renf_t nf)
{
    nf_elem_set_fmpq(a->elem, c, nf->nf);
    arb_set_fmpq(a->emb, c, nf->prec);
}

static __inline__
void renf_elem_set_mpq(renf_elem_t a, const mpq_t c, const renf_t nf)
{
    fmpq_t x;
    fmpq_init_set_readonly(x, c);
    renf_elem_set_fmpq(a, x, nf);
    fmpq_clear_readonly(x);
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
void renf_elem_add_ui(renf_elem_t a, const renf_elem_t b, ulong c, const renf_t nf)
{
    fmpz_t tmp;
    fmpz_init(tmp);
    fmpz_set_ui(tmp, c);
    nf_elem_add_fmpz(a->elem, b->elem, tmp, nf->nf);
    arb_add_fmpz(a->emb, b->emb, tmp, nf->prec);
    fmpz_clear(tmp);
}

static __inline__
void renf_elem_sub_ui(renf_elem_t a, const renf_elem_t b, ulong c, const renf_t nf)
{
    fmpz_t tmp;
    fmpz_init(tmp);
    fmpz_set_ui(tmp, c);
    nf_elem_sub_fmpz(a->elem, b->elem, tmp, nf->nf);
    arb_sub_fmpz(a->emb, b->emb, tmp, nf->prec);
    fmpz_clear(tmp);
}

static __inline__
void renf_elem_mul_ui(renf_elem_t a, const renf_elem_t b, ulong c, const renf_t nf)
{
    fmpz_t tmp;
    fmpz_init(tmp);
    fmpz_set_ui(tmp, c);
    nf_elem_scalar_mul_fmpz(a->elem, b->elem, tmp, nf->nf);
    arb_mul_fmpz(a->emb, b->emb, tmp, nf->prec);
    fmpz_clear(tmp);
}

static __inline__
void renf_elem_div_ui(renf_elem_t a, const renf_elem_t b, ulong c, const renf_t nf)
{
    fmpz_t tmp;
    fmpz_init(tmp);
    fmpz_set_ui(tmp, c);
    nf_elem_scalar_div_fmpz(a->elem, b->elem, tmp, nf->nf);
    arb_div_fmpz(a->emb, b->emb, tmp, nf->prec);
    fmpz_clear(tmp);
}

static __inline__
void renf_elem_add_fmpz(renf_elem_t a, const renf_elem_t b, const fmpz_t c, const renf_t nf)
{
    nf_elem_add_fmpz(a->elem, b->elem, c, nf->nf);
    arb_add_fmpz(a->emb, b->emb, c, nf->prec);
}

static __inline__
void renf_elem_sub_fmpz(renf_elem_t a, const renf_elem_t b, const fmpz_t c, const renf_t nf)
{
    nf_elem_sub_fmpz(a->elem, b->elem, c, nf->nf);
    arb_sub_fmpz(a->emb, b->emb, c, nf->prec);
}

static __inline__
void renf_elem_mul_fmpz(renf_elem_t a, const renf_elem_t b, const fmpz_t c, const renf_t nf)
{
    nf_elem_scalar_mul_fmpz(a->elem, b->elem, c, nf->nf);
    arb_mul_fmpz(a->emb, b->emb, c, nf->prec);
}

static __inline__
void renf_elem_div_fmpz(renf_elem_t a, const renf_elem_t b, const fmpz_t c, const renf_t nf)
{
    nf_elem_scalar_div_fmpz(a->elem, b->elem, c, nf->nf);
    arb_div_fmpz(a->emb, b->emb, c, nf->prec);
}

static __inline__
void renf_elem_add_fmpq(renf_elem_t a, const renf_elem_t b, const fmpq_t c, const renf_t nf)
{
    nf_elem_add_fmpq(a->elem, b->elem, c, nf->nf);
    arb_add_fmpq(a->emb, b->emb, c, nf->prec);
}

static __inline__
void renf_elem_sub_fmpq(renf_elem_t a, const renf_elem_t b, const fmpq_t c, const renf_t nf)
{
    nf_elem_sub_fmpq(a->elem, b->elem, c, nf->nf);
    arb_sub_fmpq(a->emb, b->emb, c, nf->prec);
}

static __inline__
void renf_elem_fmpq_sub(renf_elem_t a, const fmpq_t c, const renf_elem_t b, const renf_t nf)
{
    nf_elem_fmpq_sub(a->elem, c, b->elem, nf->nf);
    arb_fmpq_sub(a->emb, c, b->emb, nf->prec);
}

static __inline__
void renf_elem_mul_fmpq(renf_elem_t a, const renf_elem_t b, const fmpq_t c, const renf_t nf)
{
    nf_elem_scalar_mul_fmpq(a->elem, b->elem, c, nf->nf);
    arb_mul_fmpq(a->emb, b->emb, c, nf->prec);
}

static __inline__
void renf_elem_inv(renf_elem_t a, const renf_elem_t b, const renf_t nf)
{
    nf_elem_inv(a->elem, b->elem, nf->nf);
    arb_inv(a->emb, b->emb, nf->prec);
}

static __inline__
void renf_elem_div_fmpq(renf_elem_t a, const renf_elem_t b, const fmpq_t c, const renf_t nf)
{
    nf_elem_scalar_div_fmpq(a->elem, b->elem, c, nf->nf);
    arb_div_fmpq(a->emb, b->emb, c, nf->prec);
}

static __inline__
void renf_elem_fmpq_div(renf_elem_t a, const fmpq_t b, const renf_elem_t c, const renf_t nf)
{
    renf_elem_set(a, c, nf);
    renf_elem_inv(a, a, nf);
    renf_elem_mul_fmpq(a, c, b, nf);
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

#ifdef __cplusplus
}
#endif

#endif
