/*
    Copyright (C) 2016 Vincent Delecroix
                  2019 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "../../e-antic/renf_elem.h"

static void check_cmp_fmpq(renf_elem_t a, fmpq_t b, renf_t nf, int ans)
{
    int test;

    if ((test = renf_elem_cmp_fmpq(a, b, nf)) != ans)
    {
        printf("FAIL:\n");
        printf("a = "); renf_elem_print_pretty(a, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
        printf("b = "); fmpq_print(b); printf("\n");
        printf("got cmp(a,b) = %d but expected %d", test, ans);
        abort();
    }
}

static void check_cmp(renf_elem_t a, renf_elem_t b, renf_t nf, int ans)
{
    int test;

    if ((test = renf_elem_cmp(a, a, nf)))
    {
        printf("FAIL:\n");
        printf("a = "); renf_elem_print_pretty(a, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
        printf("b = "); renf_elem_print_pretty(b, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
        printf("got cmp(a,a) = %d but expected 0", test);
        abort();
    }

    if ((test = renf_elem_cmp(b, b, nf)))
    {
        printf("FAIL:\n");
        printf("a = "); renf_elem_print_pretty(a, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
        printf("b = "); renf_elem_print_pretty(b, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
        printf("got cmp(b,b) = %d but expected 0", test);
        abort();
    }

    if ((test = renf_elem_cmp(a, b, nf)) != ans)
    {
        printf("FAIL:\n");
        printf("a = "); renf_elem_print_pretty(a, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
        printf("b = "); renf_elem_print_pretty(b, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
        printf("got cmp(a,b) = %d but expected %d", test, ans);
        abort();
    }

    if ((test = renf_elem_cmp(b, a, nf)) != -ans)
    {
        printf("FAIL:\n");
        printf("a = "); renf_elem_print_pretty(a, "s", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
        printf("b = "); renf_elem_print_pretty(b, "s", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
        printf("got cmp(b,a) = %d but expected %d", test, -ans);
        abort();
    }
}

#define TEST_CMP_CLEANUP \
    fmpq_clear(k); \
    fmpq_poly_clear(pol); \
    arb_clear(emb); \
    renf_elem_clear(a, nf); \
    renf_elem_clear(b, nf); \
    renf_clear(nf);

static void test_field1(flint_rand_t state)
{
    /* QQ[sqrt(5)] */

    ulong iter;
    fmpq_t k;
    fmpq_poly_t pol;
    arb_t emb;
    renf_t nf;
    renf_elem_t a,b;

    arb_init(emb);

    fmpq_init(k);
    fmpq_set_si(k, 5, 1);
    renf_init_nth_root_fmpq(nf, k, 2, 20 + (slong)n_randint(state, 10));
    renf_elem_init(a, nf);
    renf_elem_init(b, nf);

    fmpq_poly_init(pol);
    fmpq_set_si(k, 1, 2);
    fmpq_poly_set_coeff_fmpq(pol, 0, k);
    fmpq_poly_set_coeff_fmpq(pol, 1, k);
    renf_elem_set_fmpq_poly(a, pol, nf);

    /* (1+sqrt(5))/2 vs Fibonacci */
    for (iter = 1; iter < 1000; iter++)
    {
        fmpz_fib_ui(fmpq_numref(k), iter + 1);
        fmpz_fib_ui(fmpq_denref(k), iter);
        renf_elem_set_fmpq(b, k, nf);
        check_cmp(a, b, nf, 2 * ((int)iter % 2) - 1);
    }

    TEST_CMP_CLEANUP
}

