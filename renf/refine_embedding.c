/*
    Copyright (C) 2016 Vincent Delecroix
                  2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <assert.h>
#include <config.h>
#include <err.h>

#include "../e-antic/fmpz_poly_extra.h"
#include "../e-antic/renf.h"

void renf_refine_embedding(renf_t nf, slong prec)
{
    arb_t tmp;
    slong cond;
    slong comp_prec;

    assert(!renf_set_immutable(nf, 1) && "An embedding cannot be safely refined in a multi-threaded environment; you should have refined as needed and called renf_set_immutable() before entering multi-threaded code. Alternatively, you might also be able to use a distinct copy of the renf_t in each thread.");

    _fmpz_poly_relative_condition_number_2exp(&cond,
            nf->nf->pol->coeffs, nf->nf->pol->length,
            nf->emb, FLINT_MAX(nf->prec, 64));
    cond *= nf->nf->pol->length;
    arb_init(tmp);

    comp_prec = 2 * FLINT_ABS(arb_rel_accuracy_bits(nf->emb)) + cond;
    while (arb_rel_accuracy_bits(nf->emb) < prec)
    {
        if(!_fmpz_poly_newton_step_arb(tmp,
                fmpq_poly_numref(nf->nf->pol),
                nf->der->coeffs,
                fmpq_poly_length(nf->nf->pol),
                nf->emb,
                comp_prec))
        {
            if(!_fmpz_poly_bisection_step_arb(tmp,
                    fmpq_poly_numref(nf->nf->pol),
                    fmpq_poly_length(nf->nf->pol),
                    nf->emb,
                    comp_prec))
            {
                comp_prec *= 2;
                continue;
            }
        }
        arb_swap(tmp, nf->emb);

        _fmpz_poly_relative_condition_number_2exp(&cond,
                nf->nf->pol->coeffs, nf->nf->pol->length,
                nf->emb, 32);
        comp_prec = 2 * FLINT_ABS(arb_rel_accuracy_bits(nf->emb)) + cond * nf->nf->pol->length;
    }
    arb_clear(tmp);

    assert(renf_set_immutable(nf, 0) && "An embedding cannot be safely refined in a multi-threaded environment; you should have refined as needed and called renf_set_immutable() before entering multi-threaded code. Alternatively, you might also be able to use a distinct copy of the renf_t in each thread.");
}
