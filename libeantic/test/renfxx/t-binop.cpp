/*
    Copyright (C)      2017 Vincent Delecroix
                  2020-2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/renfxx.h"

#include "../rand_generator.hpp"
#include "../renf_class_generator.hpp"
#include "../renf_elem_class_generator.hpp"

#include "../external/catch2/single_include/catch2/catch.hpp"

using namespace eantic;

TEST_CASE("Arithmetic with renf_elem_class", "[renf_elem_class][binop]")
{
    flint_rand_t& state = GENERATE(rands());
    const auto& K = GENERATE_REF(take(16, renf_classs(state)));
    auto a = GENERATE_REF(take(16, renf_elem_classs(state, K)));

    CAPTURE(K, a);

    SECTION("Unary Operators")
    {
        REQUIRE(-(-a) == a);
        REQUIRE(+a == a);
    }

    SECTION("Addition and Subtraction")
    {
        auto b = GENERATE_REF(take(16, renf_elem_classs(state, K)));

        CAPTURE(b);

        auto c = a + b;

        REQUIRE(c == b + a);

        if (a.sgn() > 0) REQUIRE(c > b);
        if (a.sgn() < 0) REQUIRE(c < b);

        c -= a;
        REQUIRE(c == b);

        c -= b;
        REQUIRE(!c);

        c += 1;
        REQUIRE(c);

        c -= 1;
        REQUIRE(!c);

        c += 1u;
        REQUIRE(c);

        c -= 1u;
        REQUIRE(!c);

        c += 1l;
        REQUIRE(c);

        c -= 1l;
        REQUIRE(!c);

        c += 1ul;
        REQUIRE(c);

        c -= 1ul;
        REQUIRE(!c);

        c += 1ll;
        REQUIRE(c);

        c -= 1ll;
        REQUIRE(!c);

        c += 1ull;
        REQUIRE(c);

        c -= 1ull;
        REQUIRE(!c);

        c += mpz_class(1);
        REQUIRE(c);

        c -= mpz_class(1);
        REQUIRE(!c);

        c += mpq_class(1);
        REQUIRE(c);

        c -= mpq_class(1);
        REQUIRE(!c);
    }

    SECTION("Multiplication and Division")
    {
        auto b = GENERATE_REF(take(16, renf_elem_classs(state, K)));

        CAPTURE(b);

        auto c = a * b;

        REQUIRE(c == b * a);

        if (a == 0 || b == 0)
        {
            REQUIRE(!c);
        }
        else
        {
            c /= a;
            REQUIRE(c == b);

            c /= b;
            REQUIRE(c == 1);

            c *= 2;
            REQUIRE(c == 2);

            c /= 2;
            REQUIRE(c == 1);

            c *= 2u;
            REQUIRE(c == 2);

            c /= 2u;
            REQUIRE(c == 1);

            c *= 2l;
            REQUIRE(c == 2);

            c /= 2l;
            REQUIRE(c == 1);

            c *= 2ul;
            REQUIRE(c == 2);

            c /= 2ul;
            REQUIRE(c == 1);

            c *= 2ll;
            REQUIRE(c == 2);

            c /= 2ll;
            REQUIRE(c == 1);

            c *= 2ull;
            REQUIRE(c == 2);

            c /= 2ull;
            REQUIRE(c == 1);

            c *= mpz_class(2);;
            REQUIRE(c == 2);

            c /= mpz_class(2);
            REQUIRE(c == 1);

            c *= mpq_class(2);
            REQUIRE(c == 2);

            c /= mpq_class(2);
            REQUIRE(c == 1);
        }
    }

    SECTION("Build Element as Sum of Terms")
    {
        auto c = K.zero();
        auto g = K.one();

        auto coeffs = a.num_vector();
        for (size_t i = 0; i < coeffs.size(); i++)
        {
            c += coeffs[i] * g;
            g *= K.gen();
        }

        c /= a.den();

        REQUIRE(a == c);
    }

    SECTION("Floor Divisions")
    {
        SECTION("Floor Division With Rationals")
        {
            auto b = renf_elem_class(mpq_class(1));

            if (b) {
                auto c = a.floordiv(b);

                REQUIRE(c == (a / b).floor());
            }

            if (a) {
                auto c = b.floordiv(a);

                REQUIRE(c == (b / a).floor());
            }
        }

        SECTION("Floor Division With Number Field Elements")
        {
            auto b = GENERATE_REF(take(16, renf_elem_classs(state, K)));

            CAPTURE(b);

            if (b)
            {
                auto c = a.floordiv(b);

                REQUIRE(c == (a / b).floor());
            }
        }
    }
}

TEST_CASE("Incompatible parents cannot be mixed", "[renf_elem][parents]")
{
    flint_rand_t& state = GENERATE(rands());

    const auto& L = GENERATE_REF(take(4, renf_classs(state)));
    const auto& M = GENERATE_REF(take(4, renf_classs(state)));

    const auto a = L.gen();
    auto b = M.gen();

    if (!a.is_rational() && !b.is_rational() && L != M)
      REQUIRE_THROWS(a + b);

    SECTION("Rational Elements can be Mixed")
    {
        b = M.zero();
        REQUIRE((a + b - b) == a);
        REQUIRE((b + a - a) == b);
        REQUIRE((a - b + b) == a);
        REQUIRE((b - a + a) == b);
        REQUIRE((a * b - a) == -a);
        REQUIRE((b * a / a) == b);
        REQUIRE((b / a * a) == b);

        b = M.one();
        REQUIRE((a + b - b) == a);
        REQUIRE((b + a - a) == b);
        REQUIRE((a - b + b) == a);
        REQUIRE((b - a + a) == b);
        REQUIRE((a * b / b) == a);
        REQUIRE((b * a / a) == b);
        REQUIRE((b / a * a) == b);
        REQUIRE((a / b * b) == a);

        b = renf_elem_class(M, "1/2");
        REQUIRE((a + b - b) == a);
        REQUIRE((b + a - a) == b);
        REQUIRE((a - b + b) == a);
        REQUIRE((b - a + a) == b);
        REQUIRE((a * b / b) == a);
        REQUIRE((b * a / a) == b);
        REQUIRE((b / a * a) == b);
        REQUIRE((a / b * b) == a);
    }
}
