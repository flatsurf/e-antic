/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/poly_extra.h>

int main(void)
{
    slong i;
    FLINT_TEST_INIT(state);

    for (i = 1; i < 10; i++)
    {
        fmpz_poly_t p;
        fmpz_poly_init(p);
        fmpz_poly_randtest_irreducible(p, state, i, 40);
        fmpz_poly_print_pretty(p, "x");
        printf("\n");
        fmpz_poly_clear(p);
    }

    FLINT_TEST_CLEANUP(state);
}
