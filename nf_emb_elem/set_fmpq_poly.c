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

void nf_emb_elem_set_fmpq_poly(nf_emb_elem_t a, const fmpq_poly_t pol, const nf_emb_t nf)
{
    nf_elem_set_fmpq_poly(a->elem, pol, nf->nf);
    if (nf->flag & NF_EMB_REAL)
        fmpq_poly_evaluate_arb(NF_ELEM_REMB_REF(a), pol, NF_REMB_REF(nf), nf->prec);
    else
        fmpq_poly_evaluate_acb(NF_ELEM_CEMB_REF(a), pol, NF_CEMB_REF(nf), nf->prec);
}


