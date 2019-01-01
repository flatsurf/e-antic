/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <flint/flint.h>
#include <e-antic/poly_extra.h>

int main(void)
{
    size_t iter;
    FLINT_TEST_INIT(state);

    for (iter = 0; iter < 200; iter++)
    {
        fprintf(stdout, "%d\n", iter);
        slong b, k, k_neg, k_pos, n;
        size_t jmax, j;
        fmpz_poly_t p,q;
        fmpq_t pow2;

        fmpq * vec;
        n = 1 + n_randint(state, 10);

        vec = _fmpq_vec_init(n);
        _fmpq_vec_randtest_uniq_sorted(vec, state, n, 80);

        fmpq_init(pow2);
        fmpz_poly_init(p);
        fmpz_poly_init(q);
        fmpz_poly_product_roots_fmpq_vec(p, vec, n);
        fmpz_poly_randtest_no_real_root(q, state, 1 + n_randint(state, 5), 80);
        fmpz_poly_mul(p, p, q);

        for (j = jmax = 0; j < n; j++)
            if(fmpq_cmp(vec + j, vec + jmax) > 0)
                jmax = j;

        b = fmpz_poly_positive_root_upper_bound_2exp(q);
        fmpq_one(pow2);
        fmpz_mul_2exp(fmpq_numref(pow2), fmpq_numref(pow2), b);
        if (fmpq_cmp(pow2, vec + jmax) < 0)
        {
            fprintf(stderr, "p = "); fmpz_poly_fprint(stderr, p); fprintf(stderr, "\n");
            fprintf(stderr, "b = %d\n", b);
            abort();
        }

        _fmpq_vec_clear(vec, n);
        fmpz_poly_clear(p);
        fmpz_poly_clear(q);
        fmpq_clear(pow2);
    }


    FLINT_TEST_CLEANUP(state);

    return 0;
}
