/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/renf_elem.h"
#include "../e-antic/fmpq_poly_extra.h"

void renf_elem_check_embedding(const renf_elem_t a, const renf_t nf, slong prec)
{
    arb_t emb;

    arb_init(emb);

    if (nf->nf->flag & NF_LINEAR)
    {
        arb_fmpz_div_fmpz(emb, LNF_ELEM_NUMREF(a->elem), LNF_ELEM_DENREF(a->elem), prec);
    }
    else if (nf->nf->flag & NF_QUADRATIC)
    {
        arb_mul_fmpz(emb, nf->emb, QNF_ELEM_NUMREF(a->elem) + 1, prec);
        arb_add_fmpz(emb, emb, QNF_ELEM_NUMREF(a->elem), prec);
        arb_div_fmpz(emb, emb, QNF_ELEM_DENREF(a->elem), prec);
    }
    else
    {
        fmpq_poly_evaluate_arb(emb, NF_ELEM(a->elem), nf->emb, prec);
    }

    if (!arb_overlaps(a->emb, emb))
    {
        fprintf(stderr, "embedding set to "); arb_fprint(stderr, a->emb);
        fprintf(stderr, " but got "); arb_fprint(stderr, emb); fprintf(stderr, "\n");
        arb_clear(emb);
        abort();
    }

    arb_clear(emb);
}
