/*
    Copyright (C) 2017 Vincent Delecroix

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

    printf("squarefree_part....");
    fflush(stdout);

    for (iter = 0; iter < 100; iter++)
    {
        fmpz_poly_t p, p2, q1, q2;

        fmpz_poly_init(p);
        fmpz_poly_init(p2);
        fmpz_poly_init(q1);
        fmpz_poly_init(q2);

        fmpz_poly_randtest(p, state, 1 + (slong)n_randint(state, 20), 10 + n_randint(state, 100));

        fmpz_poly_squarefree_part(q1, p);

        if (fmpz_poly_is_squarefree(p))
        {
            fmpz_poly_primitive_part(q2, p);
            if (!fmpz_poly_equal(q1, q2))
            {
                printf("FAIL:\n");
                printf("q2 = "); fmpz_poly_print(q2); printf("\n");
                printf("q1 = "); fmpz_poly_print(q1); printf("\n");
                fflush(stdout);
                abort();
            }
        }

        fmpz_poly_mul(p2, p, p);
        fmpz_poly_squarefree_part(q2, p2);
        if (!fmpz_poly_equal(q1, q2))
        {
            printf("FAIL:\n");
            printf("p = "); fmpz_poly_print(p); printf("\n");
            printf("q1 = "); fmpz_poly_print(q1); printf("\n");
            printf("q2 = "); fmpz_poly_print(q2); printf("\n");
            abort();
        }

        fmpz_poly_clear(p);
        fmpz_poly_clear(p2);
        fmpz_poly_clear(q1);
        fmpz_poly_clear(q2);
    }


    printf("PASS\n");

    FLINT_TEST_CLEANUP(state)
    return 0;
}
