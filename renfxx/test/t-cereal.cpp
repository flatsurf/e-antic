/*
    Copyright (C) 2019 Julian Rüth

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
#include "../../e-antic/renfxx_cereal.h"

#include "../../renf/test/rand_generator.hpp"
#include "renf_class_generator.hpp"
#include "renf_elem_class_generator.hpp"

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

static std::shared_ptr<const renf_class> K = nullptr;

TEST_CASE("Serialize and deserialize elements", "[renf_class][renf_elem_class]")
{
    flint_rand_t& state = GENERATE(rands());
    K = GENERATE_REF(renf_classs(state));
    auto a = GENERATE_REF(renf_elem_classs(state, K, 4));
    auto b = GENERATE_REF(renf_elem_classs(state, K, 4));

    CAPTURE(*K);

    test_serialization(a);
    test_serialization(std::vector<renf_elem_class>{a, a, b});
}
