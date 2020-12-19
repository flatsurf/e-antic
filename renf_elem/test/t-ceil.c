/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "../../e-antic/renf_elem.h"

static int check_ceil(renf_elem_t a, renf_t nf, int ans, const char * s)
{
    fmpz_t n;
    int test;

    fmpz_init(n);
    renf_elem_ceil(n, a, nf);
    test = fmpz_cmp_si(n, ans);
    fmpz_clear(n);

    if (test)
    {
        printf("FAIL:\n");
        printf("a = "); renf_elem_print_pretty(a, s, nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
        printf("got n = "); fmpz_print(n); printf(" but expected %d\n", ans);
        abort();
    }
    return 0;
}

 
 
static void test_field1(flint_rand_t state)
{
    /* tests in QQ[sqrt(5)] */
    ulong iter;

    fmpq_t k;
    fmpq_poly_t p;
    arb_t emb;
    renf_t nf;
    renf_elem_t a;

    fmpq_poly_init(p);
    fmpq_poly_set_coeff_si(p, 2, 1);
    fmpq_poly_set_coeff_si(p, 1, -1);
    fmpq_poly_set_coeff_si(p, 0, -1);

    arb_init(emb);
    arb_set_d(emb, 1.61803398874989);
    arb_add_error_2exp_si(emb, -20);
    renf_init(nf, p, emb, 20 + (slong)n_randint(state, 20));
    arb_clear(emb);

    renf_elem_init(a, nf);
    fmpq_init(k);

    /* (1+sqrt(5))/2 vs Fibonacci */
    fmpq_poly_zero(p);
    fmpq_poly_set_coeff_si(p, 1, -1);
    for (iter = 1; iter < 50; iter++)
    {
        fprintf(stderr, "start iter = %ld\n", iter);
        fflush(stderr);
        fmpz_fib_ui(fmpq_numref(k), iter + 1);
        fmpz_fib_ui(fmpq_denref(k), iter);
        fmpq_poly_set_coeff_fmpq(p, 0, k);
        renf_elem_set_fmpq_poly(a, p, nf);
        check_ceil(a, nf, 1 - iter % 2, "sqrt(5)");
        fprintf(stderr, "end\n");
        fflush(stderr);
    }

    renf_elem_clear(a, nf);
    renf_clear(nf);
    fmpq_clear(k);
    fmpq_poly_clear(p);
}


static void test_field2(flint_rand_t state)
{
    /* test in QQ[3^(1/4)] */
    renf_t nf;
    renf_elem_t a;
    fmpq_t d, k;
    fmpq_poly_t p;

    fmpq_init(d);
    fmpq_poly_init(p);

    fmpq_set_si(d, 3, 1);
    renf_init_nth_root_fmpq(nf, d, 4, 10 + (slong)n_randint(state, 10));

    fmpq_clear(d);

    fmpq_init(k);
    renf_elem_init(a, nf);

    /* test rationals */

    /* --> 3^(1/4)  */
    fmpq_poly_set_coeff_si(p, 1, 1);
    renf_elem_set_fmpq_poly(a, p, nf);

    check_ceil(a, nf, 2, "3^(1/4)");

    /* --> 3^(1/4) - p_34 / q_34 */
    /*     ceil = 1              */
    fmpz_set_str(fmpq_numref(k), "3871793620206447926", 10);
    fmpz_set_str(fmpq_denref(k), "2941926960111028069", 10);
    fmpq_neg(k, k);
    fmpq_poly_set_coeff_fmpq(p, 0, k);
    renf_elem_set_fmpq_poly(a, p, nf);

    check_ceil(a, nf, 1, "3^(1/4)");

    /* --> 3^(1/4) - p_35 / q_35 */
    /*     ceil = 0              */
    fmpz_set_str(fmpq_numref(k), "4393442218385055959", 10);
    fmpz_set_str(fmpq_denref(k), "3338294180377262795", 10);
    fmpq_neg(k, k);
    fmpq_poly_set_coeff_fmpq(p, 0, k);
    renf_elem_set_fmpq_poly(a, p, nf);

    check_ceil(a, nf, 0, "3^(1/4)");

    /* --> 3^(1/4) - p_200 / q_200 */
    fmpz_set_str(fmpq_numref(k), "51566086581654990699052199424489069476470199719930170996263916596162993841059250500042162091", 10);
    fmpz_set_str(fmpq_denref(k), "39181752754141206003124111890355840072199542360218864430892618765033598468868752146602163065", 10);
    fmpq_neg(k, k);
    fmpq_poly_set_coeff_fmpq(p, 0, k);
    renf_elem_set_fmpq_poly(a, p, nf);

    check_ceil(a, nf, 1, "3^(1/4)");

    /* --> 3^(1/4) - p_201 / q_201 */
    fmpz_set_str(fmpq_numref(k), "80796322887694335717970676356641716096406222234122724217891106756946083353628876437327250032", 10);
    fmpz_set_str(fmpq_denref(k), "61391929399498685496270115285641595325756438975454257165479021482386018841773493669624721869", 10);
    fmpq_neg(k, k);
    fmpq_poly_set_coeff_fmpq(p, 0, k);
    renf_elem_set_fmpq_poly(a, p, nf);

    check_ceil(a, nf, 0, "3^(1/4)");

    /* */
    fmpz_set_str(fmpq_numref(k), "13231942875843754343234", 10);
    fmpz_set_str(fmpq_denref(k), "14321431341231112121", 10);
    fmpq_poly_set_coeff_fmpq(p, 3, k);
    fmpz_set_str(fmpq_numref(k), "148589873455543948591", 10);
    fmpz_set_str(fmpq_denref(k), "12332111221111", 10);
    fmpq_neg(k, k);
    fmpq_poly_set_coeff_fmpq(p, 2, k);
    fmpz_set_str(fmpq_numref(k), "1233321469998711012392391", 10);
    fmpz_set_str(fmpq_denref(k), "11814121556810191", 10);
    fmpq_poly_set_coeff_fmpq(p, 1, k);
    fmpz_set_str(fmpq_numref(k), "1249152314425433983202991363672458443993964487436329478959287771807457205881969983777233465754608376177969464841", 10);
    fmpz_set_str(fmpq_denref(k), "10720278662399817731713810382544982753044312944075797382817281426908463944866446042500978893159281330135", 10);
    fmpq_neg(k, k);
    fmpq_poly_set_coeff_fmpq(p, 0, k);
    renf_elem_set_fmpq_poly(a, p, nf);

    check_ceil(a, nf, 231, "3^(1/4)");

    renf_elem_clear(a, nf);
    renf_clear(nf);
    fmpq_clear(k);
    fmpq_poly_clear(p);
}

int main()
{
    int iter;
    FLINT_TEST_INIT(state);

    test_field1(state);
    test_field2(state);

    for (iter = 0; iter < 100; iter++)
    {
        renf_t nf;
        renf_elem_t a;
        fmpz_t f;
        arb_t e;

        fmpz_init(f);
        arb_init(e);
        renf_randtest(nf, state,
                          2 + (slong)n_randint(state, 20),   /* length */
                          8 + (slong)n_randint(state, 2408), /* prec   */
                          10 + n_randint(state, 10)   /* bits   */
                          );
        renf_elem_init(a, nf);
        renf_elem_randtest(a, state, 30 + n_randint(state, 10), nf);

        renf_elem_ceil(f, a, nf);

        arb_sub_fmpz(e, a->emb, f, 1024);
        if (arb_is_positive(e))
        {
            printf("FAIL:\n");
            abort();
        }
        arb_add_ui(e, e, 1, 1024);
        if (arb_is_negative(e))
        {
            printf("FAIL:\n");
            abort();
        }

        renf_elem_clear(a, nf);
        renf_clear(nf);
        fmpz_clear(f);
        arb_clear(e);
    }

    FLINT_TEST_CLEANUP(state)

    return 0;
}