static void test_field2(flint_rand_t state)
{
    fmpq_t k;
    fmpq_poly_t pol;
    arb_t emb;
    renf_t nf;
    renf_elem_t a,b;

    fmpq_init(k);
    fmpq_poly_init(pol);

    fmpz_set_str(fmpq_numref(k), "355361916403493787907635266176294507670976633897711629475314", 10);
    fmpz_set_str(fmpq_denref(k), "232139863399722073304047576696214553424609044635737989176611", 10);
    fmpq_poly_set_coeff_fmpq(pol, 5, k);

    fmpz_set_str(fmpq_numref(k), "-606705117688398931339756691168336346841931241557553646478195", 10);
    fmpz_set_str(fmpq_denref(k), "210088309277125347349477982044039896561539859222111640609892", 10);
    fmpq_poly_set_coeff_fmpq(pol, 4, k);

    fmpz_set_str(fmpq_numref(k), "-9858152528117886334267041147158261014293628902166184550325", 10);
    fmpz_set_str(fmpq_denref(k), "200124997858834165462615010919755509440316489890219003729313", 10);
    fmpq_poly_set_coeff_fmpq(pol, 3, k);

    fmpz_set_str(fmpq_numref(k), "-1479703748359770987851452031591226321384235124845508972990484", 10);
    fmpz_set_str(fmpq_denref(k), "1043028493679310624035200427449756164211538920312147178214193", 10);
    fmpq_poly_set_coeff_fmpq(pol, 2, k);

    fmpz_set_str(fmpq_numref(k), "104332378878046551655079361121474039789830833342379433962753", 10);
    fmpz_set_str(fmpq_denref(k), "1623320000453868377233771192591733369463031570458966208466", 10);
    fmpq_poly_set_coeff_fmpq(pol, 1, k);

    fmpz_set_str(fmpq_numref(k), "-274491264187928159408705700641916320509396549839436536171575", 10);
    fmpz_set_str(fmpq_denref(k), "177552254550239955860824546718138124440325167071119116647997", 10);
    fmpq_poly_set_coeff_fmpq(pol, 0, k);


    arb_init(emb);
    arb_set_d(emb, 0.02406681384217074);
    arb_add_error_2exp_si(emb, -30);
    renf_init(nf, pol, emb, 20 + (slong)n_randint(state, 10));

    renf_elem_init(a, nf);
    renf_elem_init(b, nf);

    fmpq_poly_zero(pol);
    renf_elem_set_fmpq_poly(a, pol, nf);
    fmpq_poly_zero(pol);
    fmpq_poly_set_coeff_si(pol, 1, 1);
    renf_elem_set_fmpq_poly(b, pol, nf);

    check_cmp(a,b,nf,-1);

    /* 1000*a**2+30200*a**3 = 1.00019... */
    fmpq_one(k);
    renf_elem_set_fmpq(a, k, nf);
    fmpq_poly_zero(pol);
    fmpq_poly_set_coeff_si(pol, 2, 1000);
    fmpq_poly_set_coeff_si(pol, 3, 30200);
    renf_elem_set_fmpq_poly(b, pol, nf);

    check_cmp(a,b,nf,-1);

    /* 1720*a**2 = 0.9962438290... */
    fmpq_poly_zero(pol);
    fmpq_poly_set_coeff_si(pol, 2, 1720);
    renf_elem_set_fmpq_poly(b, pol, nf);

    check_cmp(a,b,nf,1);

    /* p_200 / q_200 */
    fmpz_set_str(fmpq_numref(k), "221655207065935536852143085919187545393247732705488147621", 10);
    fmpz_set_str(fmpq_denref(k), "9209993832982713679383004265379023648794553006393164120912", 10);
    renf_elem_set_fmpq(a, k, nf);

    fmpq_poly_zero(pol);
    fmpq_poly_set_coeff_si(pol, 1, 1);
    renf_elem_set_fmpq_poly(b, pol, nf);

    check_cmp(a, b, nf, -1);

    /* p_201 / q_201 */
    fmpz_set_str(fmpq_numref(k), "5446949138829031961728431244587429624292878396822018421757841049528074920313886923130786866546471390407865703", 10);
    fmpz_set_str(fmpq_denref(k), "226326142486077269509785716612844376571501620403803595766142665256382906174395467715156038314153788698218118952", 10);
    renf_elem_set_fmpq(a, k, nf);

    check_cmp(a,b,nf,1);

    /* p_537 / q_537 */
    fmpz_set_str(fmpq_numref(k), "20838994720018445309077664556953837708231807765847017174748319886797727523383947968545890726982756942159490264817378355018362444221153090144253577811530954386879565522090027733742213035789146181749525006128258253280908331737199831098075281056176346806995804215267891411419", 10);
    fmpz_set_str(fmpq_denref(k), "865880912059227603040872760489068236422863408896201094981820354474957221555082834728872622944139692411349823061625748647839370567541815533481175179908570979414079130778157654192732508344851119989949521472418828042634498540584460073346459790139531848662818637824281245595752", 10);
    renf_elem_set_fmpq(a, k, nf);

    check_cmp(a, b, nf, 1);

    /* comparison with rational zero without coefficients, see
     * https://github.com/flatsurf/e-antic/pull/75 */
    fmpq_set_si(k, 1, 1);
    /* Set a to a small integer that is not stored as an MPZ; the following
     * renf_elem_zero() does not reset its storage but only changes the
     * storage's length, i.e., RENF_ELEM_NUMREF is now invalid. */
    renf_elem_set_si(a, 1, nf);
    renf_elem_zero(a, nf);

    check_cmp_fmpq(a, k, nf, -1);

    fmpq_neg(k, k);

    check_cmp_fmpq(a, k, nf, 1);

    /* comparison with non-fmpq zero without coefficients, see
     * https://github.com/flatsurf/e-antic/pull/75 */
    /* We create a non-zero b with an embedding that contains zero */
    fmpq_set_si(k, 1, 3);
    renf_elem_set_fmpq(b, k, nf);
    renf_elem_sub(b, b, b, nf);
    fmpz_set_str(fmpq_denref(k), "179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137216", 10);
    renf_elem_add_fmpq(b, b, k, nf);

    check_cmp(a, b, nf, -1);

    TEST_CMP_CLEANUP
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
        renf_elem_t a, b;
        fmpq_t x;

        renf_randtest(nf, state,
                2 + (slong)n_randint(state, 10),   /* length */
                8 + (slong)n_randint(state, 2048), /* prec */
                10 + n_randint(state, 5)    /* bits */
                ); 
        renf_elem_init(a, nf);
        renf_elem_init(b, nf);
        fmpq_init(x);

        renf_elem_randtest(a, state, 20 + n_randint(state, 10), nf);

        fmpq_one(x);
        renf_elem_add_fmpq(b, a, x, nf);

        check_cmp(a, b, nf, -1);

        fmpz_set_ui(fmpq_denref(x), 2);
        fmpz_mul_2exp(fmpq_denref(x), fmpq_denref(x), 50);
        renf_elem_add_fmpq(b, a, x, nf);

        if (renf_elem_cmp(a, b, nf) != -1 || renf_elem_cmp(b, a, nf) != 1)
        {
            printf("FAIL:\n");
            printf("a = "); renf_elem_print_pretty(a, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
            printf("b = "); renf_elem_print_pretty(b, "x", nf, 10, EANTIC_STR_ALG & EANTIC_STR_D); printf("\n");
            abort();
        }

        fmpq_clear(x);
        renf_elem_clear(a, nf);
        renf_elem_clear(b, nf);
        renf_clear(nf);
    }

    FLINT_TEST_CLEANUP(state)
    return 0;
}
