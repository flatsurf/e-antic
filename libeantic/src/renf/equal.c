/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/renf.h"

static __inline__
int nf_equal(const nf_t nf1, const nf_t nf2)
{
    return fmpq_poly_equal(nf1->pol, nf2->pol);
}

int renf_equal(const renf_t nf1, const renf_t nf2)
{
    return nf_equal(nf1->nf, nf2->nf) && arb_overlaps(nf1->emb, nf2->emb);
}
