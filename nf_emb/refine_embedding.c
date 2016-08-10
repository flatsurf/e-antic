/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of nf_emb

    nf_emb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "fmpq_poly_extra.h"
#include "nf_emb.h"

void nf_emb_refine_embedding(nf_emb_t nf, slong prec)
{
/*	arb_t a;
	arf_t convergence_factor;

	arb_init(a);
	arf_init(convergence_factor);
	_arb_poly_newton_convergence_factor(
		convergence_factor,
		nf->pol->coeffs,
		nf->pol->length,
		nf->emb,
		prec);

	_arb_poly_newton_refine_root(
			a, 
			nf->pol->coeffs,
			nf->pol->length,
			nf->emb,
			nf->emb,
			convergence_factor,
			10,
			prec);

	arb_swap(a, nf->emb);
	arb_clear(a);
	arf_clear(convergence_factor);
*/
	fmpq_poly_newton_step_arb(nf->emb, nf->nf->pol, nf->der, nf->emb, prec);
}
