/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "nf_emb.h"
#include "fmpq_poly_extra.h"

void nf_emb_init(nf_emb_t nf, fmpq_poly_t pol, arb_t emb)
{
	if(!fmpq_poly_check_unique_root(pol, emb, DEFAULT_PREC))
	{
		printf("the given polynomial does not define a unique root\n");
		exit(EXIT_FAILURE);
	}

	nf_init(nf->nf, pol);

	fmpq_poly_init(nf->der);
	fmpq_poly_derivative(nf->der, pol);

	arb_init(nf->emb);
	arb_set(nf->emb, emb);
 
	nf_emb_refine_embedding(nf, 64);
}

