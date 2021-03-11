/*
    Copyright (C) 2018 Vincent Delecroix
                  2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/renfxx.h"

#include "../rand_generator.hpp"
#include "../renf_class_generator.hpp"
#include "../renf_elem_class_generator.hpp"

#include "../external/catch2/single_include/catch2/catch.hpp"

using namespace eantic;

template <typename S, typename T>
void check_relop(const S& a, const T& b)
{
    SECTION("Reflexivness")
    {
        REQUIRE(a == a);
        REQUIRE(not (a != a));

        REQUIRE(a <= a);
        REQUIRE(not (a < a));

        REQUIRE(a >= a);
        REQUIRE(not (a > a));
    }

    SECTION("Symmetries")
    {
        REQUIRE((a == b) == (b == a));
        REQUIRE((a != b) == (b != a));
        REQUIRE((a < b) == (b > a));
        REQUIRE((a > b) == (b < a));
        REQUIRE((a <= b) == (b >= a));
        REQUIRE((a <= b) == (b >= a));
    }

    SECTION("Implications")
    {
        if (a == b)
        {
            REQUIRE(a <= b);
            REQUIRE(a >= b);
            REQUIRE(b <= a);
            REQUIRE(b >= a);
            REQUIRE(not (a < b));
            REQUIRE(not (a > b));
            REQUIRE(not (b < a));
            REQUIRE(not (b > a));
            REQUIRE(not (a != b));
        }
        else if (a < b)
        {
            REQUIRE(a <= b);
            REQUIRE(not (a >= b));
            REQUIRE(not (b <= a));
            REQUIRE(b >= a);
            REQUIRE(not (a > b));
            REQUIRE(not (b < a));
            REQUIRE(b > a);
            REQUIRE(a != b);
        }
        else
        {
            
            REQUIRE(not (a <= b));
            REQUIRE(a >= b);
            REQUIRE(b <= a);
            REQUIRE(not (b >= a));
            REQUIRE(not (a < b));
            REQUIRE(a > b);
            REQUIRE(b < a);
            REQUIRE(not (b > a));
            REQUIRE(a != b);
        }
    }
}

TEMPLATE_TEST_CASE("Relational Operators with Integers", "[renf_elem_class]", int, unsigned int, long, unsigned long, long long, unsigned long long)
{
    using T = TestType;

    flint_rand_t& state = GENERATE(rands());
    const auto& K = GENERATE_REF(take(16, renf_classs(state)));
    
    auto a = GENERATE_REF(take(8, renf_elem_classs(state, K)));
    T b = GENERATE(0, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

    check_relop(a, b);
}

TEST_CASE("Relational Operators with mpz", "[renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    const auto& K = GENERATE_REF(take(16, renf_classs(state)));
    
    auto a = GENERATE_REF(take(8, renf_elem_classs(state, K)));
    mpz_class b = GENERATE(0, 1337, -1);

    check_relop(a, b);
}

TEST_CASE("Relational Operators with mpq", "[renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    const auto& K = GENERATE_REF(take(16, renf_classs(state)));
    
    auto a = GENERATE_REF(take(8, renf_elem_classs(state, K)));
    mpq_class b = GENERATE(mpq_class(13, 37), mpq_class(0), mpq_class(-1));;

    check_relop(a, b);
}

TEST_CASE("Relational Operators with renf_elem_class", "[renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    const auto& K = GENERATE_REF(take(16, renf_classs(state)));
    
    auto a = GENERATE_REF(take(8, renf_elem_classs(state, K)));

    SECTION("Identical Operands")
    {
        check_relop(a, a);
    }

    SECTION("Operands in same Field")
    {
        auto b = GENERATE_REF(take(4, renf_elem_classs(state, K)));

        check_relop(a, b);
    }

    SECTION("Operands in Different Fields")
    {
        const auto& L = GENERATE_REF(take(4, renf_classs(state)));
        auto b = GENERATE_REF(L.zero(), L.one());

        check_relop(a, b);
    }
}
