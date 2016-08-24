/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"

slong _num_roots_quartic_positive_discriminant(fmpz * p)
{
    /* more delicate quartic case */
    fmpz_t d, a;
    slong res = 0;

    fmpz_init(d);
    fmpz_init(a);

    /* P = 8ac - 3b^2 */
    fmpz_mul(d, p + 4, p + 2);
    fmpz_mul_ui(d, d, 8);
    fmpz_mul(a, p + 3, p + 3);
    fmpz_mul_ui(a, a, 3);
    fmpz_sub(d, d, a);

    if (fmpz_cmp_ui(d, 0) < 0)
    {
        /* D = 64 a^3 e - 16 a^2 c^2 + 16 a b^2 c - 16 a^2 b d - 3 b^4 */
        fmpz_mul(d, p + 4, p + 4);
        fmpz_mul(d, d, p + 4);
        fmpz_mul(d, d, p);
        fmpz_mul_ui(d, d, 64);

        fmpz_mul(a, p + 4, p + 4);
        fmpz_mul(a, a, p + 2);
        fmpz_mul(a, a, p + 2);
        fmpz_mul_ui(a, a, 16);
        fmpz_sub(d, d, a);

        fmpz_mul(a, p + 4, p + 3);
        fmpz_mul(a, a, p + 3);
        fmpz_mul(a, a, p + 2);
        fmpz_mul_ui(a, a, 16);
        fmpz_add(d, d, a);

        fmpz_mul(a, p + 4, p + 4);
        fmpz_mul(a, a, p + 3);
        fmpz_mul(a, a, p + 1);
        fmpz_mul_ui(a, a, 16);
        fmpz_sub(d, d, a);

        fmpz_mul(a, p + 3, p + 3);
        fmpz_mul(a, a, p + 3);
        fmpz_mul(a, a, p + 3);
        fmpz_mul_ui(a, a, 3);
        fmpz_sub(d, d, a);

        if (fmpz_cmp_ui(d, 0) < 0)
            res = 4;
        else
            res = 0;
    }

    fmpz_clear(a);
    return res;
}

slong fmpz_poly_num_real_roots(fmpz_poly_t pol)
{
    slong len = pol->length;
    fmpz_t disc;
    int s;

    if (len == 0)
    {
        fprintf(stderr, "ERROR (fmpz_poly_num_real_roots): zero polynomial\n");
        flint_abort();
    }
    else if ((len == 1) || (len == 2))
    {
        /* non-zero constant or linear polynomial */
        return len-1;
    }
    else if ((len == 3) || (len == 4) || (len == 5))
    {
        /* quadratic, cubic, quartic */
        fmpz_init(disc);
        _fmpz_poly_discriminant(disc, pol->coeffs, len);
        s = fmpz_sgn(disc);
        fmpz_clear(disc);

        if (s == 0)
        {
            fprintf(stderr, "ERROR (fmpz_poly_num_real_roots): non squarefree polynomial\n");
            flint_abort();
        }
        else if (s > 0)
        {
            if (len == 5)
                return _num_roots_quartic_positive_discriminant(pol->coeffs);
            else
                return len - 1;
        }
        else
        {
            if (len == 5)
                return 2;
            else
                return len - 3;
        }
    }
    else
        /* default */
        return fmpz_poly_num_real_roots_vca(pol);

    /* should never happen */
    return -1;
}


