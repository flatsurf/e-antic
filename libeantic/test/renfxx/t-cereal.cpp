/*
    Copyright (C) 2019-2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

#include "../../e-antic/renfxx.h"
#include "../../e-antic/cereal.hpp"

#include "../rand_generator.hpp"
#include "../renf_class_generator.hpp"
#include "../renf_elem_class_generator.hpp"

using namespace eantic;

template <typename T>
T test_serialization(const T& x)
{
    CAPTURE(x);

    std::stringstream s;
    {
        ::cereal::JSONOutputArchive archive(s);
        archive(x);
    }

    CAPTURE(s.str());

    T y;
    {
        ::cereal::JSONInputArchive archive(s);
        archive(y);
    }

    CAPTURE(y);

    REQUIRE(x == y);

    return y;
}

TEST_CASE("Serialize and deserialize elements", "[renf_class][renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    const auto& K = GENERATE_REF(take(128, renf_classs(state)));
    auto a = GENERATE_REF(take(4, renf_elem_classs(state, K)));
    auto b = GENERATE_REF(take(4, renf_elem_classs(state, K)));

    CAPTURE(K);

    test_serialization(a);
    test_serialization(std::vector<renf_elem_class>{a, a, b});
}
