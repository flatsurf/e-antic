/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "e-antic/renfxx.h"

int main(void)
{
    FLINT_TEST_INIT(state);
    int iter;

    {
        fmpq_t x;

        fmpq_init(x);
        fmpz_set_si(fmpq_numref(x), 3);
        fmpz_set_si(fmpq_denref(x), 2);

        renf_elem_class a(x);

        if (a.floor() != 1)
            throw std::runtime_error("pb with floor");

        fmpz_set_si(fmpq_numref(x), -3);
        fmpz_set_si(fmpq_denref(x), 2);

        a = x;

        if (a.floor() != -2)
            throw std::runtime_error("pb with floor");

        fmpq_clear(x);
    }

    FLINT_TEST_CLEANUP(state);
    return 0;
}

