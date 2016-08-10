/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of nf_emb

    nf_emb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "nf_emb_elem.h"
#include "fmpq_poly_extra.h"

void nf_emb_elem_set_evaluation(nf_emb_elem_t a, const nf_emb_t nf, slong prec)
{
	if (nf->nf->flag & NF_LINEAR)
	{
		fprintf(stderr, "[nf_emb_elem_set_evaluation] LINEAR");
		exit(EXIT_FAILURE);
	}
	else if(nf->nf->flag & NF_QUADRATIC)
	{
		arb_mul_fmpz(a->emb, nf->emb, QNF_ELEM_NUMREF(a->elem)+1, prec);
		arb_add_fmpz(a->emb, a->emb, QNF_ELEM_NUMREF(a->elem), prec);
		arb_div_fmpz(a->emb, a->emb, QNF_ELEM_DENREF(a->elem), prec);
	}
	else
	{
		fmpq_poly_evaluate_arb(a->emb, NF_ELEM(a->elem), nf->emb, prec);
	}
}


