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

#include <e-antic/renfxx.h>

using namespace eantic;

void check_rational(int num, int den, renf_class& K)
{
    renf_elem_class a(K);
    a = num;
    a /= den;
    if (a.get_num() != num || a.get_den() != den)
    {
        std::cerr << "num = " << num << std::endl;
        std::cerr << "den = " << num << std::endl;
        std::cerr << "a   = " << a << std::endl;
        throw std::runtime_error("num pb");
    }
}

void check_reconstruct(renf_class& K, renf_elem_class& a)
{
    renf_elem_class g = K.gen();
    renf_elem_class gg = 1;
    renf_elem_class b = 0;

    std::cerr << "bx = " << b << std::endl;
    std::vector<mpz_class> num = a.get_num_vector();
    if (num.size() != K.degree())
        throw std::runtime_error("wrong vector length");
    for (std::vector<mpz_class>::iterator it = num.begin(); it < num.end(); it++)
    {
        b += gg * (*it);
        gg *= g;
        std::cerr << "*it = " << *it << std::endl;
        std::cerr << "bx = " << b << std::endl;
    }
    b /= a.get_den();
    std::cerr << "bx = " << b << std::endl;

    if (a != b)
    {
        std::cerr << "a = " << a << std::endl;
        std::cerr << "b = " << b << std::endl;
        throw std::runtime_error("a != b");
    }

    if (a.is_rational() && a.get_den() * a != a.get_num())
    {
        std::cerr << "a = " << a << std::endl;
        std::cerr << "b = " << b << std::endl;
        throw std::runtime_error("rationality failed");
    }
}

int main(void)
{
    {
        // linear
        renf_class K("x - 2/3", "x", "0.66 +/- 0.1");

        check_rational(-12, 5, K);

        renf_elem_class a(K, 0);
        check_reconstruct(K, a);

        renf_elem_class b(K, "2/3");
        check_reconstruct(K, b);
    }

    {
        // quadratic
        renf_class K("x^2 - 2", "x", "1.41 +/- 0.1");

        check_rational(7, 12, K);

        renf_elem_class a(K, 0);
        check_reconstruct(K, a);

        renf_elem_class b(K, "1/2");
        check_reconstruct(K, b);

        renf_elem_class c(K, "-3/7x + 1");
        check_reconstruct(K, c);
    }

    {
        // cubic
        renf_class K("ZT^3 - 2/5", "ZT", "0.74 +/- 0.1");

        renf_elem_class a(K, 0);
        check_reconstruct(K, a);

        renf_elem_class b(K, "53/22");
        check_reconstruct(K, b);

        renf_elem_class c(K, "222/317 -75/22*ZT");
        check_reconstruct(K, c);

        renf_elem_class d(K, "-23/5 + 17/32 * ZT + 255/37 * ZT^2");
        check_reconstruct(K, d);
    }

    return 0;
}
