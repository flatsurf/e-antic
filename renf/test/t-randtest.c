/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "renf.h"
#include "poly_extra.h"
#include "flint.h"

int main()
{
    int iter;
    FLINT_TEST_INIT(state);

    printf("randtest....");
    fflush(stdout);

    for (iter = 0; iter < 50; iter++)
    {
        slong len = 2 + n_randint(state, 6);
        mp_bitcnt_t bits = 30 + n_randint(state, 100);
        renf_t nf;
        renf_randtest(nf, state, len, bits);
        renf_clear(nf);
    }

    FLINT_TEST_CLEANUP(state);
    printf("PASS\n");
    return 0;
}
