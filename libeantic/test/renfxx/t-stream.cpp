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

#include "../external/catch2/single_include/catch2/catch.hpp"

#include <string>
#include <regex>

#include <boost/lexical_cast.hpp>

#include "../rand_generator.hpp"
#include "../renf_class_generator.hpp"
#include "../renf_elem_class_generator.hpp"

using namespace eantic;

TEST_CASE("Converting elements to strings", "[renf_elem_class][operator<<]")
{
    const auto K1 = renf_class::make("A^3 - 2", "A", "1.25 +/- 0.1");
    const auto K2 = renf_class::make("2*abc^4 - 5*abc + 1", "abc", "0.2 +/- 0.1");

    REQUIRE(boost::lexical_cast<std::string>(renf_elem_class(mpz_class("0"))) == "0");
    REQUIRE(boost::lexical_cast<std::string>(renf_elem_class(mpq_class("2/3"))) == "(2/3 ~ 0.66666667)");
    REQUIRE(boost::lexical_cast<std::string>(renf_elem_class(K1->gen())) == "(A ~ 1.2599210)");
    REQUIRE(boost::lexical_cast<std::string>(renf_elem_class(K2->gen())) == "(abc ~ 0.20064834)");
}

TEST_CASE("Converting fields to strings", "[renf_class][operator<<]")
{
    const std::string numerical_noise = "\\d+ \\+/- \\d+\\.\\d+e-\\d+\\]\\)";

    const auto K1 = renf_class::make("A^3 - 2", "A", "1.25 +/- 0.1");
    const auto K2 = renf_class::make("2*abc^4 - 5*abc + 1", "abc", "0.2 +/- 0.1");

    {
        auto s = boost::lexical_cast<std::string>(*K1);
        CAPTURE(s);
        REQUIRE(s == K1->to_string());
        REQUIRE(std::regex_match(s, std::regex("NumberField\\(A\\^3 - 2, \\[1\\.2599210498948731" + numerical_noise)));
    }

    {
        auto s = boost::lexical_cast<std::string>(*K2);
        CAPTURE(s);
        REQUIRE(s == K2->to_string());
        REQUIRE(std::regex_match(s, std::regex("NumberField\\(2\\*abc\\^4 - 5\\*abc \\+ 1, \\[0\\.2006483391818005" + numerical_noise)));
    }
}

TEST_CASE("Writing and reading elements from streams", "[renf_elem_class][operator<<][operator>>]")
{
    flint_rand_t& state = GENERATE(rands());
    const auto& K = GENERATE_REF(take(128, renf_classs(state)));
    auto a = GENERATE_REF(take(128, renf_elem_classs(state, K)));

    CAPTURE(a);

    const auto s = boost::lexical_cast<std::string>(a);

    CAPTURE(s);

    std::stringstream in(s);

    K.set_pword(in);
    REQUIRE(*renf_class::get_pword(in) == K);

    renf_elem_class b;
    in >> b;

    REQUIRE(a == b);
}
