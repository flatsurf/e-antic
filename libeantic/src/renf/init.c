/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "../e-antic/renf.h"
#include "../e-antic/fmpq_poly_extra.h"

void renf_init(renf_t nf, fmpq_poly_t pol, arb_t emb, slong prec)
{
    const slong len = pol->length;

    if (!fmpq_poly_check_unique_real_root(pol, emb, prec))
    {
        printf("ERROR (renf_init): the given polynomial does not define a unique root\n");
        abort();
    }

    arb_init(nf->emb);
    arb_set(nf->emb, emb);
    nf_init(nf->nf, pol);
    fmpz_poly_init(nf->der);
    fmpz_poly_fit_length(nf->der, len - 1);
    _fmpz_poly_derivative(nf->der->coeffs, fmpq_poly_numref(pol), len);
    _fmpz_poly_set_length(nf->der, len - 1);
    nf->prec = prec;
    nf->immutable = 0;

    if (!(nf->nf->flag & NF_LINEAR))
        renf_refine_embedding(nf, prec);
}
