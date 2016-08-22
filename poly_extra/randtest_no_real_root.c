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
    slong i, k;
    fmpz_poly_t q;
    fmpz_t a,b,c;

    fmpz_init(a);
    fmpz_init(b);
    fmpz_init(c);
    fmpz_poly_init(q);
    fmpz_poly_one(p);

    /* we make a product of two types of polynomials:        */
    /*   - second degree poly with negative discriminant     */
    /*   - polynomial of the form a0 + a2 X^2 + a4 X^4 + ... */
    /*     with a0 > 0 and a{2i} >= 0                        */
    while(len > 0)
    {
        k = 1;
        if (len != 1)
            k += n_randint(state, len - 1);

        fmpz_poly_zero(q);
        if (k == 1)
        {
            fmpz_randtest_not_zero(a, state, bits);
            fmpz_randtest_not_zero(c, state, bits);
            if (fmpz_sgn(a) != fmpz_sgn(c))
                fmpz_neg(a, a);

            /* now choose b so that b^2 < 4ac */
            fmpz_randtest(b, state, (fmpz_bits(a) + fmpz_bits(c))/2);

            fmpz_poly_set_coeff_fmpz(q, 2, a);
            fmpz_poly_set_coeff_fmpz(q, 1, b);
            fmpz_poly_set_coeff_fmpz(q, 0, c);
        }
        else
        {
            for (i = 0; i < k; i++)
            {
                if ((i == 0) || (i == (k-1)))
                    fmpz_randtest_not_zero(a, state, bits);
                else
                    fmpz_randtest(a, state, bits);

                if (fmpz_cmp_si(a, 0) == -1) fmpz_neg(a, a);

                fmpz_poly_set_coeff_fmpz(q, 2*i, a);
            }
        }

        fmpz_poly_mul(p, q, q);
        len -= k;
    }

    fmpz_clear(a);
    fmpz_clear(b);
    fmpz_clear(c);
    fmpz_poly_clear(q);
}
