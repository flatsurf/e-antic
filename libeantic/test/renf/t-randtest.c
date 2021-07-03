/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/renf.h"
#include "../../e-antic/fmpz_poly_extra.h"

int main()
{
    int iter;
    FLINT_TEST_INIT(state);

    for (iter = 0; iter < 200; iter++)
    {
        slong len = 2 + (slong)n_randint(state, 30);
        slong prec = 8 + (slong)n_randint(state, 2048);
        mp_bitcnt_t bits = 10 + n_randint(state, 30);

        slong test;
        renf_t nf;

        renf_randtest(nf, state, len, prec, bits);
        test = labs(_fmpz_vec_max_bits(nf->nf->pol->coeffs,
                nf->nf->pol->length));
        if (test > (slong)bits)
        {
            printf("ERROR: too many bits in numerator, got %lu instead of %lu\n",
                    test, bits);
            return 1;
        }
        test = (slong)fmpz_bits(nf->nf->pol->den);
        if (test > (slong)bits)
        {
            printf("ERROR: too many bits in denominator, got %lu instead of %lu\n",
                    test, bits);
            return 1;
        }

        renf_clear(nf);
    }

    FLINT_TEST_CLEANUP(state)
    return 0;
}
