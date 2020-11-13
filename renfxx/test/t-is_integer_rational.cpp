/*
    Copyright (C) 2018 Vincent Delecroix
                  2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renfxx.h>

#include "external/catch2/single_include/catch2/catch.hpp"

#include "../../renf/test/rand_generator.hpp"
#include "renf_class_generator.hpp"

using namespace eantic;

TEST_CASE("Detect integer/rational elements", "[renf_elem_class][is_integer][is_rational]")
{
    flint_rand_t& state = GENERATE(rands());

    SECTION("Elements in the rational field")
    {
        REQUIRE(renf_elem_class().is_integer());
        REQUIRE(renf_elem_class().is_rational());
        REQUIRE(renf_elem_class(1).is_integer());
        REQUIRE(renf_elem_class(1).is_rational());
        REQUIRE(!(renf_elem_class(1)/2).is_integer());
        REQUIRE((renf_elem_class(1)/2).is_rational());
    }

    SECTION("Elements in general number fields")
    {
        auto K = GENERATE_REF(take(128, renf_classs(state)));

        REQUIRE(renf_elem_class(K, 0).is_integer());
        REQUIRE(renf_elem_class(K, 0).is_rational());

        if (K->degree() == 1)
        {
            REQUIRE(K->gen().is_rational());
        }
        else
        {
            REQUIRE(!K->gen().is_integer());
            REQUIRE(!K->gen().is_rational());
        }
    }
}
