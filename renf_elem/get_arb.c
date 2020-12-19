/*
    Copyright (C) 2019 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/renf_elem.h"

void
renf_elem_get_arb(arb_t x, renf_elem_t a, renf_t nf, slong prec)
{
    slong cond;

    if (nf_elem_is_zero(a->elem, nf->nf))
    {
        arb_zero(x);
        return;
    }

    if (arb_rel_error_bits(a->emb) < -prec-2)
    {
        arb_set_round(x, a->emb, prec+2);
        return;
    }

    renf_elem_relative_condition_number_2exp(&cond, a, nf);
    slong pprec = FLINT_MAX(nf->prec, arb_rel_accuracy_bits(nf->emb));
    do
    {
        renf_elem_set_evaluation(a, nf, pprec + cond);
        if (arb_rel_error_bits(a->emb) < -prec-2)
        {
            arb_set_round(x, a->emb, prec+2);
            return;
        }
        pprec *= 2;
        renf_refine_embedding(nf, pprec);
    } while(1);
}
