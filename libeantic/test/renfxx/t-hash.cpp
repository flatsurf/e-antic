/*
    Copyright (C) 2020-2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <functional>

#include "../../e-antic/renfxx.h"

#include "../external/catch2/single_include/catch2/catch.hpp"

using namespace eantic;

TEST_CASE("Consistency of Hash Function", "[renf_elem_class][hash]")
{
    const auto K = renf_class::make("x^2 - x - 1", "x", "1.618 +/- 0.1");
    const auto L = renf_class::make("x^2 - 2", "x", "1.41 +/- 0.1");

    REQUIRE(*K != *L);
    REQUIRE(!(*K == *L));

    SECTION("hash can distinguish fields")
    {
        REQUIRE(std::hash<renf_class>()(*K) != std::hash<renf_class>()(*L));
    }

    SECTION("hash can distinguish elements")
    {
        REQUIRE(std::hash<renf_elem_class>()(K->gen()) != std::hash<renf_elem_class>()(L->gen()));
    }
    
    SECTION("has is compatible with operator==")
    {
        REQUIRE(std::hash<renf_elem_class>()(mpq_class("3/2")) == std::hash<renf_elem_class>()(renf_elem_class(*K, mpq_class("3/2"))));
    }
}
