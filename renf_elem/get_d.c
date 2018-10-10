/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renf_elem.h>

double renf_elem_get_d(renf_elem_t a, renf_t nf, arf_rnd_t rnd)
{
    slong prec;

    if (arb_rel_error_bits(a->emb) < -53)
        return arf_get_d(arb_midref(a->emb), rnd);

    prec = nf->prec;
    if(2 * arf_bits(arb_midref(a->emb)) < prec)
        renf_elem_set_evaluation(a, nf, prec);

    while (arb_rel_error_bits(a->emb) >= -53)
    {
        prec *= 2;
        renf_refine_embedding(nf, prec);
        renf_elem_set_evaluation(a, nf, prec);
    }

    return arf_get_d(arb_midref(a->emb), rnd);
}

