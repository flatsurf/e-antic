/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "e-antic/renfxx.h"

#include <limits.h>
#include <stdexcept>

int main(void)
{
    FLINT_TEST_INIT(state);
    int iter;

    for (iter = 0; iter < 10; iter++)
    {
        renf_t nf;
        renf_randtest(nf, state, 5, 50);
        renf_class K(nf);

        renf_clear(nf);

        {
            int c = 2;
            renf_elem_class a(K);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2 || 2 != a || 2 != b)
                throw std::runtime_error("constructor from int is wrong");

            renf_elem_class d((int) INT_MIN);
            renf_elem_class e((int) INT_MAX);
            if (d != (int) INT_MIN || e != (int) INT_MAX)
                throw std::runtime_error("constructor from INT_MIN/INT_MAX is wrong");
        }

        {
            long c = 2;
            renf_elem_class a(K);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2 || 2 != a || 2 != b)
                throw std::runtime_error("constructor from long is wrong");

            renf_elem_class d((long) LONG_MIN);
            renf_elem_class e((long) LONG_MAX);
            if (d != (long) LONG_MIN || e != (long) LONG_MAX)
                throw std::runtime_error("constructor from LONG_MIN/LONG_MAX is wrong");
        }

        {
            fmpz_t c;
            fmpz_init(c);
            fmpz_set_si(c, 2);
            renf_elem_class a(K);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2 || 2 != a || 2 != b)
                throw std::runtime_error("constructor from fmpz is wrong");
            fmpz_clear(c);
        }

        {
            fmpq_t c;

            fmpq_init(c);

            fmpq_set_si(c, 2, 1);
            renf_elem_class a(K);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2 || 2 != a || 2 != b)
                throw std::runtime_error("constructor from fmpq is wrong");
            fmpq_clear(c);
        }

        {
            mpz_class c(2);

            renf_elem_class a(K);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2 || 2 != a || 2 != b)
                throw std::runtime_error("constructor from mpz_class is wrong");
        }

        {
            mpq_class c(2);

            renf_elem_class a(K);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2 || 2 != a || 2 != b)
                throw std::runtime_error("constructor from mpq_class is wrong");
        }

        {
            renf_elem_class a(K);
            renf_elem_class b(2);
            a = b;
            if (a != 2 || 2 != a)
                throw std::runtime_error("constructor from another renf_elem_class is wrong");
        }

        {
            renf_elem_class a(K);
            renf_elem_class b(K);
            b = a;

            if (b != 0 || 0 != b)
                throw std::runtime_error("constructor from another renf_elem_class is wrong");
        }
    }

    FLINT_TEST_CLEANUP(state);
    return 0;
}

