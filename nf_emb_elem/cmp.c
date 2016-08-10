/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of nf_emb

    nf_emb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "nf_emb_elem.h"

int nf_emb_elem_cmp(nf_emb_elem_t a, nf_emb_elem_t b, nf_emb_t nf)
{
	arb_t difference;
	slong prec;

	/* equality? */
	if(nf_elem_equal(a->elem, b->elem, nf->nf)) return 0;

	/* try default precision */
	arb_init(difference);
	arb_sub(difference, a->emb, b->emb, DEFAULT_PREC);
	if(!arb_contains_zero(difference))
	{
		arb_clear(difference);
		return arf_sgn(arb_midref(difference));
	}

	/* precision doubling up to the current precision */
	prec = 2*DEFAULT_PREC;
	while(prec < arf_bits(arb_midref(nf->emb)))
	{
		if(2*arf_bits(arb_midref(a->emb)) < prec)
			nf_emb_elem_set_evaluation(a, nf, prec);
		if(2*arb_bits(arb_midref(b->emb)) < prec)
			nf_emb_elem_set_evaluation(b, nf, prec);
		arb_sub(difference, a->emb, b->emb, prec);
		if(!arb_contains_zero(difference))
		{
			arb_clear(difference);
			return arf_sgn(arb_midref(difference));
		}
		prec *= 2;
	}

	/* augment precision ad libitum */
	do{
		nf_emb_refine_embedding(nf, prec);
		nf_emb_elem_set_evaluation(a, nf, prec);
		nf_emb_elem_set_evaluation(b, nf, prec);
		arb_sub(difference, a->emb, b->emb, prec);
		if(!arb_contains_zero(difference))
		{
			arb_clear(difference);
			return arf_sgn(arb_midref(difference));
		}
		prec *= 2;
	}while(1);
}


