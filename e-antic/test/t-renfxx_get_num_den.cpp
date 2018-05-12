/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <exception>
#include <vector>

#include "e-antic/renfxx.h"

void check_reconstruct(renf_class& K, renf_elem_class& a)
{
    renf_elem_class g = K.gen();
    renf_elem_class gg = 1;
    renf_elem_class b = 0;

    std::vector<mpz_class> num = a.get_num_vector();
    for (std::vector<mpz_class>::iterator it = num.begin(); it < num.end(); it++)
    {
        b += gg * (*it);
        gg *= g;
    }
    b /= a.get_den();

    if (a != b)
        throw 10;

    if (a.is_rational() && a.get_den() * a != a.get_num())
        throw 10;
}

int main(void)
{
    {
        // linear
        renf_class K("x - 2/3", "0.66 +/- 0.1");

        renf_elem_class a(K, 0);
        check_reconstruct(K, a);

        renf_elem_class b(K, "2/3");
        check_reconstruct(K, b);
    }

    {
        // quadratic
        renf_class K("x^2 - 2", "1.41 +/- 0.1");

        renf_elem_class a(K, 0);
        check_reconstruct(K, a);

        renf_elem_class b(K, "1/2");
        check_reconstruct(K, b);

        renf_elem_class c(K, "-3/7 2/155");
        check_reconstruct(K, c);
    }

    {
        // cubic
        renf_class K("x^3 - 2/5", "0.74 +/- 0.1");

        renf_elem_class a(K, 0);
        check_reconstruct(K, a);

        renf_elem_class b(K, "53/22");
        check_reconstruct(K, b);

        renf_elem_class c(K, "222/317 -75/22");
        check_reconstruct(K, c);

        renf_elem_class d(K, "-23/5 17/32 255/37 18/7");
        check_reconstruct(K, d);
    }

    return 0;
}
