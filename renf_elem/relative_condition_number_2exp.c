/*
    Copyright (C) 2019 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "../e-antic/renf_elem.h"
#include "../e-antic/fmpz_poly_extra.h"

int renf_elem_relative_condition_number_2exp(slong * cond, renf_elem_t a, renf_t nf)
{
    fmpz * p;
    slong len;

    if (nf_elem_is_rational(a->elem, nf->nf))
    {
        *cond = 0;
        return 1;
    }

    if (nf->nf->flag & NF_QUADRATIC)
    {
        p = QNF_ELEM_NUMREF(a->elem);
        len = 2;
    }
    else
    {
        p = NF_ELEM(a->elem)->coeffs;
        len = NF_ELEM(a->elem)->length;
    }

    return _fmpz_poly_relative_condition_number_2exp(cond, p,
                len, nf->emb, FLINT_MAX(nf->prec, 16));
}

