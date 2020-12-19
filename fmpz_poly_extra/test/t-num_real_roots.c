/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <flint/fmpq_vec.h>

#include "../../e-antic/fmpz_poly_extra.h"

int main()
{
    int iter;

    FLINT_TEST_INIT(state);

    /* check with rational roots */
    for (iter = 0; iter < 200; iter++)
    {
        slong k1, k2, k3;
        fmpz_poly_t p;

        fmpz_poly_init(p);

        do{
            fmpz_poly_randtest_not_zero(p, state, 10 + (slong)n_randint(state, 50), 10 + n_randint(state, 100));
        } while (!fmpz_poly_is_squarefree(p));

        k1 = fmpz_poly_num_real_roots_sturm(p);
        k2 = fmpz_poly_num_real_roots_vca(p);
        k3 = fmpz_poly_num_real_roots(p);
        if ((k1 != k2) || (k1 != k3))
        {
            printf("ERROR:\n");
            flint_printf("found k1=%wd, k2=%wd and k3=%wd\n", k1, k2, k3);
            printf("p = "); fmpz_poly_print(p); printf("\n");
            abort();
        }

        fmpz_poly_clear(p);
    }

    FLINT_TEST_CLEANUP(state)

    return 0;
}
