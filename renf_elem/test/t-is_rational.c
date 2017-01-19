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

void test_field(renf_t nf, flint_rand_t state, mp_bitcnt_t bits)
{
    int iter;

    for (iter = 0; iter < 10; iter++)
    {
        fmpq_t x;
        renf_elem_t a;

        fmpq_init(x);
        fmpq_randtest(x, state, bits);

        renf_elem_init(a, nf);
        renf_elem_set_fmpq(a, x, nf);

        if (!renf_elem_is_rational(a, nf))
        {
            printf("FAIL:\n");
            flint_abort();
        }

        fmpq_clear(x);
        renf_elem_clear(a, nf);
    }

    if (!(nf->nf->flag & NF_LINEAR))
    {
        fmpq_poly_t p;
        renf_elem_t a;

        fmpq_poly_init(p);
        fmpq_poly_set_coeff_si(p, 1, 1);

        renf_elem_init(a, nf);
        renf_elem_set_fmpq_poly(a, p, nf);

        if (renf_elem_is_rational(a, nf))
        {
            printf("FAIL:\n");
            flint_abort();
        }

        fmpq_poly_clear(p);
        renf_elem_clear(a, nf);
    }

}


void test_field1(flint_rand_t state)
{
    fmpq_t k;
    fmpq_poly_t p;
    arb_t emb;
    renf_t nf;

    fmpq_init(k);
    fmpq_poly_init(p);

    /* (1+sqrt(5))/2 vs Fibonacci */
    fmpq_poly_set_coeff_si(p, 2, 1);
    fmpq_poly_set_coeff_si(p, 1, -1);
    fmpq_poly_set_coeff_si(p, 0, -1);

    arb_init(emb);
    arb_set_d(emb, 1.61803398874989);
    arb_add_error_2exp_si(emb, -20);
    renf_init(nf, p, emb, 20 + n_randint(state, 100));
    arb_clear(emb);

    test_field(nf, state, 100);
}


void test_field2(flint_rand_t state)
{
    fmpq_t k;
    renf_t nf;

    fmpq_init(k);
    fmpq_randtest_not_zero(k, state, 100);

    renf_init_nth_root_fmpq(nf, k, 2, 20 + n_randint(state, 20));
    test_field(nf, state, 50);

    renf_init_nth_root_fmpq(nf, k, 3, 20 + n_randint(state, 20));
    test_field(nf, state, 50);

    renf_init_nth_root_fmpq(nf, k, 5, 20 + n_randint(state, 20));
    test_field(nf, state, 50);
}

int main()
{
    FLINT_TEST_INIT(state);

    printf("is_rational....");
    fflush(stdout);

    test_field1(state);
    test_field2(state);

    FLINT_TEST_CLEANUP(state);
    printf("PASS\n");
    return 0;
}

