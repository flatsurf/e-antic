/*
    Copyright (C)      2017 Vincent Delecroix
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
#include <flint/fmpq.h>

using namespace eantic;

TEMPLATE_TEST_CASE("Assign renf_elem_class from integers", "[renf_elem_class]", int, unsigned int, long, unsigned long, long long, unsigned long long)
{
    using T = TestType;

    flint_rand_t& state = GENERATE(rands());
    auto& K = GENERATE_REF(take(128, renf_classs(state)));

    renf_elem_class a(K);
    renf_elem_class b;

    SECTION("Construct from minimum value")
    {
      T value = std::numeric_limits<T>::min();
      a = value;
      b = value;
      REQUIRE(a == value);
      REQUIRE(b == value);
    }

    SECTION("Construct from maximum value")
    {
      T value = std::numeric_limits<T>::max();
      a = value;
      b = value;
      REQUIRE(a == value);
      REQUIRE(b == value);
    }

    SECTION("Construct from small value")
    {
      T value = 2;
      a = value;
      b = value;
      REQUIRE(a == value);
      REQUIRE(b == value);
    }
}

TEST_CASE("Assign renf_elem_class from mpz", "[renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    auto& K = GENERATE_REF(take(128, renf_classs(state)));

    renf_elem_class a(K);
    a = mpz_class(1337);

    REQUIRE(a == 1337);
}

TEST_CASE("Assign renf_elem_class from mpq", "[renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    auto& K = GENERATE_REF(take(128, renf_classs(state)));

    renf_elem_class a(K);
    a = mpq_class(13, 37);

    REQUIRE(a == mpq_class(13, 37));
}

TEST_CASE("Assign renf_elem_class from fmpz", "[renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    auto& K = GENERATE_REF(take(128, renf_classs(state)));

    renf_elem_class a(K);
    {
        fmpz_t value;
        fmpz_init_set_si(value, 1337);
        
        a = value;

        fmpz_clear(value);
    }
    REQUIRE(a == 1337);
}

TEST_CASE("Assign renf_elem_class from fmpq", "[renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    auto& K = GENERATE_REF(take(128, renf_classs(state)));

    renf_elem_class a(K);
    {
        fmpq_t value;
        fmpq_init(value);
        fmpq_set_str(value, "13/37", 10);
        
        a = value;

        fmpq_clear(value);
    }
    REQUIRE(a == mpq_class(13, 37));
}

TEST_CASE("Assign renf_elem_class from another renf_elem_class", "[renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    const auto& K = GENERATE_REF(take(128, renf_classs(state)));

    auto a = GENERATE_REF(renf_elem_class(), renf_elem_class(K), K.gen());
    auto b = GENERATE_REF(renf_elem_class(), renf_elem_class(2), renf_elem_class(K), renf_elem_class(K, 2), K.gen());

    a = b;

    REQUIRE(a == b);
    REQUIRE(b == a);
}

TEST_CASE("Move Assignment", "[renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    const auto& K = GENERATE_REF(take(128, renf_classs(state)));

    auto a = K.gen();
    auto b = renf_elem_class();

    b = std::move(a);
    a = std::move(b);

    b = renf_elem_class(std::move(a));
    a = std::move(b);

    REQUIRE(a == K.gen());
}

