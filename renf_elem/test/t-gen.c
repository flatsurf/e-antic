/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <flint/flint.h>

#include "../../e-antic/renf_elem.h"

int main(void)
{
    slong i, j, iter;
    FLINT_TEST_INIT(state);

    for (iter = 0; iter < 100; iter++)
    {
        fmpq_t d;

        fmpq_init(d);
        fmpq_set_si(d, 2, 3);

        for (i = 2; i < 5; i++)
        {
            renf_t nf;
            renf_elem_t a,b;

            renf_init_nth_root_fmpq(nf, d, (ulong)i, 64);

            renf_elem_init(a, nf);
            renf_elem_init(b, nf);


            renf_elem_gen(a, nf);
            renf_elem_set(b, a, nf);
            for (j = 1; j < i; j++) renf_elem_mul(b, b, a, nf);

            if (renf_elem_cmp_fmpq(b, d, nf))
            {
                printf("FAIL:\n");
                printf("d = "); fmpq_print(d); printf("\n");
                printf("a = "); renf_elem_print_pretty(a, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
                printf("b = "); renf_elem_print_pretty(a, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
                abort();
            }

            renf_elem_clear(a, nf);
            renf_elem_clear(b, nf);

            renf_clear(nf);
        }

        fmpq_clear(d);
    }

    FLINT_TEST_CLEANUP(state)
    printf("PASS\n");
    return 0;
}

