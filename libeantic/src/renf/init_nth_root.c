/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include <assert.h>

#include "../e-antic/renf.h"

void renf_init_nth_root_fmpq(renf_t nf, fmpq_t d, ulong n, slong prec)
{
    fmpq_t p0;
    fmpq_poly_t pol;
    arb_t emb;

    if (fmpz_cmp_si(fmpq_numref(d), 0) < 0)
        abort();

    fmpq_poly_init(pol);
    fmpq_init(p0);
    fmpq_set(p0, d);
    fmpq_neg(p0, p0);
    fmpq_poly_set_coeff_fmpq(pol, 0, p0);
    fmpq_poly_set_coeff_ui(pol, (slong)n, 1);

    arb_init(emb);
    arb_set_fmpq(emb, d, prec);
    arb_root_ui(emb, emb, n, prec);

    renf_init(nf, pol, emb, prec);

    fmpq_clear(p0);
    fmpq_poly_clear(pol);
    arb_clear(emb);
}
