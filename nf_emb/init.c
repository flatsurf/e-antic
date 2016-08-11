/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "nf_emb.h"
#include "poly_extra.h"

void _nf_emb_init_common(nf_emb_t nf, fmpq_poly_t pol, slong prec)
{
    const slong len = fmpq_poly_length(pol);

    nf_init(nf->nf, pol);

    fmpz_poly_init(nf->der);
    fmpz_poly_fit_length(nf->der, len - 1);
    _fmpz_poly_derivative(nf->der->coeffs, fmpq_poly_numref(pol), len);
    _fmpz_poly_set_length(nf->der, len - 1);

    nf->prec = prec;
 
    nf_emb_refine_embedding(nf, 2 * prec);
}

void nf_emb_real_init(nf_emb_t nf, fmpq_poly_t pol, arb_t emb, slong prec)
{
    if (!fmpq_poly_check_unique_real_root(pol, emb, prec))
    {
        printf("the given polynomial does not define a unique root\n");
        exit(EXIT_FAILURE);
    }
#ifdef DEBUG
    printf("[nf_emb_real_init]: pol = "); fmpq_poly_print_pretty(pol, "x"); printf("\n");
    printf("[nf_emb_real_init]: emb = "); arb_printd(emb, 10); printf("\n");
    flint_printf("[nf_emb_real_init]: prec = %wd\n", prec);
#endif

    arb_init(NF_REMB_REF(nf));
    arb_set(NF_REMB_REF(nf), emb);

    nf->flag = NF_EMB_REAL;

    _nf_emb_init_common(nf, pol, prec);
}

void nf_emb_complex_init(nf_emb_t nf, fmpq_poly_t pol, acb_t emb, slong prec)
{
    if (!fmpq_poly_check_unique_complex_root(pol, emb, prec))
    {
        printf("the given polynomial does not define a unique root\n");
        exit(EXIT_FAILURE);
    }

    acb_init(NF_CEMB_REF(nf));
    acb_set(NF_CEMB_REF(nf), emb);

    nf->flag = NF_EMB_COMPLEX;

    _nf_emb_init_common(nf, pol, prec);
}
