/*
    Copyright (C) 2017 Vincent Delecroix
                  2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <set>

#include <e-antic/renfxx.h>

#include "external/catch2/single_include/catch2/catch.hpp"

#include "../../renf/test/rand_generator.hpp"
#include "renf_class_generator.hpp"

using namespace eantic;

TEST_CASE("Construct renf_elem_class from integers and rationals", "[renf_elem_class]")
{
    REQUIRE(renf_elem_class() == 0);
    REQUIRE(renf_elem_class(1) == 1);
    REQUIRE(renf_elem_class(mpz_class(2)) == 2);
    REQUIRE(renf_elem_class(mpq_class(3)) == 3);
}

TEST_CASE("Construct renf_elem_class from string", "[renf_elem_class]")
{
    // operator = for renf_class
    auto K1 = renf_class::make("a^2 - 2", "a", "1.41 +/- 0.1", 128);
    auto K2 = K1;
  
    renf_elem_class a(K1, "a + 1");
    renf_elem_class b(K2, "a + 1");
  
    REQUIRE(a == b);
}

TEST_CASE("Construct trivial renf_elem_class", "[renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    auto K = GENERATE_REF(renf_classs(state));

    auto a = renf_elem_class(K);

    REQUIRE(a == 0);
    REQUIRE(renf_elem_class(a) == 0);
    REQUIRE(K->zero() == 0);
    REQUIRE(renf_elem_class(K, 0) == 0);
    REQUIRE(renf_elem_class(K, "0") == 0);
    REQUIRE(renf_elem_class(K, "0*a^2 + 0") == 0);
}

TEST_CASE("Construct renf_elem_class from renf_elem_class", "[renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    auto K = GENERATE_REF(renf_classs(state));

    auto a = renf_elem_class(mpq_class(13, 37));

    REQUIRE(renf_elem_class(K, a) == a);
    REQUIRE(renf_elem_class(K, renf_elem_class(K, a)) == a);
}

TEST_CASE("Construct renf_elem_class from vector", "[renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    auto K = GENERATE_REF(renf_classs(state));

    auto a = renf_elem_class(K, std::vector<int>(K->degree(), -1));
    auto b = renf_elem_class(K, std::vector<unsigned int>(K->degree(), 1));
    auto c = renf_elem_class(K, std::vector<long>(K->degree(), -2l));
    auto d = renf_elem_class(K, std::vector<unsigned long>(K->degree(), 2ul));
    auto e = renf_elem_class(K, std::vector<mpz_class>(K->degree(), mpz_class(3)));
    auto f = renf_elem_class(K, std::vector<mpq_class>(K->degree(), mpq_class(4)));

    REQUIRE(std::set<renf_elem_class>{a, b, c, d, e, f}.size() == 6);
}
