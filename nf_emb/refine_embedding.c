/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "fmpq_poly_extra.h"
#include "nf_emb.h"

void nf_emb_refine_embedding(nf_emb_t nf, slong prec)
{
	fmpq_poly_newton_step_arb(nf->emb, nf->nf->pol, nf->der, nf->emb, prec);
}
