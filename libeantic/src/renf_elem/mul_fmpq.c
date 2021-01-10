/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/renf_elem.h"

static void arb_mul_fmpq(arb_t a, const arb_t b, const fmpq_t c, slong prec)
{
    arb_t x;
    arb_init(x);
    arb_set_fmpq(x, c, prec);
    arb_mul(a, b, x, prec);
    arb_clear(x);
}

void renf_elem_mul_fmpq(renf_elem_t a, const renf_elem_t b, const fmpq_t c, const renf_t nf)
{
    nf_elem_scalar_mul_fmpq(a->elem, b->elem, c, nf->nf);
    arb_mul_fmpq(a->emb, b->emb, c, nf->prec);
}
