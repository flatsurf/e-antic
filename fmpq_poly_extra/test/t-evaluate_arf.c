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

	printf("evaluate_arf....");
	fflush(stdout);

	for(iter=0; iter < 1000; iter++)
	{
		fmpq_poly_t p;
		arf_t x,y;

		fmpq_poly_init(p);
		arf_init(x);
		arf_init(y);

		fmpq_poly_randtest(p, state, 1 + n_randint(state,100), 10);
		arf_randtest(x, state, 60, 10);

		fmpq_poly_evaluate_arf(y, p, x, 60);
		fmpq_poly_evaluate_arf(x, p, x, 60);

		if(!arf_equal(x,y))
		{
			printf("FAIL (x not equal y):\n");
			printf("p = "); fmpq_poly_print(p); printf("\n");
			printf("x = "); arf_print(x); printf("\n");
			printf("y = "); arf_print(y); printf("\n");
		}

		fmpq_poly_clear(p);
		arf_clear(x);
		arf_clear(y);
	}

	FLINT_TEST_CLEANUP(state);

	printf("PASS\n");
	return 0;
}
