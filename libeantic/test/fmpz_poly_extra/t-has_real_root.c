/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/fmpz_poly_extra.h"

int main()
{
    int iter;
    FLINT_TEST_INIT(state);

    printf("has_real_root....");
    fflush(stdout);

    /* test on polynomial without roots */
    for (iter = 0; iter < 100; iter++)
    {
        fmpz_poly_t p;

        fmpz_poly_init(p);

        do{
            fmpz_poly_randtest_no_real_root(p, state, 50, 1 + n_randint(state, 100));
        } while (!fmpz_poly_is_squarefree(p));

        if(fmpz_poly_has_real_root(p))
        {
            printf("FAIL:\n");
            printf("polynomial with no real root p = ");
            fmpz_poly_print(p); printf("\n");
            abort();
        }

        fmpz_poly_clear(p);
    }

    for (iter = 0; iter < 100; iter++)
    {
        fmpz_poly_t p;
        int ans;
        slong n,n2;

        fmpz_poly_init(p);

        do{
            fmpz_poly_randtest_not_zero(p, state, 50, 1 + n_randint(state, 100));
        } while(!fmpz_poly_is_squarefree(p));

        n = fmpz_poly_num_real_roots_sturm(p);
        n2 = fmpz_poly_num_real_roots_vca(p);
        ans = fmpz_poly_has_real_root(p);
        if ((n > 0) != ans)
        {
            printf("FAIL:\n");
            printf("p = "); fmpz_poly_print(p); printf("\n");
            flint_printf("n = %wd n2 = %wd\n", n, n2);
            flint_printf("ans = %wd\n", ans);
            abort();
        }
        fmpz_poly_clear(p);
    }


    FLINT_TEST_CLEANUP(state)

    printf("PASS\n");
    return 0;
}
