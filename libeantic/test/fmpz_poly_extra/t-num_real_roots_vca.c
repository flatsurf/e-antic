/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of FLINT

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>

#include <flint/fmpz_poly.h>
#include <flint/fmpq_vec.h>

#include "../../e-antic/config.h"

#include "../../e-antic/fmpz_poly_extra.h"

int main(void)
{
    int iter;

    FLINT_TEST_INIT(state);

    printf("num_real_roots_vca....");

    /* call with random nonzero polynomials */
    for (iter = 0; iter < 100; iter++)
    {
        slong k;
        fmpz_poly_t p;

        fmpz_poly_init(p);
        fmpz_poly_randtest_not_zero(p, state, 20, 10 + n_randint(state, 100));
        k = fmpz_poly_num_real_roots_vca(p);
        if (k < 0 || k > fmpz_poly_degree(p))
        {
            printf("ERROR:\n");
            flint_printf("got k in wrong range k = %wd\n", k);
            printf("p = "); fmpz_poly_print(p); printf("\n");
            abort();
        }

        fmpz_poly_clear(p);
    }

    /* we check on products of the form               */
    /*   Prod (X - r_i) x  R                          */
    /* where r_i are rationals and R has no real root */
    for (iter = 0; iter < 200; iter++)
    {
        slong k, n;
        fmpz_poly_t p,q;
        fmpq * vec;

        n = 1 + (slong)n_randint(state, 10);

        vec = _fmpq_vec_init(n);
        _fmpq_vec_randtest_uniq_sorted(vec, state, n, 80);

        fmpz_poly_init(p);
        fmpz_poly_init(q);
        fmpz_poly_product_roots_fmpq_vec(p, vec, n);
        fmpz_poly_randtest_no_real_root(q, state, 1 + (slong)n_randint(state, 5), 80);
        /* note: should we check that q is squarefree? */
        fmpz_poly_mul(p, p, q);

        k = fmpz_poly_num_real_roots_vca(p);
        if (k != n)
        {
            printf("ERROR:\n");
            flint_printf("found k = %wd instead of n = %wd\n", k, n);
            printf("vec = "); _fmpq_vec_print(vec, n); printf("\n");
            printf("p = "); fmpz_poly_print(p); printf("\n");
            abort();
        }

        _fmpq_vec_clear(vec, n);
        fmpz_poly_clear(p);
        fmpz_poly_clear(q);
    }

#ifdef FLINT_TEST_CLEAR
    FLINT_TEST_CLEAR(state)
#else
    // FLINT <3.2.2 calls this macro FLINT_TEST_CLEAR instead of FLINT_TEST_CLEANUP
    FLINT_TEST_CLEANUP(state)
#endif

    printf("PASS\n");
    return 0;
}
