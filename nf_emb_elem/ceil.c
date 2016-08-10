/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "nf_emb_elem.h"

void nf_emb_elem_ceil(fmpz_t a, nf_emb_elem_t b, nf_emb_t nf)
{
	arf_t cl, cr;
	slong prec;

	arf_init(cl);
	arf_init(cr);
	prec = DEFAULT_PREC;
	do{
		arb_get_interval_arf(cl, cr, b->emb, prec);
#ifdef DEBUG
		printf("[ceil] cl = "); arf_printd(cl, 30); printf("\n");
		printf("[ceil] cr = "); arf_printd(cr, 30); printf("\n");
#endif
		arf_ceil(cl, cl);
		arf_ceil(cr, cr);
#ifdef DEBUG
		printf("[ceil] ceil(cl) = "); arf_printd(cl, 30); printf("\n");
		printf("[ceil] ceil(cr) = "); arf_printd(cr, 30); printf("\n");
#endif
		if(arf_equal(cl,cr))
		{
			arf_get_fmpz(a, cl, ARF_RND_NEAR);
			arf_clear(cl);
			arf_clear(cr);
			return;
		}
		prec *= 2;
		if(arf_bits(arb_midref(nf->emb)) < prec)
			nf_emb_refine_embedding(nf, 2*prec);
		if(2*arf_bits(arb_midref(b->emb)) < prec)
			nf_emb_elem_set_evaluation(b, nf, prec);
	}while(1);
}

