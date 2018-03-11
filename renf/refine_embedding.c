/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/poly_extra.h>
#include <e-antic/renf.h>

/* TODO: use multiple Newton if possible */

void renf_refine_embedding(renf_t nf, slong prec)
{
    arb_t tmp;
    arb_init(tmp);

    if(!_fmpz_poly_newton_step_arb(tmp,
            fmpq_poly_numref(nf->nf->pol),
            nf->der->coeffs,
            fmpq_poly_length(nf->nf->pol),
            nf->emb,
            prec))
    {
        _fmpz_poly_bisection_step_arb(tmp,
                fmpq_poly_numref(nf->nf->pol),
                fmpq_poly_length(nf->nf->pol),
                nf->emb,
                prec);
    }
    arb_swap(tmp, nf->emb);
    arb_clear(tmp);
}
