/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
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

#ifdef __cplusplus
 extern "C" {
#endif

#define NF_EMB_REAL 1
#define NF_EMB_COMPLEX 2

typedef struct
{
  nf_t nf;
  fmpz_poly_t der;  /* derivative (without denominator)         */
  slong prec;       /* default precision for arb computations   */
  union {
	  arb_t remb;   /* embedding of generator as a real ball    */
	  acb_t cemb;   /* embedding of generator as a complex ball */
  } emb;
  ulong flag;       /* whether this is real or complex          */
} nf_emb_struct;

typedef nf_emb_struct nf_emb_t[1];

#define NF_REMB_REF(xxx) (((xxx)->emb).remb)
#define NF_CEMB_REF(xxx) (((xxx)->emb).cemb)

void nf_emb_real_init(nf_emb_t nf, fmpq_poly_t pol, arb_t emb, slong prec);
void nf_emb_complex_init(nf_emb_t nf, fmpq_poly_t pol, acb_t emb, slong prec);
void nf_emb_init_nth_root_fmpq(nf_emb_t nf, fmpq_t d, ulong n, slong prec);

void nf_emb_clear(nf_emb_t nf);
void nf_emb_refine_embedding(nf_emb_t nf, slong prec);

#ifdef __cplusplus
}
#endif

#endif
