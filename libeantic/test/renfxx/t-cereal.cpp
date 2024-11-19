/*
    Copyright (C) 2019-2022 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>

#include "../../e-antic/config.h"

#include "../../e-antic/cereal.hpp"

#include "../rand_generator.hpp"
#include "../renf_class_generator.hpp"
#include "../renf_elem_class_generator.hpp"

using namespace eantic;

template <typename OutputArchive, typename T>
T test_serialization(const T& x)
{
    CAPTURE(x);

    std::stringstream s;
    {
        OutputArchive archive(s);
        archive(x);
    }

    CAPTURE(s.str());

    T y;
    {
        using InputArchive = std::conditional_t<std::is_same<OutputArchive, ::cereal::JSONOutputArchive>::value, ::cereal::JSONInputArchive, ::cereal::BinaryInputArchive>;

        InputArchive archive(s);
        archive(y);
    }

    CAPTURE(y);

    REQUIRE(x == y);

    return y;
}

TEMPLATE_TEST_CASE("Serialize and deserialize elements", "[renf_class][renf_elem_class]", ::cereal::JSONOutputArchive, ::cereal::BinaryOutputArchive)
{
    flint_rand_t& state = GENERATE(rands());
    const auto& K = GENERATE_REF(take(128, renf_classs(state)));
    auto a = GENERATE_REF(take(4, renf_elem_classs(state, K)));
    auto b = GENERATE_REF(take(4, renf_elem_classs(state, K)));

    CAPTURE(K);

    test_serialization<TestType>(a);
    test_serialization<TestType>(std::vector<renf_elem_class>{a, a, b});
}

TEST_CASE("Deserialize elements which were created with e-antic <1") {
    std::stringstream s;
    s << R"({"cereal":{"cereal_class_version": 0, "parent": {"shared": 2147483649, "name": "c", "embedding": "[1.979642883761865464752184075553437574753038743897433376 +/- 4.15e-55]", "minpoly": "c^10 - 11*c^8 + 44*c^6 - 77*c^4 + 55*c^2 - 11", "precision": 64}, "value": "292/33*c^9 + 2272/33*c^8 - 804/11*c^7 - 6225/11*c^6 + 6032/33*c^5 + 46124/33*c^4 - 156*c^3 - 39275/33*c^2 + 100/3*c + 307"}})";

    renf_elem_class x;
    {
      ::cereal::JSONInputArchive archive(s);
      archive(x);
    }
}
