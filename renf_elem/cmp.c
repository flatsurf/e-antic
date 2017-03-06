/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include <e-antic/renf_elem.h>

int renf_elem_cmp(renf_elem_t a, renf_elem_t b, renf_t nf)
{
    arb_t difference;
    slong prec;
    int s;

    /* equality? */
    if (nf_elem_equal(a->elem, b->elem, nf->nf)) return 0;

    /* try default precision */
    arb_init(difference);
    arb_sub(difference, a->emb, b->emb, nf->prec);
    if (!arb_contains_zero(difference))
    {
        s = arf_sgn(arb_midref(difference));
        arb_clear(difference);
        return s;
    }

    /* precision doubling up to the current precision */
    prec = 2 * nf->prec;
    while (prec < arf_bits(arb_midref(nf->emb)))
    {
        if (2 * arf_bits(arb_midref(a->emb)) < prec)
            renf_elem_set_evaluation(a, nf, prec);
        if (2 * arf_bits(arb_midref(b->emb)) < prec)
            renf_elem_set_evaluation(b, nf, prec);
        arb_sub(difference, a->emb, b->emb, prec);
        if (!arb_contains_zero(difference))
        {
            s = arf_sgn(arb_midref(difference));
            arb_clear(difference);
            return s;
        }
        prec *= 2;
    }

    /* augment precision ad libitum */
    do{
        renf_refine_embedding(nf, prec);
        renf_elem_set_evaluation(a, nf, prec);
        renf_elem_set_evaluation(b, nf, prec);
        arb_sub(difference, a->emb, b->emb, prec);
        if(!arb_contains_zero(difference))
        {
            s = arf_sgn(arb_midref(difference));
            arb_clear(difference);
            return s;
        }
        prec *= 2;
    }while (1);
}

