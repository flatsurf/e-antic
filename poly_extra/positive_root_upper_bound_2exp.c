/*
    Copyright (C) 2015, Elias Tsigaridas
    Copyright (C) 2016, Vincent Delecroix

    This file is part of FLINT.

    The implementation follows the function slv_poly_root_upper_bound_2exp in
    the library SLV version 0.5 by Elias Tsigaridas (in the file poly_ops.c
    lines 67-125). 

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"

/* Note: in Elias code, the bound of differs by 1 (the code below is more efficient) */
/* TODO: check about the root 2^k */
slong _fmpz_poly_positive_root_upper_bound_2exp(fmpz * pol, slong len)
{
    slong p, i, j;
    slong q1,q2;

    int ad_sgn = fmpz_sgn(pol + len - 1);

    q1 = WORD_MIN;
    for (i = 0; i < len - 1; i++)
    {
        if ((fmpz_sgn(pol + i) == ad_sgn) || (fmpz_sgn(pol + i) == 0)) continue;

        q2 = WORD_MAX;
        for (j = i + 1; j <= len - 1; j++)
        {
            if (fmpz_sgn(pol + j) != ad_sgn) continue;
            p = fmpz_bits(pol + i) - fmpz_bits(pol + j) - 1;
            q2 = FLINT_MIN(q2, p / (j - i));
        }
        q1 = FLINT_MAX(q1, q2);
    }

    if (q1 == WORD_MIN) return WORD_MIN;
    return q1+3;
}

slong fmpz_poly_positive_root_upper_bound_2exp(fmpz_poly_t pol)
{
    slong i0;

    if (fmpz_poly_is_zero(pol)) return 0;

    i0 = 0;
    while (fmpz_is_zero(pol->coeffs + i0)) i0++;

    return _fmpz_poly_positive_root_upper_bound_2exp(
               pol->coeffs + i0,
               fmpz_poly_length(pol) - i0);
}
