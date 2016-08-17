/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"

void fmpz_randtest_positive(fmpz_t f, flint_rand_t state, mp_bitcnt_t bits)
{
    fmpz_randtest_not_zero(f, state, bits);
    if (fmpz_cmp_ui(f, 0) < 0) fmpz_neg(f, f);
}

void fmpz_randtest_negative(fmpz_t f, flint_rand_t state, mp_bitcnt_t bits)
{
    fmpz_randtest_not_zero(f, state, bits);
    if (fmpz_cmp_ui(f, 0) > 0) fmpz_neg(f, f);
}

void fmpz_poly_randtest_rational_roots(fmpz_poly_t p, flint_rand_t state, mp_bitcnt_t bits, int real_pos_roots, int real_neg_roots, int complex_roots)
{
    fmpz_poly_t q;
    fmpz_t num, num2, den, a;
    slong i;

    fmpz_init(num);
    fmpz_init(num2);
    fmpz_init(den);
    fmpz_init(a);

    fmpz_poly_init(q);

    fmpz_poly_one(p);
    for ( i = 0; i < real_pos_roots; i++)
    {
        fmpz_randtest_negative(num, state, bits);
        fmpz_randtest_positive(den, state, bits);
        fmpz_poly_set_coeff_fmpz(q, 1, den);
        fmpz_poly_set_coeff_fmpz(q, 0, num);
        fmpz_poly_mul(p, p, q);
    }

    for ( i = 0; i < real_neg_roots; i++)
    {
        fmpz_randtest_positive(num, state, bits);
        fmpz_randtest_positive(den, state, bits);
        fmpz_poly_set_coeff_fmpz(q, 1, den);
        fmpz_poly_set_coeff_fmpz(q, 0, num);
        fmpz_poly_mul(p, p, q);
    }

    for( i = 0; i < complex_roots; i++ )
    {
        fmpz_randtest(num, state, bits);
        fmpz_randtest_not_zero(num2, state, bits);
        fmpz_randtest_positive(den, state, bits);

        fmpz_mul(a, den, den);
        fmpz_poly_set_coeff_fmpz(q, 2, a);

        fmpz_mul(a, num, den);
        fmpz_mul_ui(a, a, 2);
        fmpz_poly_set_coeff_fmpz(q, 1, a);

        fmpz_mul(a, num, num);
        fmpz_addmul(a, num2, num2);
        fmpz_poly_set_coeff_fmpz(q, 0, a);
        fmpz_poly_mul(p, p, q);
    }

    fmpz_clear(a);
    fmpz_clear(num);
    fmpz_clear(num2);
    fmpz_clear(den);
    fmpz_poly_clear(q);
}
