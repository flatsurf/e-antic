/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/fmpz_poly_extra.h"

int main(void)
{
    ulong len;
    mp_bitcnt_t maxbits;
    FLINT_TEST_INIT(state);

    for (len = 1; len < 20; len++)
    {
        fmpz_poly_t p;
        fmpz_poly_init(p);
        maxbits = 2 + n_randint(state, 100);
        fmpz_poly_randtest_irreducible(p, state, (slong)len, maxbits);
        if (fmpz_poly_length(p) > (slong)len)
        {
            fprintf(stderr, "ERROR: too long %lu instead of len=%lu",
                    fmpz_poly_length(p),
                    len);
            fmpz_poly_fprint_pretty(stderr, p, "x");
            fprintf(stderr, "\n");
            return 1;
        }

        if (labs(fmpz_poly_max_bits(p)) > (slong)maxbits)
        {
            fprintf(stderr, "ERROR: too many bits %lu instead of maxbits=%lu\n",
                    labs(fmpz_poly_max_bits(p)),
                    maxbits);
            fmpz_poly_fprint_pretty(stderr, p, "x");
            fprintf(stderr, "\n");
            return 1;
        }

        fmpz_poly_clear(p);
    }

    FLINT_TEST_CLEANUP(state)
    return 0;
}
