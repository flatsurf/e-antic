/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"

void fmpz_poly_randtest_no_real_root(fmpz_poly_t p, flint_rand_t state, slong len, mp_bitcnt_t bits)
{
    slong i;
    fmpz_poly_t q;
    fmpz_t a,b,c;

    fmpz_init(a);
    fmpz_init(b);
    fmpz_init(c);
    fmpz_poly_init(q);
    fmpz_poly_one(p);
    for (i = 0; i < len; i++)
    {
        fmpz_randtest(a, state, bits);
        fmpz_randtest_not_zero(b, state, bits);

        fmpz_poly_set_coeff_si(q, 2, 1);

        fmpz_mul_si(c, a, 2);
        fmpz_neg(c, c);
        fmpz_poly_set_coeff_fmpz(q, 1, c);

        fmpz_mul(c, a, a);
        fmpz_addmul(c, b, b);
        fmpz_poly_set_coeff_fmpz(q, 0, c);

        fmpz_poly_mul(p, q, q);
    }

    fmpz_clear(a);
    fmpz_clear(b);
    fmpz_clear(c);
    fmpz_poly_clear(q);
}

