/*
    Copyright (C)      2018 Vincent Delecroix
                  2020-2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/renfxx.h"

#include "../external/catch2/single_include/catch2/catch.hpp"

#include "../rand_generator.hpp"
#include "../renf_class_generator.hpp"
#include "../renf_elem_class_generator.hpp"

using namespace eantic;

TEST_CASE("Test is_* Predicates", "[renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());

    SECTION("Elements in the Rational Field")
    {
        auto zero = renf_elem_class();

        REQUIRE(zero.is_zero());
        REQUIRE(!zero.is_one());
        REQUIRE(zero.is_integer());
        REQUIRE(zero.is_rational());

        auto one = renf_elem_class(1);

        REQUIRE(!one.is_zero());
        REQUIRE(one.is_one());
        REQUIRE(one.is_integer());
        REQUIRE(one.is_rational());

        auto gen = renf_class::make().gen();

        REQUIRE(!gen.is_zero());
        REQUIRE(gen.is_one());
        REQUIRE(gen.is_integer());
        REQUIRE(gen.is_rational());

        auto frac = renf_elem_class(1) / 2;

        REQUIRE(!frac.is_zero());
        REQUIRE(!frac.is_one());
        REQUIRE(!frac.is_integer());
        REQUIRE(frac.is_rational());
    }

    SECTION("Specific Elements in Random Number Fields")
    {
        auto& K = GENERATE_REF(take(128, renf_classs(state)));
        CAPTURE(K);

        auto zero = renf_elem_class(K);

        REQUIRE(zero.is_zero());
        REQUIRE(!zero.is_one());
        REQUIRE(zero.is_integer());
        REQUIRE(zero.is_rational());

        auto one = renf_elem_class(K, 1);

        REQUIRE(!one.is_zero());
        REQUIRE(one.is_one());
        REQUIRE(one.is_integer());
        REQUIRE(one.is_rational());

        auto gen = K.gen();

        if (K.degree() != 1)
        {
            REQUIRE(!gen.is_zero());
            REQUIRE(!gen.is_one());
            REQUIRE(!gen.is_integer());
            REQUIRE(!gen.is_rational());
        }

        auto frac = renf_elem_class(K, 1) / 2;

        REQUIRE(!frac.is_zero());
        REQUIRE(!frac.is_one());
        REQUIRE(!frac.is_integer());
        REQUIRE(frac.is_rational());
    }

    SECTION("Random Elements")
    {
        const auto& K = GENERATE_REF(take(16, renf_classs(state)));
        auto a = GENERATE_REF(take(16, renf_elem_classs(state, K)));
        CAPTURE(K, a);

        if (a.is_zero())
        {
            REQUIRE(!a.is_one());
            REQUIRE(a.is_integer());
            REQUIRE(a.is_rational());
        }
        else if (a.is_one())
        {
            REQUIRE(!a.is_zero());
            REQUIRE(a.is_integer());
            REQUIRE(a.is_rational());
        }
        else if (a.is_integer())
        {
            REQUIRE(a.is_rational());
            REQUIRE(static_cast<mpz_class>(a) == a);
        }
        if (a.is_rational())
        {
            REQUIRE(static_cast<mpq_class>(a) == a);
            REQUIRE(static_cast<mpq_class>(a) == a.get_rational());
        }
    }
}
