/*
    Copyright (C) 2015, Elias Tsigaridas
    Copyright (C) 2016, Vincent Delecroix

    This file is part of FLINT.

    The implementation follows the SLV library version 0.5 by Elias Tsigaridas
    (namely the function Descartes_test in the file vca_solver_1.c lines
    67-125)

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"


/* The function below gives an upper bound for the number of roots of p in (0,1) */
/* It is assumed that neither 0 nor 1 is a root                                  */
/* returns either 0, 1, 2, ..., bound or WORD_MAX                                */
slong _fmpz_poly_descartes_bound_0_1(fmpz * p, slong len, slong bound)
{
    slong V = 0;
    slong i,j;
    int s, t;
    slong deg = len - 1;
    fmpz * q;

    j = deg;
    t = fmpz_sgn(p + deg);

    while ((j >= 0) && ((fmpz_sgn(p + j) == t) || fmpz_sgn(p + j) == 0))
        j--;

    if (j < 0)
        /* all coefficients are non-negative */
        return 0;

    q = _fmpz_vec_init(len);
    fmpz_set(q, p);
    for (j = 0; j <= deg - 1; j++)
    {
        fmpz_set(q + j + 1, p + j + 1);
        fmpz_add(q + j + 1, q + j + 1, q + j);
    }


    s = fmpz_sgn(q + deg);  /* = sign(p(1)) */

    for (i = 1; i <= deg - 1; i++)
    {
        j = deg - i;
        t = s;

        while ((j >= 0) && (t == 0))
        {
            t = fmpz_sgn(q + j);
            j--;
        }

        while ((j >= 0) && ((fmpz_sgn(q + j) == t) || (fmpz_sgn(q + j) == 0)))
            j--;

        if (j < 0)
        {
            /* all coefficients of q are non-negative */
            _fmpz_vec_clear(q, len);
            return V;
        }

        for (j = 0; j <= deg - i - 1; j++)
            fmpz_add(q + j + 1, q + j + 1, q + j);

        if (s == 0)
            s = fmpz_sgn(q + deg - i);
        else if (s == -fmpz_sgn(q + deg - i))
        {
            if (V == bound)
            {
                _fmpz_vec_clear(q, len);
                return WORD_MAX;
            }
            V++;
            s = -s;
        }
    }

    if (s == -fmpz_sgn(q))
    {
        if (V == bound) return WORD_MAX;
        V++;
    }

    _fmpz_vec_clear(q, len);

    return V;
}

slong fmpz_poly_descartes_bound(fmpz_poly_t pol)
{
    slong i0, ret;
    slong k;
    slong len = fmpz_poly_length(pol);
    fmpz * pol2;

    if (fmpz_poly_is_zero(pol))
        return 0;

#ifdef DEBUG
    printf("[fmpz_poly_descartes_bound] pol = ");
    fmpz_poly_print_pretty(pol, "x");
    printf("\n");
    fflush(stdout);
#endif

    i0 = 0;
    while (fmpz_is_zero(pol->coeffs + i0))
        i0++;

    pol2 = _fmpz_vec_init(len - i0);

    _fmpz_vec_set(pol2, pol->coeffs + i0, len - i0);

    /* we rescale pol2 to pol2(2^k X) in order that all roots belong to (0,1) */
    /* TODO:FIXME do we need to check whether 2^k is a root of pol2? */
#ifdef DEBUG
    printf("[fmpz_poly_descartes_bound] calling upper bound...");
    fflush(stdout);
#endif
    k = _fmpz_poly_positive_root_upper_bound_2exp(pol2, len - i0);
#ifdef DEBUG
    flint_printf("got k = %wd\n", k);
    fflush(stdout);
#endif
    if (k == WORD_MIN) return 0;
    _fmpz_poly_scale_2exp(pol2, len - i0, k);
#ifdef DEBUG
    printf("scaling done\n");
    fflush(stdout);
#endif

    ret = _fmpz_poly_descartes_bound_0_1(pol2, len - i0, len - 1);

    _fmpz_vec_clear(pol2, len - i0);

    return ret;
}

