/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/fmpq_poly_extra.h"

int main()
{
    slong iter;

    FLINT_TEST_INIT(state);

    {
        slong k;

        arb_t a,b;
        arf_t c,d;

        fmpq_poly_t p;

        arb_init(a);
        arb_init(b);
        arf_init(c);
        arf_init(d);

        /* x+1 */
        fmpq_poly_init(p);
        fmpq_poly_set_coeff_si(p, 0, 1);
        fmpq_poly_set_coeff_si(p, 1, 1);
        for (iter = 0; iter < 5000; iter++)
        {
            k = (slong)n_randint(state, 10000);
            arb_set_si(a, k);
            fmpq_poly_evaluate_arb(b, p, a, 30 + (slong)n_randint(state, 100));
            if (!arb_equal_si(b, k + 1))
            {
                printf("FAIL (fmpq_poly_evaluate_arb):\n");
                printf("a = "); arb_print(a); printf("\n");
                printf("b = "); arb_print(b); printf("\n");
                printf("p = "); fmpq_poly_print(p); printf("\n");
                abort();
            }
            arf_set_si(c, k);
            fmpq_poly_evaluate_arf(d, p, c, 30 + (slong)n_randint(state, 100));
            if (!arf_equal_si(d, k + 1))
            {
                printf("FAIL (fmpq_poly_evaluate_arf):\n");
                printf("c = "); arf_print(c); printf("\n");
                printf("d = "); arf_print(d); printf("\n");
                printf("p = "); fmpq_poly_print(p); printf("\n");
                abort();
            }
        }

        /* x^2 */
        fmpq_poly_zero(p);
        fmpq_poly_set_coeff_si(p, 2, 1);
        for (iter = 0; iter < 1000; iter++)
        {
            k = (slong)n_randint(state, 10000);

            arb_set_si(a, k);
            fmpq_poly_evaluate_arb(b, p, a, 30 + (slong)n_randint(state, 100));
            if (!arb_equal_si(b, k * k))
            {
                printf("Error (test_fmpq_poly_evaluate_arb):\n");
                printf("a = "); arb_print(a); printf("\n");
                printf("b = "); arb_print(b); printf("\n");
                printf("p = "); fmpq_poly_print(p); printf("\n");
                abort();
            }

            arf_set_si(c, k);
            fmpq_poly_evaluate_arf(d, p, c, 30 + (slong)n_randint(state, 100));
            if (!arf_equal_si(d, k * k))
            {
                printf("Error (test_fmpq_poly_evaluate_arf):\n");
                printf("c = "); arf_print(c); printf("\n");
                printf("d = "); arf_print(d); printf("\n");
                printf("p = "); fmpq_poly_print(p); printf("\n");
                abort();
            }
        }

        fmpq_poly_clear(p);
        arb_clear(a);
        arb_clear(b);
        arf_clear(c);
        arf_clear(d);
    }

    /* check evaluate_arb agains exact evaluate_fmpq */
    for (iter = 0; iter < 1000; iter++)
    {
        fmpq_poly_t p;
        fmpq_t x,y;
        arb_t a,b;

        fmpq_poly_init(p);
        fmpq_init(x);
        fmpq_init(y);
        arb_init(a);
        arb_init(b);

        fmpq_poly_randtest(p, state, 1 + (slong)n_randint(state, 100), 10);
        fmpq_randtest(x, state, 10);
        arb_set_fmpq(a, x, 64);

        fmpq_poly_evaluate_fmpq(y, p, x);
        fmpq_poly_evaluate_arb(b, p, a, 60);

        if (!arb_contains_fmpq(b, y))
        {
            printf("FAIL (y not in b):\n");
            printf("p = "); fmpq_poly_print(p); printf("\n");
            printf("x = "); fmpq_print(x); printf("\n");
            printf("y = "); fmpq_print(y); printf("\n");
            printf("a = "); arb_print(a); printf("\n");
            printf("b = "); arb_print(b); printf("\n");
            abort();
        }

        fmpq_poly_evaluate_arb(a, p, a, 60);
        if (!arb_equal(a,b))
        {
            printf("FAIL (a not equal b):\n");
            printf("p = "); fmpq_poly_print(p); printf("\n");
            printf("x = "); fmpq_print(x); printf("\n");
            printf("y = "); fmpq_print(y); printf("\n");
            printf("a = "); arb_print(a); printf("\n");
            printf("b = "); arb_print(b); printf("\n");
            abort();
        }

        fmpq_poly_clear(p);
        fmpq_clear(x);
        fmpq_clear(y);
        arb_clear(a);
        arb_clear(b);
    }


    /* test aliasing */
    for (iter = 0; iter < 1000; iter++)
    {
        fmpq_poly_t p;

        arb_t a,b;
        arf_t c,d;

        fmpq_poly_init(p);
        arb_init(a);
        arb_init(b);
        arf_init(c);
        arf_init(d);

        fmpq_poly_randtest(p, state, 1 + (slong)n_randint(state, 100), 10);
        arb_randtest(a, state, 60, 10);
        arb_randtest(b, state, 60, 10);
        arf_randtest(c, state, 60, 10);
        arf_randtest(d, state, 60, 10);

        fmpq_poly_evaluate_arb(b, p, a, 60);
        fmpq_poly_evaluate_arb(a, p, a, 60);

        if (!arb_equal(a, b))
        {
            printf("FAIL (a not equal b):\n");
            printf("p = "); fmpq_poly_print(p); printf("\n");
            printf("a = "); arb_print(a); printf("\n");
            printf("b = "); arb_print(b); printf("\n");
            abort();
        }

        fmpq_poly_evaluate_arf(d, p, c, 60);
        fmpq_poly_evaluate_arf(c, p, c, 60);

        if (!arf_equal(c, d))
        {
            printf("FAIL (c not equal d):\n");
            printf("p = "); fmpq_poly_print(p); printf("\n");
            printf("c = "); arf_print(c); printf("\n");
            printf("d = "); arf_print(d); printf("\n");
        }

        fmpq_poly_clear(p);
        arb_clear(a);
        arb_clear(b);
        arf_clear(c);
        arf_clear(d);
    }

    FLINT_TEST_CLEANUP(state)

    return 0;
}
