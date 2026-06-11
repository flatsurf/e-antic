/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>

#include "../../e-antic/config.h"

#include "../../e-antic/fmpz_poly_extra.h"

int main(void)
{
    int iter;
    FLINT_TEST_INIT(state);

    printf("fmpz_poly_num_real_roots_0_1....");

    for (iter = 0; iter < 11; iter++)
    {
        fmpz_poly_t p;
        slong k1, k2;

        fmpz_poly_init(p);

        if (iter < 10) {
          do{
            fmpz_poly_randtest(p, state, 10, 10);
          } while (fmpz_poly_is_zero(p) || !fmpz_poly_is_squarefree(p));
        } else {
          // issue #301
          fmpz_poly_set_coeff_si(p, 0, 52);
          fmpz_poly_set_coeff_si(p, 3, -304);
          fmpz_poly_set_coeff_si(p, 8, -23);
        }

        k1 = fmpz_poly_num_real_roots_0_1_vca(p);
        k2 = fmpz_poly_num_real_roots_0_1_sturm(p);
        if (k1 != k2)
        {
            printf("ERROR:\n");
            printf("vca and Sturm disagree\n");
            printf("p = "); fmpz_poly_print(p); printf("\n");
            flint_printf("(vca) k1 = %wd  (Sturm) k2 = %wd\n", k1, k2);
            abort();
        }

        fmpz_poly_clear(p);
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
