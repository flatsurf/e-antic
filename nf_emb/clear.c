/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "nf_emb.h"

void nf_emb_clear(nf_emb_t nf)
{
	nf_clear(nf->nf);
	fmpz_poly_clear(nf->der);

	if (nf->flag & NF_EMB_REAL)
		arb_clear(NF_REMB_REF(nf));
	else
		acb_clear(NF_CEMB_REF(nf));
}


