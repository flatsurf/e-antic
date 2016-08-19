/*
    Copyright (C) 2015, Elias Tsigaridas
    Copyright (C) 2016, Vincent Delecroix

    This file is part of e-antic.

    The implementation was inspired from the SLV library version 0.5 by Elias
    Tsigaridas (namely the function Descartes_test in the file vca_solver_1.c
    lines 67-125)

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"

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

slong fmpz_poly_num_real_roots_upper_bound(fmpz_poly_t pol)
{
    slong i, i0, k, ret, len;
    fmpz * pol2;

    if (fmpz_poly_is_zero(pol))
    {
        fprintf(stderr, "ERROR (fmpz_poly_positive_roots_upper_bound): zero polynomial\n");
        abort();
    }

    i0 = 0;
    while (fmpz_is_zero(pol->coeffs + i0))
        i0++;
    len = pol->length - i0;
    pol2 = _fmpz_vec_init(len);

    /* zero roots */
    ret = i0;

    /* positive roots */
    _fmpz_vec_set(pol2, pol->coeffs + i0, len);

    k = _fmpz_poly_positive_root_upper_bound_2exp(pol2, len);
    if (k != WORD_MIN)
    {
        _fmpz_poly_scale_2exp(pol2, len, k);
        ret += _fmpz_poly_descartes_bound_0_1(pol2, len, len);
    }

    /* negative roots */
    _fmpz_vec_set(pol2, pol->coeffs + i0, len);
    for (i = 1; i < len; i += 2)
        fmpz_neg(pol2 + i, pol2 + i);

    k = _fmpz_poly_positive_root_upper_bound_2exp(pol2, len);
    if (k != WORD_MIN)
    {
        _fmpz_poly_scale_2exp(pol2, len, k);
        ret += _fmpz_poly_descartes_bound_0_1(pol2, len, len);
    }

    _fmpz_vec_clear(pol2, len);
    return ret;
}

