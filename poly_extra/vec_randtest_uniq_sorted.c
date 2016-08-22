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

void _fmpq_vec_randtest_uniq_sorted(fmpq * vec, flint_rand_t state, slong len, mp_bitcnt_t bits)
{
    slong i;
    int do_again;

    _fmpq_vec_randtest(vec, state, len, bits);
    if (len <= 1) return;

    do
    {
        do_again = 0;
        _fmpq_vec_sort(vec, len);
        for (i = 0; i < len - 1; i++)
        {
            if (fmpq_equal(vec + i, vec + i + 1))
            {
                fmpq_randtest(vec + i, state, bits);
                do_again = 1;
            }
        }
    }while(do_again);
}
