/*
    Copyright (C) 2016, Vincent Delecroix

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"

int main()
{
    int iter;
    FLINT_TEST_INIT(state);

    printf("scale_2exp....");
    fflush(stdout);

    for (iter = 0; iter < 1000; iter++)
    {
        fmpz_poly_t f, g;
        slong k;

        k = n_randint(state, 200);

        fmpz_poly_init(f);
        fmpz_poly_init(g);


        fmpz_poly_randtest(f, state, n_randint(state, 100), 200);
        _fmpz_poly_remove_content_2exp(f->coeffs, f->length);
        fmpz_poly_set(g, f);

        _fmpz_poly_scale_2exp(f->coeffs, f->length, k);
        _fmpz_poly_scale_2exp(f->coeffs, f->length, -k);

        if ( !fmpz_poly_equal(f, g) )
        {
            flint_printf("FAIL:\n");
            fmpz_poly_print(f); printf("\n\n");
            printf("ERROR \n");
            abort();
        }


        fmpz_poly_clear(f);
        fmpz_poly_clear(g);
    }

    FLINT_TEST_CLEANUP(state);

    printf("PASS\n");
    return 0;
}
