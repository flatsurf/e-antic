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

    for (iter = 0; iter < 100; iter++)
    {
        renf_t nf;
        renf_randtest(nf, state, 10, 64, 10);
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
            unsigned int c = 2;
            renf_elem_class a(K);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2 || 2 != a || 2 != b)
                throw std::runtime_error("constructor from int is wrong");

            renf_elem_class d((unsigned int) UINT_MAX);
            if (d != (unsigned int) UINT_MAX)
                throw std::runtime_error("constructor from UINT_MAX is wrong");
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
            unsigned long c = 2;
            renf_elem_class a(K);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2 || 2 != a || 2 != b)
                throw std::runtime_error("constructor from long is wrong");

            renf_elem_class d((unsigned long) ULONG_MAX);
            if (d != (unsigned long) ULONG_MAX)
                throw std::runtime_error("constructor from ULONG_MAX is wrong");
        }

        {
            mpz_class c(2);

            renf_elem_class a(K);
            renf_elem_class b(0);
            a = c;
            b = c;
            if (a != 2 || b != 2 || 2 != a || 2 != b)
                throw std::runtime_error("constructor from small mpz_class is wrong");
        }
        {
            mpz_class c("134983749573957838576538601923480397593857698357946");

            renf_elem_class a(K);
            renf_elem_class b(0);
            a = c;
            b = c;
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
            mpq_class c("130498349583795687209384039850478694587694856/193482794587695830598130598349851");

            renf_elem_class a(K);
            renf_elem_class b(0);
            a = c;
            b = c;
        }

        {
            renf_elem_class a;
            renf_elem_class b(2);
            a = b;
            if (a != 2 || 2 != a || a != b || b != a)
                throw std::runtime_error("constructor from another renf_elem_class is wrong (1)");
        }

        {
            renf_elem_class a;
            renf_elem_class b(K, 2);
            a = b;
            if (a != 2 || 2 != a || a != b || b != a)
                throw std::runtime_error("constructor from another renf_elem_class is wrong (2)");
        }

        {
            renf_elem_class a(K);
            renf_elem_class b(2);
            a = b;

            if (b != 2 || 2 != b || a != b || b != a)
                throw std::runtime_error("constructor from another renf_elem_class is wrong (2)");
        }

        {
            renf_elem_class a(K);
            renf_elem_class b(K, 2);
            a = b;

            if (b != 2 || 2 != b || a != b || b != a)
                throw std::runtime_error("constructor from another renf_elem_class is wrong (3)");
        }

        {
            if (K.degree() >= 2)
            {
                std::vector<mpz_class> v;
                v.push_back(1);
                v.push_back(1);
                renf_elem_class a(K, v);
                renf_elem_class b(K);
                b = v;

                if (a != K.gen() + 1 || b != K.gen() + 1)
                    throw std::runtime_error("constructor from std::vector<mpz_class> failed");
            }
        }

        {
            if (K.degree() >= 2)
            {
                std::vector<mpq_class> v;
                v.push_back(mpq_class(1,2));
                v.push_back(mpq_class(-2,3));
                renf_elem_class a(K,v);
                renf_elem_class b(K,v);
                b = v;

                if (a != (-2*K.gen()/3 + mpq_class(1,2)) ||
                    b != (-2*K.gen()/3 + mpq_class(1,2)))
                    throw std::runtime_error("constructor from std::vector<mpq_class> failed");
            }
        }

    }

    FLINT_TEST_CLEANUP(state);
    return 0;
}

