/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "flint.h"
#include "fmpq_poly_extra.h"

int main()
{
	slong iter;

	FLINT_TEST_INIT(state);

	printf("evaluate_arb....");
	fflush(stdout);

	{
		arb_t a,b;
		fmpq_poly_t p;

		arb_init(a);
		arb_init(b);

		fmpq_poly_init(p);

		/* x+1 */
		arb_set_si(a, 1);
		fmpq_poly_set_coeff_si(p, 0, 1);
		fmpq_poly_set_coeff_si(p, 1, 1);
		fmpq_poly_evaluate_arb(b, p, a, 64);
		if(!arb_equal_si(b, 2))
		{
			printf("Error (test_fmpq_poly_evaluate_arb):\n");
			printf("a = "); arb_print(a); printf("\n");
			printf("b = "); arb_print(b); printf("\n");
			printf("p = "); fmpq_poly_print(p); printf("\n");
			abort();
		}

		/* x^2 */
		fmpq_poly_zero(p);
		fmpq_poly_set_coeff_si(p, 2, 1);
		arb_set_si(a, 2);
		fmpq_poly_evaluate_arb(b, p, a, 64);
		if(!arb_equal_si(b, 4))
		{
			printf("Error (test_fmpq_poly_evaluate_arb):\n");
			printf("a = "); arb_print(a); printf("\n");
			printf("b = "); arb_print(b); printf("\n");
			printf("p = "); fmpq_poly_print(p); printf("\n");
			abort();
		}
	}

	for(iter=0; iter < 1000; iter++)
	{
		fmpq_poly_t p;
		fmpq_t x,y;
		arb_t a,b;

		fmpq_poly_init(p);
		fmpq_init(x);
		fmpq_init(y);
		arb_init(a);
		arb_init(b);

		fmpq_poly_randtest(p, state, 1 + n_randint(state,100), 10);
		fmpq_randtest(x, state, 10);
		arb_set_fmpq(a, x, 64);

		fmpq_poly_evaluate_fmpq(y, p, x);
		fmpq_poly_evaluate_arb(b, p, a, 60);

		if(!arb_contains_fmpq(b, y))
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
		if(!arb_equal(a,b))
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

	FLINT_TEST_CLEANUP(state);

	printf("PASS\n");
	return 0;
}
