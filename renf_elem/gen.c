/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include <e-antic/renf_elem.h>
#include <e-antic/poly_extra.h>

void renf_elem_gen(renf_elem_t a, const renf_t nf)
{
    if (nf->nf->flag & NF_LINEAR)
    {
        renf_elem_one(a, nf);
    }
    else
    {
        fmpq_poly_t p;

        fmpq_poly_init(p);
        fmpq_poly_set_coeff_si(p, 1, 1);
        nf_elem_set_fmpq_poly(a, p, nf);
        fmpq_poly_clear(p);
    }
}

