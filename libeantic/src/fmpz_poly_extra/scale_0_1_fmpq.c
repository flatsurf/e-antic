/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-ANTIC

    e-ANTIC is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/fmpz_poly_extra.h"

void _fmpz_poly_scale_0_1_fmpq(fmpz * pol, slong len, fmpq_t a, fmpq_t b)
{
    slong i;
    fmpz_t c, t, p1, q;

    if (len <= 1) return;

    fmpz_init(p1);
    fmpz_init(q);
    fmpz_init(c);
    fmpz_init(t);

    fmpz_lcm(q, fmpq_denref(a), fmpq_denref(b));

    fmpz_gcd(t, fmpq_denref(a), fmpq_denref(b));
    fmpz_mul(p1, fmpq_numref(a), fmpq_denref(b));
    fmpz_divexact(p1, p1, t);
    fmpz_mul(c, fmpq_numref(b), fmpq_denref(a));
    fmpz_divexact(c, c, t);
    fmpz_sub(t, c, p1);

    /* 1 : x -> x / q */
    fmpz_one(c);
    for (i = len - 2; i >= 0; i--)
    {
        fmpz_mul(c, c, q);
        fmpz_mul(pol + i, pol + i, c);
    }
    /* 2 : x -> x + p1 */
    _fmpz_poly_taylor_shift(pol, p1, len);
    /* 3 : x -> (p2 - p1) x */
    fmpz_one(c);
    for (i = 1; i < len; i++)
    {
        fmpz_mul(c, c, t);
        fmpz_mul(pol + i, pol + i, c);
    }

    _fmpz_poly_primitive_part(pol, pol, len);

    fmpz_clear(t);
    fmpz_clear(c);
    fmpz_clear(p1);
    fmpz_clear(q);
}
