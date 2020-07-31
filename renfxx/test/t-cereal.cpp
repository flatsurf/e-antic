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

#include "e-antic/renfxx.h"
#include "e-antic/renfxx_cereal.h"

using namespace eantic;
using cereal::JSONOutputArchive;
using cereal::JSONInputArchive;

template <typename T>
T test_serialization(const T& x)
{
    std::stringstream s;

    {
        JSONOutputArchive archive(s);
        archive(x);
    }

    T y;
    {
        JSONInputArchive archive(s);
        archive(y);
    }

    if (x != y)
    {
        throw std::runtime_error("deserialization failed to reconstruct element, the original value had serialized to " + s.str());
    }

    return y;
}

int main(void)
{
    auto K1 = renf_class::make("A^3 - 2", "A", "1.25 +/- 0.1");
    auto K2 = renf_class::make("2*abc^4 - 5*abc + 1", "abc", "0.2 +/- 0.1");
    const renf_elem_class g1 = K1->gen();
    const renf_elem_class g2 = K2->gen();

    test_serialization(renf_elem_class(mpz_class("0")));
    test_serialization(renf_elem_class(mpq_class("2/3")));
    test_serialization(K1->gen());
    test_serialization(K2->gen());
    test_serialization(renf_elem_class(K1));
    test_serialization(renf_elem_class(K2));
    test_serialization(renf_elem_class(K1, "3*A^2-1"));

    auto items = test_serialization(std::vector<renf_elem_class>{g1, K1->one()});
    if (&*items[0].parent() != &*items[1].parent())
    {
        throw std::runtime_error("parents of elements were not deduplicated");
    }

    return 0;
}
