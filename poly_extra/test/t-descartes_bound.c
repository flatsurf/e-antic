/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "flint.h"
#include "fmpz.h"
#include "poly_extra.h"

int main()
{
    int iter;
    FLINT_TEST_INIT(state);

    printf("descartes_bound....");
    fflush(stdout);
    
    /* test polynomials with random rational roots */
    for( iter = 0; iter <= 1000; iter++ )
    {
        int real_pos_roots, real_neg_roots, complex_roots;
        slong bound;
        fmpz_poly_t p;

        real_neg_roots = n_randint(state, 20);
        real_pos_roots = n_randint(state, 20);
        complex_roots = n_randint(state, 20);

        fmpz_poly_init(p);
        fmpz_poly_randtest_rational_roots(p, state, 50, real_pos_roots, real_neg_roots, complex_roots);

        bound = fmpz_poly_descartes_bound(p);

        if ( real_pos_roots > bound )
        {
            printf("FAIL:\n");
            printf("p = "); fmpz_poly_print(p); printf("\n");
            printf("real_pos_roots = %d\n", real_pos_roots);
            printf("real_neg_roots = %d\n", real_neg_roots);
            printf("complex_roots  = %d\n", complex_roots);
            flint_printf("got bound = %wd\n", bound);
            abort();
        }
    }

    FLINT_TEST_CLEANUP(state);

    printf("PASS\n");
    return 0;
}
