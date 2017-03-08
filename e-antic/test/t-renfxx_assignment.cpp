/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renfxx.h>

int main(void)
{
    FLINT_TEST_INIT(state);
    int iter;

    for (iter = 0; iter < 100; iter++)
    {
        renf_t nf;
        renf_randtest(nf, state, 5, 50);

        std::cout << "c++ assignment...\n";

        {
            int c = 2;
            renf_elem_class a(nf);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2)
            {
                std::cerr << "pb with int";
                throw 10;
            }
        }

        {
            unsigned int c = 2;
            renf_elem_class a(nf);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2)
            {
                std::cerr << "pb with uint\n";
                throw 10;
            }
        }

        {
            long c = 2;
            renf_elem_class a(nf);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2)
            {
                std::cerr << "pb with long\n";
                throw 10;
            }
        }

        {
            unsigned long c= 2;
            renf_elem_class a(nf);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2)
            {
                std::cerr << "pb with ulong\n";
                throw 10;
            }
        }

        {
            fmpz_t c;
            std::cout << " fmpz_t\n";
            fmpz_init(c);
            fmpz_set_si(c, 2);
            renf_elem_class a(nf);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2)
            {
                std::cerr << "pb with fmpz_t\n";
                throw 10;
            }
            fmpz_clear(c);
        }

        {
            fmpq_t c;

            std::cout << " fmpq_t\n";
            fmpq_init(c);

            fmpq_set_si(c, 2, 1);
            renf_elem_class a(nf);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2)
            {
                std::cerr << "pb with fmpq_t\n";
                throw 10;
            }
            fmpq_clear(c);
        }

        {
            mpz_class c(2);

            renf_elem_class a(nf);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2)
            {
                std::cerr << "pb with mpz_class\n";
                throw 10;
            }
        }

        {
            mpq_class c(2);

            renf_elem_class a(nf);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2)
            {
                std::cerr << "pb with mpq_class\n";
                throw 10;
            }
        }

        {
            renf_elem_class a(nf);
            renf_elem_class b(2);
            a = b;
            if (a != 2)
            {
                std::cerr << "pb with renf_elem\n";
                throw 10;
            }
        }

        {
            renf_elem_class a(nf);
            renf_elem_class b(2);
            b = a;
            if (b != 0)
            {
                std::cerr << "pb with renf_elem\n";
                throw 10;
            }
        }

        renf_clear(nf);
    }

    FLINT_TEST_CLEANUP(state);
    std::cout << "PASS\n";
    return 0;
}

