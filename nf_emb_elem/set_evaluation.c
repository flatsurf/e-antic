/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "nf_emb_elem.h"
#include "poly_extra.h"

void nf_emb_elem_set_evaluation(nf_emb_elem_t a, const nf_emb_t nf, slong prec)
{
    if (nf->nf->flag & NF_LINEAR)
    {
        fprintf(stderr, "[nf_emb_elem_set_evaluation] LINEAR");
        exit(EXIT_FAILURE);
    }
    else if (nf->nf->flag & NF_QUADRATIC)
    {
        if (nf->flag & NF_EMB_REAL)
        {
            arb_mul_fmpz(NF_ELEM_REMB_REF(a), NF_REMB_REF(nf), QNF_ELEM_NUMREF(a->elem) + 1, prec);
            arb_add_fmpz(NF_ELEM_REMB_REF(a), NF_ELEM_REMB_REF(a), QNF_ELEM_NUMREF(a->elem), prec);
            arb_div_fmpz(NF_ELEM_REMB_REF(a), NF_ELEM_REMB_REF(a), QNF_ELEM_DENREF(a->elem), prec);
        }
        else
        {
            acb_mul_fmpz(NF_ELEM_CEMB_REF(a), NF_CEMB_REF(nf), QNF_ELEM_NUMREF(a->elem) + 1, prec);
            acb_add_fmpz(NF_ELEM_CEMB_REF(a), NF_ELEM_CEMB_REF(a), QNF_ELEM_NUMREF(a->elem), prec);
            acb_div_fmpz(NF_ELEM_CEMB_REF(a), NF_ELEM_CEMB_REF(a), QNF_ELEM_DENREF(a->elem), prec);
        }
    }
    else
    {
        if (nf->flag & NF_EMB_REAL)
            fmpq_poly_evaluate_arb(NF_ELEM_REMB_REF(a), NF_ELEM(a->elem), NF_REMB_REF(nf), prec);
        else
            fmpq_poly_evaluate_acb(NF_ELEM_CEMB_REF(a), NF_ELEM(a->elem), NF_CEMB_REF(nf), prec);
    }
}

