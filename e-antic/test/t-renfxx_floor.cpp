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

    std::cout << "c++ floor...\n";

    {
        fmpq_t x;

        fmpq_init(x);
        fmpz_set_si(fmpq_numref(x), 3);
        fmpz_set_si(fmpq_denref(x), 2);

        renf_elem_class a(x);

        if (a.floor() != 1)
        {
            std::cerr << "pb with floor\n";
            throw 10;
        }

        fmpz_set_si(fmpq_numref(x), -3);
        fmpz_set_si(fmpq_denref(x), 2);

        a = x;

        if (a.floor() != -2)
        {
            std::cerr << "pb with floor\n";
            throw 10;
        }

        fmpq_clear(x);
    }

    FLINT_TEST_CLEANUP(state);
    std::cout << "PASS\n";
    return 0;
}

