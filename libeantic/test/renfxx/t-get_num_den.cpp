/*
    Copyright (C)      2017 Vincent Delecroix
                  2020-2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <vector>

#include "../../e-antic/renfxx.h"

#include "../external/catch2/single_include/catch2/catch.hpp"

using namespace eantic;

TEST_CASE("Numerator and denominator", "[renf_elem_class][get_num][get_den]")
{
    auto check_rational = [](int num, int den, const renf_class& K)
    {
        renf_elem_class a(K);
        a = num;
        a /= den;
        REQUIRE(a.get_num() == num);
        REQUIRE(a.num() == num);
        REQUIRE(a.get_den() == den);
        REQUIRE(a.den() == den);
    };

    auto check_reconstruct = [](const renf_class& K, renf_elem_class& a)
    {
        renf_elem_class g = K.gen();
        renf_elem_class gg = 1;
        renf_elem_class b = 0;

        std::vector<mpz_class> num = a.get_num_vector();
        REQUIRE(static_cast<slong>(num.size()) == K.degree());

        for (auto n : num)
        {
            b += gg * n;
            gg *= g;
        }
        b /= a.get_den();

        REQUIRE(a == b);
        if (a.is_rational())
            REQUIRE(a.get_den() * a == a.get_num());
    };

    SECTION("A linear field")
    {
        const auto K = renf_class::make("x - 2/3", "x", "0.66 +/- 0.1");

        check_rational(-12, 5, *K);

        renf_elem_class a(*K, 0);
        check_reconstruct(*K, a);

        renf_elem_class b(*K, "2/3");
        check_reconstruct(*K, b);
    }

    SECTION("A quadratic field")
    {
        const auto K = renf_class::make("x^2 - 2", "x", "1.41 +/- 0.1");

        check_rational(7, 12, *K);

        renf_elem_class a(*K, 0);
        check_reconstruct(*K, a);

        renf_elem_class b(*K, "1/2");
        check_reconstruct(*K, b);

        renf_elem_class c(*K, "-3/7x + 1");
        check_reconstruct(*K, c);
    }

    SECTION("A cubic field")
    {
        const auto K = renf_class::make("ZT^3 - 2/5", "ZT", "0.74 +/- 0.1");

        renf_elem_class a(*K, 0);
        check_reconstruct(*K, a);

        renf_elem_class b(*K, "53/22");
        check_reconstruct(*K, b);

        renf_elem_class c(*K, "222/317 -75/22*ZT");
        check_reconstruct(*K, c);

        renf_elem_class d(*K, "-23/5 + 17/32 * ZT + 255/37 * ZT^2");
        check_reconstruct(*K, d);
    }
}
