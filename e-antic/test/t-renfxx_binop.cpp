/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "e-antic/renfxx.h"

int main(void)
{
    int c1 = -1123;
    unsigned int c2 = 2223;
    long c3 = 134;
    unsigned long c4 = 513;
    mpz_class c5(232);
    mpq_class c6(211561);
    int iter;
    FLINT_TEST_INIT(state);

    for (iter=0; iter<100; iter++)
    {
        renf_t nf;
        renf_randtest(nf, state, 5, 50);

        renf_elem_class a(nf);
        renf_elem_class aa(nf);
        renf_elem_gen(a.get_renf_elem(), nf);
        renf_elem_gen(aa.get_renf_elem(), nf);

        renf_elem_class b(1);
        renf_elem_class bb(1);

        std::cout << " a + aa : " << (a + aa) << "\n";
        std::cout << " a - aa : " << (a - aa) << "\n";
        std::cout << " a * aa : " << (a * aa) << "\n";
        std::cout << " a / aa : " << (a / aa) << "\n";
        std::cout << " a + b : " << (a + b) << "\n";
        std::cout << " a - b : " << (a - b) << "\n";
        std::cout << " a * b : " << (a * b) << "\n";
        std::cout << " a / b : " << (a / b) << "\n";
        std::cout << " b + a : " << (b + a) << "\n";
        std::cout << " b - a : " << (b - a) << "\n";
        std::cout << " b * a : " << (b * a) << "\n";
        std::cout << " b / a : " << (b / a) << "\n";
        std::cout << " b + bb : " << (b + bb) << "\n";
        std::cout << " b - bb : " << (b - bb) << "\n";
        std::cout << " b * bb : " << (b * bb) << "\n";
        std::cout << " b / bb : " << (b / bb) << "\n";

        std::cout << " a + int(2) : " << (a + c1) << "\n";
        std::cout << " int(2) + a : " << (c1 + a) << "\n";
        std::cout << " a - int(2) : " << (a - c1) << "\n";
        std::cout << " int(2) - a : " << (c1 - a) << "\n";
        std::cout << " a * int(2) : " << (a * c1) << "\n";
        std::cout << " int(2) * a : " << (c1 * a) << "\n";
        std::cout << " a / int(2) : " << (a / c1) << "\n";
        std::cout << " int(2) / a : " << (c1 / a) << "\n";

        std::cout << " a + uint(2) : " << (a + c2) << "\n";
        std::cout << " uint(2) + a : " << (c2 + a) << "\n";
        std::cout << " a - uint(2) : " << (a - c2) << "\n";
        std::cout << " uint(2) - a : " << (c2 - a) << "\n";
        std::cout << " a * uint(2) : " << (a * c2) << "\n";
        std::cout << " uint(2) * a : " << (c2 * a) << "\n";
        std::cout << " a / uint(2) : " << (a / c2) << "\n";
        std::cout << " uint(2) / a : " << (c2 / a) << "\n";

        std::cout << " a + long(2) : " << (a + c3) << "\n";
        std::cout << " long(2) + a : " << (c3 + a) << "\n";
        std::cout << " a - long(2) : " << (a - c3) << "\n";
        std::cout << " long(2) - a : " << (c3 - a) << "\n";
        std::cout << " a * long(2) : " << (a * c3) << "\n";
        std::cout << " long(2) * a : " << (c3 * a) << "\n";
        std::cout << " a / long(2) : " << (a / c3) << "\n";
        std::cout << " long(2) / a : " << (c3 / a) << "\n";

        std::cout << " a + ulong(2) : " << (a + c4) << "\n";
        std::cout << " ulong(2) + a : " << (c4 + a) << "\n";
        std::cout << " a - ulong(2) : " << (a - c4) << "\n";
        std::cout << " ulong(2) - a : " << (c4 - a) << "\n";
        std::cout << " a * ulong(2) : " << (a * c4) << "\n";
        std::cout << " ulong(2) * a : " << (c4 * a) << "\n";
        std::cout << " a / ulong(2) : " << (a / c4) << "\n";
        std::cout << " ulong(2) / a : " << (c4 / a) << "\n";

        std::cout << " a + mpz(2) : " << (a + c5) << "\n";
        std::cout << " mpz(2) + a : " << (c5 + a) << "\n";
        std::cout << " a - mpz(1) : " << (a - c5) << "\n";
        std::cout << " mpz(2) - a : " << (c5 - a) << "\n";
        std::cout << " a * mpz(2) : " << (a * c5) << "\n";
        std::cout << " mpz(2) * a : " << (c5 * a) << "\n";
        std::cout << " a / mpz(2) : " << (a / c5) << "\n";
        std::cout << " mpz(2) / a : " << (c5 / a) << "\n";

        std::cout << " a + mpq(2) : " << (a + c6) << "\n";
        std::cout << " mpq(2) + a : " << (c6 + a) << "\n";
        std::cout << " a - mpq(1) : " << (a - c6) << "\n";
        std::cout << " mpq(2) - a : " << (c6 - a) << "\n";
        std::cout << " a * mpq(2) : " << (a * c6) << "\n";
        std::cout << " mpq(2) * a : " << (c6 * a) << "\n";
        std::cout << " a / mpq(2) : " << (a / c6) << "\n";
        std::cout << " mpq(2) / a : " << (c6 / a) << "\n";

        renf_clear(nf);
    }

    FLINT_TEST_CLEANUP(state);
    std::cout << "PASS\n";
    return 0;
}


