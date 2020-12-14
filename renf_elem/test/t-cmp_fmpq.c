/*
    Copyright (C) 2017 Vincent Delecroix

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

    for (iter = 0; iter < 200; iter++)
    {
        int iter2;
        renf_t nf;

        slong len = 2 + (slong)n_randint(state, 10);
        slong prec = 8 + (slong)n_randint(state, 1024);
        mp_bitcnt_t bits = 30 + n_randint(state, 30);
        renf_randtest(nf, state, len, prec, bits);

        for (iter2 = 0; iter2 < 100; iter2++)
        {

            renf_elem_t a;
            fmpq_t q1, q2;

            fmpq_init(q1);
            fmpq_init(q2);

            fmpq_randtest(q1, state, 30 + n_randint(state, 30));
            fmpq_randtest(q2, state, 30 + n_randint(state, 30));
            renf_elem_init(a, nf);
            renf_elem_set_fmpq(a, q1, nf);

            if (renf_elem_cmp_fmpq(a, q1, nf) != 0 || renf_elem_cmp_fmpq(a, q2, nf) != fmpq_cmp(q1, q2))
            {
                printf("FAIL:\n");
                printf("a = "); renf_elem_print_pretty(a, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
                printf("q1 = "); fmpq_print(q1); printf("\n");
                printf("q2 = "); fmpq_print(q2); printf("\n");
                abort();
            }

            fmpq_clear(q1);
            fmpq_clear(q2);
            renf_elem_clear(a, nf);
        }

        if (!(nf->nf->flag & NF_LINEAR))
        {
            renf_elem_t a;
            fmpq_poly_t p;

            fmpq_poly_init(p);
            renf_elem_init(a, nf);
            fmpq_poly_set_coeff_si(p, 1, 1);
            renf_elem_set_fmpq_poly(a, p, nf);

            for (iter2 = 0; iter2 < 30; iter2++)
            {
                fmpq_t q;
                fmpq_init(q);
                fmpq_randtest(q, state, 30 + n_randint(state, 30));
                if (!renf_elem_cmp_fmpq(a, q, nf))
                {
                    printf("FAIL:\n");
                    printf("a = "); renf_elem_print_pretty(a, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
                    printf("q = "); fmpq_print(q); printf("\n");
                    abort();
                }
                fmpq_clear(q);
            }

            fmpq_poly_clear(p);
            renf_elem_clear(a, nf);
        }

        renf_clear(nf);
    }

    FLINT_TEST_CLEANUP(state)
    return 0;
}
