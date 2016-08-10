/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of nf_emb

    nf_emb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#ifndef NF_EMB_H
#define NF_EMB_H

#include "gmp.h"
#include "flint.h"
#include "fmpq_poly.h"
#include "nf.h"
#include "nf_elem.h"
#include "arb.h"
#include "arb_poly.h"

#define DEFAULT_PREC 64

#ifdef __cplusplus
 extern "C" {
#endif

/* embedding of the generators of a nf */
typedef struct
{
  nf_t nf;
  fmpq_poly_t der;  /* derivative */
  arb_t emb;        /* embedding of generator as a ball */
} nf_emb_struct;

typedef nf_emb_struct nf_emb_t[1];

void nf_emb_init(nf_emb_t nf, fmpq_poly_t pol, arb_t emb);
void nf_emb_init_nth_root_fmpq(nf_emb_t nf, fmpq_t d, ulong n);

void nf_emb_clear(nf_emb_t nf);
void nf_emb_refine_embedding(nf_emb_t nf, slong prec);

#ifdef __cplusplus
}
#endif

#endif
