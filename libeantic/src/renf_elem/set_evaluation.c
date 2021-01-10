/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/renf_elem.h"
#include "../e-antic/fmpq_poly_extra.h"

void renf_elem_set_evaluation(renf_elem_t a, const renf_t nf, slong prec)
{
    if (nf->nf->flag & NF_LINEAR)
    {
        arb_fmpz_div_fmpz(a->emb, LNF_ELEM_NUMREF(a->elem), LNF_ELEM_DENREF(a->elem), prec);
    }
    else if (nf->nf->flag & NF_QUADRATIC)
    {
        arb_mul_fmpz(a->emb, nf->emb, QNF_ELEM_NUMREF(a->elem) + 1, prec);
        arb_add_fmpz(a->emb, a->emb, QNF_ELEM_NUMREF(a->elem), prec);
        arb_div_fmpz(a->emb, a->emb, QNF_ELEM_DENREF(a->elem), prec);
    }
    else
    {
        fmpq_poly_evaluate_arb(a->emb, NF_ELEM(a->elem), nf->emb, prec);
    }
}
