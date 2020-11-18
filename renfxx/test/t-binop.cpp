/*
    Copyright (C) 2017 Vincent Delecroix
                  2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/renfxx.h"

#include "../../renf/test/rand_generator.hpp"
#include "renf_class_generator.hpp"
#include "renf_elem_class_generator.hpp"

#include "external/catch2/single_include/catch2/catch.hpp"

using namespace eantic;

static std::shared_ptr<const renf_class> K = nullptr;

TEST_CASE("Arithmetic with renf_elem_class", "[renf_elem_class][binop]")
{
    flint_rand_t& state = GENERATE(rands());
    K = GENERATE_REF(take(16, renf_classs(state)));
    auto a = GENERATE_REF(take(16, renf_elem_classs(state, K)));

    CAPTURE(*K, a);

    SECTION("Addition and Subtraction")
    {
        auto b = GENERATE_REF(take(16, renf_elem_classs(state, K)));

        CAPTURE(b);

        auto c = a + b;

        if (a > 0) REQUIRE(c > b);
        if (a < 0) REQUIRE(c < b);

        c -= a;
        REQUIRE(c == b);

        c -= b;
        REQUIRE(!c);
    }

    SECTION("Multiplication and Division")
    {
        auto b = GENERATE_REF(take(16, renf_elem_classs(state, K)));

        CAPTURE(b);

        auto c = a * b;

        if (a == 0 || b == 0)
        {
            REQUIRE(!c);
        }
        else
        {
            c /= a;

            REQUIRE(c == b);

            c /= b;

            REQUIRE(c == 1);
        }
    }

    SECTION("Build Element as Sum of Terms")
    {
        auto c = K->zero();
        auto g = K->one();

        auto coeffs = a.num_vector();
        for (int i = 0; i < coeffs.size(); i++)
        {
            c += coeffs[i] * g;
            g *= K->gen();
        }

        c /= a.den();

        REQUIRE(a == c);
    }

    SECTION("Floor Divisions")
    {
        auto b = GENERATE_REF(take(16, renf_elem_classs(state, K)));

        CAPTURE(b);

        if (b)
        {
            auto c = a.floordiv(b);

            REQUIRE(c == (a / b).floor());
        }
    }
}
