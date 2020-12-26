/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/fmpq_poly_extra.h"

int main(void)
{
    fmpq_poly_t p;
    slong a, b;
    arb_t emb;
    int ans;

    fmpq_poly_init(p);
    arb_init(emb);

    a = 123;
    b = 7;
    fmpq_poly_set_coeff_si(p, 0, -a);
    fmpq_poly_set_coeff_si(p, 1, b);
    arb_set_si(emb, a);
    arb_div_si(emb, emb, b, 64);

    ans = fmpq_poly_check_unique_real_root(p, emb, 64);
    if (ans != 1)
    {
        fprintf(stderr, "error with poly check unique real root (got %d instead of 1)\n", ans);
        abort();
    }

    arb_set_si(emb, 0);
    ans = fmpq_poly_check_unique_real_root(p, emb, 64);
    if (ans != 0)
    {
        fprintf(stderr, "error with poly check unique real root (got %d instead of 0)\n", ans);
        abort();
    }


    fmpq_poly_clear(p);
    arb_clear(emb);

    return 0;
}
