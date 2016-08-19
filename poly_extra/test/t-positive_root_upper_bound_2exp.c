/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
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

    printf("positive_root_upper_bound_2exp....");
    fflush(stdout);
    
    /* test polynomials with random Gaussian integer roots */
    for (iter = 0; iter <= 1000; iter++)
    {
        int real_roots, complex_roots;
        int i;
        fmpz_t a, b, c, max_root;
        slong bound;
        fmpz_poly_t p,q;

#ifdef DEBUG
        printf("iter = %d\n", iter);
        fflush(stdout);
#endif

        real_roots = n_randint(state, 50);
        complex_roots = n_randint(state, 50);

        fmpz_poly_init(p);
        fmpz_poly_init(q);
        fmpz_init(a);
        fmpz_init(b);
        fmpz_init(c);
        fmpz_init(max_root);

        fmpz_set_si(max_root, -1);

        fmpz_poly_one(p);
        fmpz_poly_set_coeff_si(q, 1, 1);
        for ( i = 0; i < real_roots; i++)
        {
            fmpz_randtest(a, state, 1 + n_randint(state, 100));
            if(fmpz_cmp(max_root, a) < 0)
                fmpz_set(max_root, a);

            fmpz_neg(a, a);
            fmpz_poly_set_coeff_fmpz(q, 0, a);
            fmpz_poly_mul(p, p, q);
        }

        fmpz_poly_set_coeff_si(q, 2, 1);
        for( i = 0; i < complex_roots; i++ )
        {
            fmpz_randtest(a, state, n_randint(state, 100));
            fmpz_randtest(b, state, 1 + n_randint(state, 100));

            fmpz_mul_si(c, a, 2);
            fmpz_poly_set_coeff_fmpz(q, 1, c);  /* 2*a */

            fmpz_mul(c, a, a);
            fmpz_addmul(c, b, b);
            fmpz_poly_set_coeff_fmpz(q, 0, c); /* a*a + b*b */

            fmpz_poly_mul(p, p, q);
        }
#ifdef DEBUG
/*        printf("p = "); fmpz_poly_print(p); printf("\n");*/
        fflush(stdout);
#endif

        bound = fmpz_poly_positive_root_upper_bound_2exp(p);

#ifdef DEBUG
        flint_printf("Descartes bound = %wd\n", bound);
#endif

        if ((bound == WORD_MIN))
        {
           if (fmpz_cmp_si(max_root, 0) > 0)
            {
                printf("FAIL:\n");
                printf("got bound = -1 for p = ");
                fmpz_poly_print(p);
                printf("\n");
                printf("but max_root = "); fmpz_print(max_root); printf("\n");
                abort();
            }
        }
        else
        {
            fmpz_one(a);
            fmpz_mul_2exp(a, a, bound);

            if ( fmpz_cmp(a, max_root) < 0 )
            {
                printf("FAIL:\n");
                printf("p = "); fmpz_poly_print(p); printf("\n");
                printf("max_real_root = "); fmpz_print(max_root);
                flint_printf("   bits(max_real_root) = %wd", fmpz_bits(max_root)); printf("\n");
                flint_printf("got bound = %wd\n", bound);
                abort();
            }
        }

        fmpz_clear(a);
        fmpz_clear(b);
        fmpz_clear(c);
        fmpz_clear(max_root);
        fmpz_poly_clear(p);
        fmpz_poly_clear(q);
    }

    FLINT_TEST_CLEANUP(state);

    printf("PASS\n");
    return 0;
}
