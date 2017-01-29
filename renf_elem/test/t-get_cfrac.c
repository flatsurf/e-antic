/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "renf_elem.h"

void check_nf_cf(renf_t nf, flint_rand_t state, mp_bitcnt_t bits, slong n, slong num)
{
    slong iter;

    for(iter = 0; iter < num; iter++)
    {
        renf_elem_t a, b;
        int s = 1;
        int ans;
        slong nn;
        slong i;
        fmpz * quot;
        fmpq_t c0, c1;

        renf_elem_init(a, nf);
        renf_elem_init(b, nf);
        renf_elem_randtest(a, state, bits, nf);

        fmpq_init(c0);
        fmpq_init(c1);
        fmpz_set_si(fmpq_numref(c0), 0); fmpz_set_si(fmpq_numref(c1), 1);
        fmpz_set_si(fmpq_denref(c0), 1); fmpz_set_si(fmpq_denref(c1), 0);

        quot = _fmpz_vec_init(n);
        nn = renf_elem_get_cfrac(quot, b, a, n, nf);
        if (nn != n && ! renf_elem_is_rational(a, nf))
        {
            printf("FAIL:\n");
            printf("a = "); renf_elem_print_pretty(a, nf, "x", 10); printf("\n");
            printf("do not get enough quotients nn = %ld but n = %ld^n", nn, n);
            flint_abort();
        }

        for (i = 0; i < nn; i++)
        {
            fmpz_addmul(fmpq_numref(c0), fmpq_numref(c1), quot + i);
            fmpz_addmul(fmpq_denref(c0), fmpq_denref(c1), quot + i);
            fmpq_swap(c0, c1);
            ans = renf_elem_cmp_fmpq(a, c1, nf);
            if (nn < n && i == nn-1)
            {
                if (!renf_elem_is_zero(b, nf))
                {
                    printf("FAIL:\n");
                    printf("a = "); renf_elem_print_pretty(a, nf, "x", 10); printf("\n");
                    printf("remainder should be zero but got b = ");
                    renf_elem_print_pretty(b, nf, "x", 10); printf("\n");
                    flint_abort();
                }

                s = 0;
            }
            if (ans != s)
            {
                printf("FAIL:\n");
                printf("a = "); renf_elem_print_pretty(a, nf, "x", 10); printf("\n");
                printf("i = %ld\n", i);
                printf("c = "); fmpq_print(c1); printf("\n");
                printf("got renf_elem_cmp = %d instead of the expected %d\n", ans, s);
                flint_abort();
            }
            s *= -1;
        }

        renf_elem_clear(a, nf);
        renf_elem_clear(b, nf);
        _fmpz_vec_clear(quot, n);
        fmpq_clear(c0);
        fmpq_clear(c1);
    }
}

int main()
{
    renf_t nf;
    fmpq_t d;
    FLINT_TEST_INIT(state);

    printf("get_cfrac....");

    fmpq_init(d);
    fmpq_set_si(d, 2, 1);

    renf_init_nth_root_fmpq(nf, d, 2, 64);
    check_nf_cf(nf, state, 30, 100, 20);

    renf_init_nth_root_fmpq(nf, d, 3, 64);
    check_nf_cf(nf, state, 30, 100, 20);

    renf_randtest(nf, state, 2 + n_randint(state, 10), 30);
    check_nf_cf(nf, state, 30, 100, 20);

    renf_clear(nf);

    FLINT_TEST_CLEANUP(state);
    printf("PASS\n");
    return 0;
}
