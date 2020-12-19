/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/renf_elem.h"

static void check_nf_cf(renf_t nf, flint_rand_t state, const mp_bitcnt_t bits, const slong n, const slong num)
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
            printf("a = "); renf_elem_print_pretty(a, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
            printf("do not get enough quotients nn = %ld but n = %ld^n", nn, n);
            abort();
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
                    printf("a = "); renf_elem_print_pretty(a, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
                    printf("remainder should be zero but got b = ");
                    renf_elem_print_pretty(b, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
                    abort();
                }

                s = 0;
            }
            if (ans != s)
            {
                printf("FAIL:\n");
                printf("a = "); renf_elem_print_pretty(a, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
                printf("i = %ld\n", i);
                printf("c = "); fmpq_print(c1); printf("\n");
                printf("got renf_elem_cmp = %d instead of the expected %d\n", ans, s);
                abort();
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

    fmpq_init(d);
    fmpq_set_si(d, 2, 1);

    renf_init_nth_root_fmpq(nf, d, 2, 64);
    check_nf_cf(nf, state, 30, 100, 20);
    renf_clear(nf);

    renf_init_nth_root_fmpq(nf, d, 3, 64);
    check_nf_cf(nf, state, 30, 100, 20);
    renf_clear(nf);

    renf_randtest(nf, state, 2 + (slong)n_randint(state, 10), 64, 30);
    check_nf_cf(nf, state, 30, 100, 20);
    renf_clear(nf);

    fmpq_clear(d);
    
    FLINT_TEST_CLEANUP(state)
    return 0;
}
