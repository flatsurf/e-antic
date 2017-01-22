/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "flint.h"
#include "renf_elem.h"

int main()
{
    int iter;
    FLINT_TEST_INIT(state);

    printf("binop....");

    for (iter = 0; iter < 100; iter++)
    {
        renf_t nf;
        renf_elem_t a,b,c;
        slong len = 2 + n_randint(state, 10);
        mp_bitcnt_t bits = 30 + n_randint(state, 100);

        renf_randtest(nf, state, len, bits);

        renf_elem_init(a, nf);
        renf_elem_init(b, nf);
        renf_elem_init(c, nf);

        renf_elem_randtest(a, state, 10 + n_randint(state, 50), nf);
        renf_elem_randtest(b, state, 10 + n_randint(state, 50), nf);

        renf_elem_add(c, a, b, nf);
        renf_elem_check_embedding(c, nf, 1024);
        renf_elem_sub(c, a, b, nf);
        renf_elem_check_embedding(c, nf, 1024);
        renf_elem_mul(c, a, b, nf);
        renf_elem_check_embedding(c, nf, 1024);
        if (!renf_elem_is_zero(b, nf))
        {
            renf_elem_div(c, a, b, nf);
            renf_elem_check_embedding(c, nf, 1024);
        }

        renf_elem_clear(a, nf);
        renf_elem_clear(b, nf);
        renf_elem_clear(c, nf);
        renf_clear(nf);
    }

    FLINT_TEST_CLEANUP(state);

    printf("PASS\n");
    return 0;
}

