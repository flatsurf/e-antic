/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "../e-antic/renf_elem.h"
#include "../e-antic/fmpz_poly_extra.h"

void renf_elem_gen(renf_elem_t a, const renf_t nf)
{
    if (nf->nf->flag & NF_LINEAR)
    {
        fmpz_set(LNF_ELEM_NUMREF(a->elem), nf->nf->pol->coeffs + 1);
        fmpz_set(LNF_ELEM_DENREF(a->elem), nf->nf->pol->coeffs);

        fmpz_neg(LNF_ELEM_NUMREF(a->elem), LNF_ELEM_NUMREF(a->elem));

        _fmpq_canonicalise(LNF_ELEM_NUMREF(a->elem), LNF_ELEM_DENREF(a->elem));

        arb_fmpz_div_fmpz(a->emb, LNF_ELEM_NUMREF(a->elem),
                          LNF_ELEM_DENREF(a->elem), nf->prec);
    }

    else
    {
        fmpq_poly_t p;

        fmpq_poly_init(p);
        fmpq_poly_zero(p);
        fmpq_poly_set_coeff_si(p, 1, 1);
        renf_elem_set_fmpq_poly(a, p, nf);
        fmpq_poly_clear(p);
    }
}
