/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"
#include "fmpz_poly.h"
#include "fmpq_vec.h"

int main()
{
    int iter;

    FLINT_TEST_INIT(state);

    printf("num_real_roots_sturm....");

    /* check with rational roots */
    for (iter = 0; iter < 500; iter++)
    {
        slong k1, k2, k3, n;
        fmpz_poly_t p,q;
        fmpq * vec;

        n = 1 + n_randint(state, 20);

        vec = _fmpq_vec_init(n);
        _fmpq_vec_randtest_uniq_sorted(vec, state, n, 30);

        fmpz_poly_init(p);
        fmpz_poly_init(q);
        fmpz_poly_set_rational_roots(p, vec, n);
        fmpz_poly_randtest_no_real_root(q, state, n_randint(state, 10), 50);
        fmpz_poly_mul(p, p, q);

        k1 = fmpz_poly_num_real_roots_sturm(p);
        k2 = fmpz_poly_num_real_roots_vca(p);
        k3 = fmpz_poly_num_real_roots(p);
        if ((k1 != n) || (k2 != n) || (k3 != n))
        {
            printf("ERROR:\n");
            flint_printf("found k1=%wd, k2=%wd and k3=%wd instead of %wd\n", k1, k2, k3, n);
            printf("vec = "); _fmpq_vec_print(vec, n); printf("\n");
            printf("p = "); fmpz_poly_print(p); printf("\n");
            abort();
        }

        fmpz_poly_clear(p);
        fmpz_poly_clear(q);
    }

    /* check sturm vs vca vs discriminant on random polynomials */
    for (iter = 0; iter < 1000; iter++)
    {
        slong k1, k2, k3;
        fmpz_poly_t p;

        fmpz_poly_init(p);
        fmpz_poly_randtest_not_zero(p, state, 1 + n_randint(state, 10), 100);
        if (!fmpz_poly_is_squarefree(p)) continue;

        k1 = fmpz_poly_num_real_roots_sturm(p);
        k2 = fmpz_poly_num_real_roots_vca(p);
        k3 = fmpz_poly_num_real_roots(p);
        if ((k1 != k2) || (k1 != k3))
        {
            printf("ERROR:\n");
            flint_printf("Sturm, discriminant and Descartes disagree: k1=%wd, k2=%wd and k3=%wd\n", k1, k2, k3);
            printf("p = "); fmpz_poly_print(p); printf("\n");
            abort();
        }

        fmpz_poly_clear(p);
    }

    FLINT_TEST_CLEANUP(state);

    printf("PASS\n");
    return 0;
}
