/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/renf.h"
#include "../../e-antic/fmpz_poly_extra.h"

#define MAX_DEGREE 20

int main()
{
    int iter;
    FLINT_TEST_INIT(state);

    for (iter = 0; iter < 100; iter++)
    {
        slong i, n;
        fmpz_poly_t p;
        renf nfemb[MAX_DEGREE];

        fmpz_poly_init(p);

        do{
            fmpz_poly_randtest_irreducible(p, state, MAX_DEGREE + 1, 20);
        }while(fmpz_poly_degree(p) < 1);

        n  = renf_set_embeddings_fmpz_poly(nfemb, p, MAX_DEGREE, 64);

        fmpz_poly_clear(p);
        for (i = 0; i < n; i++) renf_clear(nfemb + i);
    }

    FLINT_TEST_CLEANUP(state)

    return 0;
}
