/*
    Copyright (C) 2015, Elias Tsigaridas
    Copyright (C) 2016, Vincent Delecroix

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"

/*#define DEBUG*/

void _fmpz_poly_scale_2exp(fmpz * pol, slong len, slong k)
{
    slong i;
    ulong p, z;

#ifdef DEBUG
    flint_printf("[_fmpz_poly_scale_2exp] k = %wd\n", k);
    fflush(stdout);
#endif

    if ((k == 0) || (len <= 1))
        return;
    else if (k > 0)
    {
        if (fmpz_is_zero(pol))
            z = ULONG_MAX;
        else
            z = fmpz_val2(pol);

        p = k;
        for (i = 1; i < len; i++, p += k)
        {
#ifdef DEBUG
            flint_printf("[_fmpz_poly_scale_2exp] p_%wd =", i);
            fmpz_print(pol + i); printf("\n");
            flint_printf("[_fmpz_poly_scale_2exp] p = %wu\n", p);
            fflush(stdout);
#endif
            if (!fmpz_is_zero(pol + i))
            {
                z = FLINT_MIN(z, fmpz_val2(pol + i) + p);
                fmpz_mul_2exp(pol + i, pol + i, p);
            }
        }
    }
    else
    {
        if (fmpz_is_zero(pol + len - 1))
            z = ULONG_MAX;
        else
            z = fmpz_val2(pol + len - 1);

        p = -k;
        for (i = len - 2; i >= 0; i--, p -= k)
        {
#ifdef DEBUG
            flint_printf("[_fmpz_poly_scale_2exp] pol_%wd = ", i);
            fmpz_print(pol + i); printf("\n");
            flint_printf("[_fmpz_poly_scale_2exp] p = %wu\n", p);
            fflush(stdout);
#endif
            if (!fmpz_is_zero(pol + i))
            {
                z = FLINT_MIN(z, fmpz_val2(pol + i) + p);
                fmpz_mul_2exp(pol + i, pol + i, p);
            }
        }
    }

#ifdef DEBUG
    flint_printf("[_fmpz_poly_scale_2exp] z = %wd\n", z);
    fflush(stdout);
    if (z == ULONG_MAX)
    {
        fprintf(stderr, "ERROR (_fmpz_poly_scale_2exp): got z = ULONG_MAX\n");
        abort();
    }
#endif
    if (z)
    {
        for (i = 0; i < len; i++)
            fmpz_fdiv_q_2exp(pol + i, pol + i, z);
    }
#ifdef DEBUG
    flint_printf("[_fmpz_poly_scale_2exp] done\n");
    fflush(stdout);
#endif
}

