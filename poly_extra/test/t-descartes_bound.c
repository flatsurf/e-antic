/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "flint.h"
#include "poly_extra.h"

int main()
{
    int iter;
    FLINT_TEST_INIT(state);
    
    /* test polynomials with random integer roots */
    for( iter = 0; iter <= 1000; iter++ )
    {
        int real_pos_roots, real_neg_roots, complex_roots;
        int i;
        int a,b;
        slong bound;
        fmpz_poly_t p,q;

        real_neg_roots = n_randint(state, 50);
        real_pos_roots = n_randint(state, 50);
        complex_roots = n_randint(state, 50);

        if ( !real_neg_roots && !real_pos_roots && !complex_roots )
            continue;

        fmpz_poly_init(p);
        fmpz_poly_init(q);
        fmpz_poly_one(p);
        fmpz_poly_set_coeff_si(q, 1, 1);
        for ( i = 0; i < real_pos_roots; i++)
        {
            fmpz_poly_set_coeff_si(q, 0, -1 - n_randint(state, 100));
            fmpz_poly_mul(p, p, q);
        }
        for ( i = 0; i < real_neg_roots; i++)
        {
            fmpz_poly_set_coeff_si(q, 0, 1 + n_randint(state, 100));
            fmpz_poly_mul(p, p, q);
        }

        fmpz_poly_set_coeff_si(q, 2, 1);
        for( i = 0; i < complex_roots; i++ )
        {
            a = n_randint(state, 100);
            b = 1 + n_randint(state, 100);
            fmpz_poly_set_coeff_si(q, 1, 2*a);
            fmpz_poly_set_coeff_si(q, 0, a*a + b*b);
            fmpz_poly_mul(p, p, q);
        }

#ifdef DEBUG
        printf("p = "); fmpz_poly_print_pretty(p, "x"); printf("\n");
        printf(" pos = %d  neg = %d  complex = %d\n", real_pos_roots, real_neg_roots, complex_roots);
#endif
        bound = fmpz_poly_descartes_bound(p);
#ifdef DEBUG
        flint_printf("bound = %wd\n\n", bound);
#endif

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
    return 0;
}
