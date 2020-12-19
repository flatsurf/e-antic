/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-ANTIC

    e-ANTIC is free software: you can redistribute it and/or modify it under
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

    for (iter = 0; iter < 1000; iter++)
    {
        fmpz_poly_t p1, p2;
        fmpq *v1, *v2;
        fmpq_t a, b, c;
        slong n;
        slong i;

        fmpq_init(a);
        fmpq_init(b);
        fmpq_init(c);
        fmpz_poly_init(p1);
        fmpz_poly_init(p2);

        fmpq_randtest(a, state, 30);
        do{
            fmpq_randtest(b, state, 30);
        }while (fmpq_equal(a, b));

        n = 1 + (slong)n_randint(state, 20);
        
        v1 = _fmpq_vec_init(n);
        v2 = _fmpq_vec_init(n);
        _fmpq_vec_randtest(v1, state, n, 20);
        fmpz_poly_product_roots_fmpq_vec(p1, v1, n);
        fmpz_poly_set(p2, p1);

        _fmpz_poly_scale_0_1_fmpq(p2->coeffs, p2->length, a, b);

        if (fmpz_poly_length(p1) != fmpz_poly_length(p2))
        {
            printf("ERROR:\n");
            printf(" p1 = "); fmpz_poly_print(p1); printf("\n");
            printf(" p2 = "); fmpz_poly_print(p2); printf("\n");
            abort();
        }
        fmpq_sub(c, b, a);
        for (i = 0; i < n; i++)
        {
            fmpq_t r;
            fmpq_init(r);
            fmpq_sub(v2 + i, v1 + i, a);
            fmpq_div(v2 + i, v2 + i, c);
            fmpz_poly_evaluate_fmpq(r, p2, v2 + i);
            if (!fmpq_is_zero(r))
            {
                printf("ERROR:\n");
                printf("p1 = "); fmpz_poly_print(p1); printf("\n");
                printf("p2 = "); fmpz_poly_print(p2); printf("\n");
                printf("v1 = "); _fmpq_vec_print(v1, n); printf("\n");
                printf("v2 = "); _fmpq_vec_print(v2, n); printf("\n");
                printf("a = "); fmpq_print(a); printf("\n");
                printf("b = "); fmpq_print(b); printf("\n");
                printf("r = "); fmpq_print(r); printf("\n");
                abort();
            }
            fmpq_clear(r);
        }

        fmpq_clear(a);
        fmpq_clear(b);
        fmpq_clear(c);
        fmpz_poly_clear(p1);
        fmpz_poly_clear(p2);
        _fmpq_vec_clear(v1, n);
        _fmpq_vec_clear(v2, n);
    }

    FLINT_TEST_CLEANUP(state)

    return 0;
}
