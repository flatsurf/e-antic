/*
    Copyright (C) 2019 Vincent Delecroix
    Copyright (C) 2019 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "../../e-antic/renf_elem.h"

int main()
{
    int iter;
    FLINT_TEST_INIT(state);

    for (iter = 0; iter < 20; iter++)
    {
        /* operations on two renf elements */
        int i;
        renf_t nf;
        slong len = 2 + (slong)n_randint(state, 10);
        slong prec = 8 + (slong)n_randint(state, 2048);
        mp_bitcnt_t bits = 10 + n_randint(state, 30);

        renf_randtest(nf, state, len, prec, bits);

        for (i = 0; i < 10; i++)
        {
            ulong exp;

            /* renf renf operations */
            renf_elem_t a, b;
            renf_elem_init(a, nf);
            renf_elem_init(b, nf);

            renf_elem_randtest(a, state, 10 + n_randint(state, 30), nf);

            exp = n_randint(state, 20);

            renf_elem_pow(b, a, exp, nf);
            renf_elem_check_embedding(b, nf, 1024);

            renf_elem_clear(a, nf);
            renf_elem_clear(b, nf);
        }

        renf_clear(nf);
    }

    FLINT_TEST_CLEANUP(state)
    return 0;
}
