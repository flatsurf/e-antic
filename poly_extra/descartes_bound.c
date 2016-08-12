/* TODO: to be moved within FLINT! */
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

#include "flint.h"
#include "poly_extra.h"
#include "fmpz.h"


/* set pol(X) to pol(2^k X) / content(poly(2^k X)) */
void _fmpz_poly_scale_2exp(fmpz * pol, slong len, ulong k)
{
    ulong p, z;
    slong i;

    if (fmpz_sgn(pol))
        z = fmpz_val2(pol);
    else
        z = ULONG_MAX;

    p = k;
    for (i = 1; i < len; i++, p += k)
    {
        if (fmpz_sgn(pol+i))
        {
            z = FLINT_MIN(z, fmpz_val2(pol + i) + p);
            fmpz_mul_2exp(pol+i, pol+i, p);
        }
    }
    if (z)
    {
        for (i = 0; i < len; i++)
            fmpz_fdiv_q_2exp(pol + i, pol + i, z);
    }
}

/* The function below gives an upper bound for the number of roots of p in (0,1) */
/* It is assumed that neither 0 nor 1 is a root */
slong _fmpz_poly_descartes_bound(fmpz * p, fmpz * q, slong len)
{
    slong V = 0;
    slong i,j;
    int s, t;
    slong deg = len - 1;

    j = deg;
    t = fmpz_sgn(p + deg);

    while ( (j >= 0) && ((fmpz_sgn(p + j) == t) || fmpz_sgn(p + j) == 0))
        j--;

    if ( j < 0 )
        /* all coefficients are non-negative */
        return 0;

    /* if p = a0 + a1 X + a2 X^2 + ... the loop below sets */
    /* q = a0 + (a0+a1)X + (a0+a1+a2)X^2 + ...             */
    fmpz_set(q, p);
    for (j = 0; j <= deg - 1; j++)
    {
        fmpz_set(q + j + 1, p + j + 1);
        fmpz_add(q + j + 1, q + j + 1, q + j);
    }


    s = fmpz_sgn(q + deg);  /* = sign(p(1)) */

    for ( i = 1; i <= deg - 1; i++)
    {
        j = deg - i;
        t = s;

        while ( (j >= 0) && (t == 0) )
        {
            t = fmpz_sgn(q + j);
            j--;
        }

        while ( (j >= 0) && ( (fmpz_sgn(q + j) == t) || (fmpz_sgn(q + j) == 0) ) )
            j--;

        if ( j < 0 )
            /* all coefficients of q are non-negative */
            return V;

        for ( j = 0; j <= deg - i - 1; j++)
            fmpz_add(q + j + 1, q + j + 1, q + j);

        if ( s == 0 )
            s = fmpz_sgn(q + deg - i);
        else
        {
            if ( s == -fmpz_sgn(q + deg - i) )
            {
                V++;
                s = -s;
            }
        }
    }

    if ( s == -fmpz_sgn(q) ) V++;

    return V;
}

slong fmpz_poly_descartes_bound(fmpz_poly_t pol)
{
    slong i0, ret;
    ulong k;
    fmpz *pol2, *buffer;
    slong len = fmpz_poly_length(pol);

    if (fmpz_poly_is_zero(pol))
        return 0;

    i0 = 0;
    while (fmpz_is_zero(pol->coeffs + i0))
        i0++;

    buffer = _fmpz_vec_init(2 * (len - i0));
    pol2 = buffer + len - i0;

    _fmpz_vec_set(pol2, pol->coeffs + i0, len - i0);

    /* we rescale pol2 to pol2(2^k X) in order that all roots belong to (0,1) */
    /* TODO:FIXME do we need to check whether 2^k is a root of pol2? */
    k = _fmpz_poly_positive_root_upper_bound_2exp(pol2, len - i0);
    if (k == -1) return 0;
    _fmpz_poly_scale_2exp(pol2, len - i0, k);

    ret = _fmpz_poly_descartes_bound(pol2, buffer, len - i0);

    _fmpz_vec_clear(buffer, fmpz_poly_length(pol));

    return ret;
}

