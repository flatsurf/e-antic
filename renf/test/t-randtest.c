/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renf.h>
#include <e-antic/poly_extra.h>

int main()
{
    int iter;
    FLINT_TEST_INIT(state);

    for (iter = 0; iter < 50; iter++)
    {
        slong len = 2 + n_randint(state, 6);
        mp_bitcnt_t bits = 30 + n_randint(state, 30);
        renf_t nf;

        renf_randtest(nf, state, len, bits);
        renf_clear(nf);
    }

    FLINT_TEST_CLEANUP(state);
    return 0;
}
