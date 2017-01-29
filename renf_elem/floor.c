/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "renf_elem.h"

void renf_elem_floor(fmpz_t a, renf_elem_t b, renf_t nf)
{
    arf_t cl, cr;
    slong prec;

    if (_nf_elem_is_integer(b->elem, nf->nf))
    {
        if (nf_elem_is_zero(b->elem, nf->nf))
            fmpz_zero(a);
        if (nf->nf->flag & NF_LINEAR)
            fmpz_set(a, LNF_ELEM_NUMREF(b->elem));
        else if (nf->nf->flag & NF_QUADRATIC)
            fmpz_set(a, QNF_ELEM_NUMREF(b->elem));
        else
            fmpz_set(a, NF_ELEM_NUMREF(b->elem));
        return;
    }

    arf_init(cl);
    arf_init(cr);
    prec = nf->prec;
    do{
        arb_get_interval_arf(cl, cr, b->emb, prec);
        arf_floor(cl, cl);
        arf_floor(cr, cr);
        if(arf_equal(cl,cr))
        {
            arf_get_fmpz(a, cl, ARF_RND_NEAR);
            arf_clear(cl);
            arf_clear(cr);
            return;
        }
        prec *= 2;
        if(arf_bits(arb_midref(nf->emb)) < prec)
            renf_refine_embedding(nf, 2 * prec);
        if(2 * arf_bits(arb_midref(b->emb)) < prec)
            renf_elem_set_evaluation(b, nf, prec);
    }while (1);
}

