/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"
#include "fmpq_vec.h"

int main()
{
    int iter;
    FLINT_INIT_TEST(state);

    printf("randtest_uniq_sorted....");

    for (iter = 0; iter < 100; iter++)
    {
        slong i;
        slong n;
        fmpq * vec;

        n = n_randint(state, 20);
        vec = _vec_init(n);
        _fmpq_vec_randtest_uniq_sorted(vec, state, n, 100);

        for (i = 0; i < n-1; i++)
        {
            if (fmpq_cmp(vec + i + 1, vec + i) != 1)
            {
                printf("ERROR:\n");
                flint_printf("got vec[%wd] = ", i); fmpq_print(vec + i);
                flint_printf(" and vec[%wd] = ", i+1); fmpq_print(vec + i + 1);
                printf("\n");
                abort();
            }
        }

        _vec_clear(vec);
    }

    FLINT_TEST_CLEANUP(state);

    printf("PASS\n");
    return 0;
}
