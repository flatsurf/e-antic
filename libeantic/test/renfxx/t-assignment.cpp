/*
    Copyright (C) 2017 Vincent Delecroix
                  2020 Julian Rüth

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

using namespace eantic;

TEMPLATE_TEST_CASE("Assign renf_elem_class from integers", "[renf_elem_class][operator=]", int, unsigned int, long, unsigned long)
{
    flint_rand_t& state = GENERATE(rands());
    auto& K = GENERATE_REF(take(128, renf_classs(state)));

    renf_elem_class a(K);
    renf_elem_class b(0);
    TestType c = GENERATE(static_cast<TestType>(2), std::numeric_limits<TestType>::min(), std::numeric_limits<TestType>::max());

    a = c;
    b = c;

    REQUIRE(a == c);
    REQUIRE(c == a);
    REQUIRE(b == c);
    REQUIRE(c == b);
}

TEMPLATE_TEST_CASE("Assign renf_elem_class from GMP types", "[renf_elem_class][operator=]", mpz_class, mpq_class)
{
    flint_rand_t& state = GENERATE(rands());
    auto& K = GENERATE_REF(take(128, renf_classs(state)));

    renf_elem_class a(K);
    renf_elem_class b(0);

    SECTION("Small integer values")
    {
        TestType c = 2;
        a = c;
        b = c;
        REQUIRE(a == c);
        REQUIRE(b == c);
    }

    SECTION("Big integer values")
    {
        TestType c("134983749573957838576538601923480397593857698357946");
        a = c;
        b = c;
        REQUIRE(a == c);
        REQUIRE(b == c);
    }

}

TEST_CASE("Assign renf_elem_class from another renf_elem_class", "[renf_elem_class][operator=]")
{
    flint_rand_t& state = GENERATE(rands());
    auto& K = GENERATE_REF(take(128, renf_classs(state)));

    auto a = GENERATE_REF(renf_elem_class(), renf_elem_class(K), K.gen());
    auto b = GENERATE_REF(renf_elem_class(), renf_elem_class(2), renf_elem_class(K), renf_elem_class(K, 2), K.gen());

    a = b;
    REQUIRE(a == b);
    REQUIRE(b == a);
}
