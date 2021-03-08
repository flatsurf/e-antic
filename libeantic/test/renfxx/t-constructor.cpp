/*
    Copyright (C)      2017 Vincent Delecroix
                  2020-2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <limits>
#include <set>

#include "../../e-antic/renfxx.h"

#include "../external/catch2/single_include/catch2/catch.hpp"

#include "../rand_generator.hpp"
#include "../renf_class_generator.hpp"

using namespace eantic;

TEST_CASE("Construct renf_elem_class from integers and rationals", "[renf_elem_class]")
{
    SECTION("Construct from small values")
    {
        REQUIRE(renf_elem_class() == 0);
        REQUIRE(renf_elem_class(1) == 1);
        REQUIRE(renf_elem_class(mpz_class(2)) == 2);
        REQUIRE(renf_elem_class(mpq_class(3)) == 3);
    }

    SECTION("Construct from extreme integers")
    {
        REQUIRE(renf_elem_class(std::numeric_limits<int>::min()) == std::numeric_limits<int>::min());
        REQUIRE(renf_elem_class(std::numeric_limits<int>::max()) == std::numeric_limits<int>::max());
        REQUIRE(renf_elem_class(std::numeric_limits<unsigned int>::min()) == std::numeric_limits<unsigned int>::min());
        REQUIRE(renf_elem_class(std::numeric_limits<unsigned int>::max()) == std::numeric_limits<unsigned int>::max());
        REQUIRE(renf_elem_class(std::numeric_limits<long>::min()) == std::numeric_limits<long>::min());
        REQUIRE(renf_elem_class(std::numeric_limits<long>::max()) == std::numeric_limits<long>::max());
        REQUIRE(renf_elem_class(std::numeric_limits<unsigned long>::min()) == std::numeric_limits<unsigned long>::min());
        REQUIRE(renf_elem_class(std::numeric_limits<unsigned long>::max()) == std::numeric_limits<unsigned long>::max());
        REQUIRE(renf_elem_class(std::numeric_limits<long long>::min()) == std::numeric_limits<long long>::min());
        REQUIRE(renf_elem_class(std::numeric_limits<long long>::max()) == std::numeric_limits<long long>::max());
        REQUIRE(renf_elem_class(std::numeric_limits<unsigned long long>::min()) == std::numeric_limits<unsigned long long>::min());
        REQUIRE(renf_elem_class(std::numeric_limits<unsigned long long>::max()) == std::numeric_limits<unsigned long long>::max());

        REQUIRE((renf_elem_class() = std::numeric_limits<int>::min()) == std::numeric_limits<int>::min());
        REQUIRE((renf_elem_class() = std::numeric_limits<int>::max()) == std::numeric_limits<int>::max());
        REQUIRE((renf_elem_class() = std::numeric_limits<unsigned int>::min()) == std::numeric_limits<unsigned int>::min());
        REQUIRE((renf_elem_class() = std::numeric_limits<unsigned int>::max()) == std::numeric_limits<unsigned int>::max());
        REQUIRE((renf_elem_class() = std::numeric_limits<long>::min()) == std::numeric_limits<long>::min());
        REQUIRE((renf_elem_class() = std::numeric_limits<long>::max()) == std::numeric_limits<long>::max());
        REQUIRE((renf_elem_class() = std::numeric_limits<unsigned long>::min()) == std::numeric_limits<unsigned long>::min());
        REQUIRE((renf_elem_class() = std::numeric_limits<unsigned long>::max()) == std::numeric_limits<unsigned long>::max());
        REQUIRE((renf_elem_class() = std::numeric_limits<long long>::min()) == std::numeric_limits<long long>::min());
        REQUIRE((renf_elem_class() = std::numeric_limits<long long>::max()) == std::numeric_limits<long long>::max());
        REQUIRE((renf_elem_class() = std::numeric_limits<unsigned long long>::min()) == std::numeric_limits<unsigned long long>::min());
        REQUIRE((renf_elem_class() = std::numeric_limits<unsigned long long>::max()) == std::numeric_limits<unsigned long long>::max());
    }

    SECTION("Construct from FLINT integer")
    {
        fmpz_t n;
        fmpz_init(n);
        fmpz_set_str(n, "1337", 10);
        REQUIRE(renf_elem_class(n) == 1337);
        REQUIRE((renf_elem_class() = n) == 1337);
        fmpz_clear(n);
    }

    SECTION("Construct from FLINT rational")
    {
        fmpq_t q;
        fmpq_init(q);
        fmpq_set_str(q, "13/37", 10);
        REQUIRE(renf_elem_class(q) == mpq_class(13, 37));
        REQUIRE((renf_elem_class() = q) == mpq_class(13, 37));
        fmpq_clear(q);
    }

    auto K = renf_class::make("a^2 - 2", "a", "1.41 +/- 0.1", 128);

    SECTION("Construct from small values with a given field")
    {
        REQUIRE(renf_elem_class(*K) == 0);
        REQUIRE(renf_elem_class(*K, 1) == 1);
        REQUIRE(renf_elem_class(*K, mpz_class(2)) == 2);
        REQUIRE(renf_elem_class(*K, mpq_class(3)) == 3);
    }

    SECTION("Construct from extreme integers with a given field")
    {
        REQUIRE(renf_elem_class(*K, std::numeric_limits<int>::min()) == std::numeric_limits<int>::min());
        REQUIRE(renf_elem_class(*K, std::numeric_limits<int>::max()) == std::numeric_limits<int>::max());
        REQUIRE(renf_elem_class(*K, std::numeric_limits<unsigned int>::min()) == std::numeric_limits<unsigned int>::min());
        REQUIRE(renf_elem_class(*K, std::numeric_limits<unsigned int>::max()) == std::numeric_limits<unsigned int>::max());
        REQUIRE(renf_elem_class(*K, std::numeric_limits<long>::min()) == std::numeric_limits<long>::min());
        REQUIRE(renf_elem_class(*K, std::numeric_limits<long>::max()) == std::numeric_limits<long>::max());
        REQUIRE(renf_elem_class(*K, std::numeric_limits<unsigned long>::min()) == std::numeric_limits<unsigned long>::min());
        REQUIRE(renf_elem_class(*K, std::numeric_limits<unsigned long>::max()) == std::numeric_limits<unsigned long>::max());
        REQUIRE(renf_elem_class(*K, std::numeric_limits<long long>::min()) == std::numeric_limits<long long>::min());
        REQUIRE(renf_elem_class(*K, std::numeric_limits<long long>::max()) == std::numeric_limits<long long>::max());
        REQUIRE(renf_elem_class(*K, std::numeric_limits<unsigned long long>::min()) == std::numeric_limits<unsigned long long>::min());
        REQUIRE(renf_elem_class(*K, std::numeric_limits<unsigned long long>::max()) == std::numeric_limits<unsigned long long>::max());
    }

    SECTION("Construct from FLINT rational with a given field")
    {
        fmpq_t q;
        fmpq_init(q);
        fmpq_set_str(q, "13/37", 10);
        REQUIRE(renf_elem_class(*K, q) == mpq_class(13, 37));
        fmpq_clear(q);
    }

    SECTION("Cannot set from a different number field for most values")
    {

      const auto L = renf_class::make("a^2 - 3", "a", "1.7 +/- 1", 128);

      REQUIRE_THROWS(renf_elem_class(*L, K->gen()));

      REQUIRE(renf_elem_class(*L, K->one()) == 1);
      REQUIRE(renf_elem_class(*L, K->zero()) == 0);
    }
}

TEST_CASE("Construct renf_elem_class from string", "[renf_elem_class]")
{
    // operator = for renf_class
    const auto K1 = renf_class::make("a^2 - 2", "a", "1.41 +/- 0.1", 128);
    const auto K2 = K1;
  
    renf_elem_class a(*K1, "a + 1");
    renf_elem_class b(*K2, "a + 1");
  
    REQUIRE(a == b);
}

TEST_CASE("Construct trivial renf_elem_class", "[renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    const auto& K = GENERATE_REF(take(128, renf_classs(state)));

    auto a = renf_elem_class(K);

    REQUIRE(a == 0);
    REQUIRE(renf_elem_class(a) == 0);
    REQUIRE(K.zero() == 0);
    REQUIRE(renf_elem_class(K, 0) == 0);
    REQUIRE(renf_elem_class(K, "0") == 0);
    REQUIRE(renf_elem_class(K, "0*a^2 + 0") == 0);
}

TEST_CASE("Construct renf_elem_class from renf_elem_class", "[renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    const auto& K = GENERATE_REF(take(128, renf_classs(state)));

    auto a = renf_elem_class(mpq_class(13, 37));

    REQUIRE(renf_elem_class(K, a) == a);
    REQUIRE(renf_elem_class(K, renf_elem_class(K, a)) == a);
}

TEST_CASE("Construct renf_elem_class from vector", "[renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    const auto& K = GENERATE_REF(take(128, renf_classs(state)));

    auto a = renf_elem_class(K, std::vector<int>(static_cast<size_t>(K.degree()), -1));
    auto b = renf_elem_class(K, std::vector<unsigned int>(static_cast<size_t>(K.degree()), 1));
    auto c = renf_elem_class(K, std::vector<long>(static_cast<size_t>(K.degree()), -2l));
    auto d = renf_elem_class(K, std::vector<unsigned long>(static_cast<size_t>(K.degree()), 2ul));
    auto e = renf_elem_class(K, std::vector<mpz_class>(static_cast<size_t>(K.degree()), mpz_class(3)));
    auto f = renf_elem_class(K, std::vector<mpq_class>(static_cast<size_t>(K.degree()), mpq_class(4)));

    REQUIRE(std::set<renf_elem_class>{a, b, c, d, e, f}.size() == 6);
}

TEST_CASE("Construct trivial field", "[renf_class]")
{
    const auto& K = renf_class::make();

    REQUIRE(K.degree() == 1);
}
