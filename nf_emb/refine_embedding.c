/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"
#include "nf_emb.h"

/* TODO: use bisection if it fails */
/* TODO: use multiple Newton if possible */

void nf_emb_refine_embedding(nf_emb_t nf, slong prec)
{
    if (nf->flag & NF_EMB_REAL)
	{
		arb_t tmp;
		arb_init(tmp);
#ifdef DEBUG
		printf("[_nf_emb_refine_embedding]: before emb=");
		arb_printd(NF_REMB_REF(nf), 10);
		printf("\n");
#endif
        _fmpz_poly_newton_step_arb(tmp,
                fmpq_poly_numref(nf->nf->pol),
                nf->der->coeffs,
                fmpq_poly_length(nf->nf->pol),
                NF_REMB_REF(nf),
                prec);
		arb_swap(tmp, NF_REMB_REF(nf));
		arb_clear(tmp);
#ifdef DEBUG
		printf("[_nf_emb_refine_embedding]: after emb=");
		arb_printd(NF_REMB_REF(nf), 10);
		printf("\n");
#endif

	}

    else
	{
		acb_t tmp;
		acb_init(tmp);
        _fmpz_poly_newton_step_acb(tmp,
                fmpq_poly_numref(nf->nf->pol),
                nf->der->coeffs,
                fmpq_poly_length(nf->nf->pol),
                NF_CEMB_REF(nf),
                prec);
		acb_swap(tmp, NF_CEMB_REF(nf));
		acb_clear(tmp);
	}
}
