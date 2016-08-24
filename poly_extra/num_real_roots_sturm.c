/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-ANTIC

    e-ANTIC is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"

static __inline__
void fmpz_poly_evaluate_at_one(fmpz_t res, fmpz * p, slong len)
{
    slong i;

    fmpz_set(res, p);
    for (i = 1; i < len; i++)
        fmpz_add(res, res, p + i);
}

slong fmpz_poly_num_real_roots_0_1_sturm(fmpz_poly_t pol)
{
    fmpz_poly_t p0, p1;
    slong i;
    ulong d;
    fmpz_t c;
    int s, s0a, s0b, t;

    if (fmpz_poly_is_zero(pol))
    {
        fprintf(stderr, "ERROR (fmpz_poly_num_real_roots_sturm): zero polynomial\n");
        flint_abort();
    }

    fmpz_init(c);
    fmpz_poly_init(p0);
    fmpz_poly_init(p1);

    fmpz_poly_set(p0, pol);
    fmpz_poly_derivative(p1, p0);

    s0a = fmpz_sgn(pol->coeffs);
    fmpz_poly_evaluate_at_one(c, pol->coeffs, pol->length);
    s0b = fmpz_sgn(c);

    t = 0;
    while (!fmpz_poly_is_zero(p1))
    {
        /* sign change at 0 */
        s = fmpz_sgn(p1->coeffs);
        if (s0a == 0)
        {
            t++;
            s0a = s;
        }
        else if (s != s0a)
        {
            t++;
            s0a = s;
        }

        /* sign change at 1 */
        fmpz_poly_evaluate_at_one(c, p1->coeffs, p1->length);
        s = fmpz_sgn(c);
        if (s0b == 0)
            s0b = s;
        else if (s != s0b)
        {
            t--;
            s0b = s;
        }

        fmpz_poly_swap(p0, p1);
        fmpz_poly_pseudo_rem(p1, &d, p1, p0);
        if ((d%2 == 0) || (fmpz_sgn(p0->coeffs + p0->length - 1) == 1))
            fmpz_poly_neg(p1, p1);
        fmpz_poly_content(c, p1);
        if (!fmpz_is_one(c))
        {
            for (i = 0; i < p1->length; i++)
                fmpz_divexact(p1->coeffs + i, p1->coeffs + i, c);
        }
    }

    fmpz_poly_clear(p0);
    fmpz_poly_clear(p1);
    fmpz_clear(c);

    return t;
}

slong fmpz_poly_num_real_roots_interval_fmpq_sturm(fmpz_poly_t pol, fmpq_t a, fmpq_t b)
{
    fmpz_poly_t p0, p1;
    slong i;
    ulong d;
    fmpz_t c;
    fmpq_t q;
    int s, s0a, s0b, t;

    if (fmpz_poly_is_zero(pol))
    {
        fprintf(stderr, "ERROR (fmpz_poly_num_real_roots_sturm): zero polynomial\n");
        flint_abort();
    }

    fmpz_init(c);
    fmpq_init(q);
    fmpz_poly_init(p0);
    fmpz_poly_init(p1);

    fmpz_poly_set(p0, pol);
    fmpz_poly_derivative(p1, p0);

    fmpz_poly_evaluate_fmpq(q, pol, a);
    s0a = fmpq_sgn(q);
    fmpz_poly_evaluate_fmpq(q, pol, b);
    s0b = fmpq_sgn(q);

    t = 0;
    while (!fmpz_poly_is_zero(p1))
    {
        /* sign change at 0 */
        fmpz_poly_evaluate_fmpq(q, p1, a);
        s = fmpq_sgn(q);
        if (s0a == 0)
            s0a = s;
        else if (s != s0a)
        {
            t--;
            s0a = s;
        }

        /* sign change at 1 */
        fmpz_poly_evaluate_fmpq(q, p1, b);
        s = fmpz_sgn(c);
        if (s0b == 0)
            s0b = s;
        else if (s != s0b)
        {
            t++;
            s0b = s;
        }

        fmpz_poly_swap(p0, p1);
        fmpz_poly_pseudo_rem(p1, &d, p1, p0);
        if ((d%2 == 0) || (fmpz_sgn(p0->coeffs + p0->length - 1) == 1))
            fmpz_poly_neg(p1, p1);
        fmpz_poly_content(c, p1);
        if (!fmpz_is_one(c))
        {
            for (i = 0; i < p1->length; i++)
                fmpz_divexact(p1->coeffs + i, p1->coeffs + i, c);
        }
    }

    fmpz_poly_clear(p0);
    fmpz_poly_clear(p1);
    fmpz_clear(c);

    return t;
}



