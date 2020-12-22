/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/renf_elem.h"

int renf_elem_sgn(renf_elem_t a, renf_t nf)
{
    slong prec;
    slong cond;

    if (nf_elem_is_rational(a->elem, nf->nf))
    {
        if (nf->nf->flag & NF_LINEAR)
            return fmpz_sgn(LNF_ELEM_NUMREF(a->elem));
        else if (nf->nf->flag & NF_QUADRATIC)
            return fmpz_sgn(QNF_ELEM_NUMREF(a->elem));
        else if (NF_ELEM(a->elem)->length == 0)
            return 0;
        else
            return fmpz_sgn(NF_ELEM_NUMREF(a->elem));
    }

    if (!arb_contains_zero(a->emb))
        return arf_sgn(arb_midref(a->emb));

    renf_elem_relative_condition_number_2exp(&cond, a, nf);
    prec = FLINT_MAX(nf->prec, arb_rel_accuracy_bits(nf->emb));
    renf_elem_set_evaluation(a, nf, prec + cond);

    do
    {
        if (!arb_contains_zero(a->emb))
            return arf_sgn(arb_midref(a->emb));

        prec *= 2;
        renf_refine_embedding(nf, prec);
        renf_elem_set_evaluation(a, nf, prec + cond);
    } while(1);
}

