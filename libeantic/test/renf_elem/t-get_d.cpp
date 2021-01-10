/*
    Copyright (C) 2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <arf.h>

#include "../rand_generator.hpp"
#include "../renf_generator.hpp"
#include "../renf_elem_generator.hpp"

#include "../../e-antic/renf_elem.h"

static void d_get_fmpq(double d, fmpq_t q)
{
    arf_t a;
    fmpz_t man, exp, quo;

    REQUIRE(isnormal(d));

    arf_init(a);
    fmpz_init(man);
    fmpz_init(exp);
    fmpz_init(quo);

    arf_set_d(a, d);
    arf_get_fmpz_2exp(man, exp, a);
    fmpz_set_si(quo, 1);

    if (fmpz_cmp_si(exp, 0) > 0)
      fmpz_mul_2exp(man, man, fmpz_get_ui(exp));
    else
      fmpz_mul_2exp(quo, quo, static_cast<mp_limb_t>(-fmpz_get_si(exp)));

    fmpq_set_fmpz_frac(q, man, quo);

    fmpz_clear(man);
    fmpz_clear(exp);
    fmpz_clear(quo);
    arf_clear(a);
}

TEST_CASE("Convert renf_elem to double", "[renf_elem][get_d]")
{
    flint_rand_t& state = GENERATE(rands());
    renf_t& nf = GENERATE_REF(take(64, renfs(state)));
    renf_elem_t& a = GENERATE_REF(take(128, renf_elems(state, nf)));

    CAPTURE(nf);
    CAPTURE(a);

    double ceil = renf_elem_get_d(a, nf, ARF_RND_CEIL);
    double floor = renf_elem_get_d(a, nf, ARF_RND_FLOOR);
    double up = renf_elem_get_d(a, nf, ARF_RND_UP);
    double down = renf_elem_get_d(a, nf, ARF_RND_DOWN);
    double near = renf_elem_get_d(a, nf, ARF_RND_NEAR);

    CAPTURE(ceil, floor, up, down, near);

    REQUIRE((floor == near || ceil == near));

    int sgn = renf_elem_sgn(a, nf);

    if (sgn == 0)
    {
        REQUIRE(ceil == 0);
        REQUIRE(floor == 0);
        REQUIRE(up == 0);
        REQUIRE(down == 0);
        REQUIRE(near == 0);
    }
    else
    {
        if (sgn == 1)
        {
            REQUIRE(ceil == up);
            REQUIRE(floor == down);
        }
        else
        {
            REQUIRE(ceil == down);
            REQUIRE(floor == up);
        }

        SECTION("ceil & floor are successive doubles")
        {
            REQUIRE((floor == ceil || nextafter(floor, 1./0.) == ceil));
        }

        SECTION("finite and infinite doubles")
        {
            fmpq_t dmax;
            fmpq_init(dmax);

            d_get_fmpq(-DBL_MAX, dmax);

            if (renf_elem_cmp_fmpq(a, dmax, nf) < 0)
            {
                REQUIRE(isinf(floor));
                REQUIRE(floor < 0);
            }
            else
            {
                REQUIRE((!isinf(floor) || floor > 0));
            }

            d_get_fmpq(DBL_MAX, dmax);

            if (renf_elem_cmp_fmpq(a, dmax, nf) > 0)
            {
                REQUIRE(isinf(ceil));
                REQUIRE(ceil > 0);
            }
            else
            {
                REQUIRE((!isinf(ceil) || ceil < 0));
            }

            fmpq_clear(dmax);
        }

        SECTION("near rounded correctly")
        {
            fmpq_t q_floor, q_ceil;

            fmpq_init(q_floor);
            fmpq_init(q_ceil);

            renf_elem_t floor_to_a, a_to_ceil;

            renf_elem_init(floor_to_a, nf);
            renf_elem_init(a_to_ceil, nf);

            {
                d_get_fmpq(floor, q_floor);
                d_get_fmpq(ceil, q_ceil);

                REQUIRE(renf_elem_cmp_fmpq(a, q_floor, nf) >= 0);
                REQUIRE(renf_elem_cmp_fmpq(a, q_ceil, nf) <= 0);

                renf_elem_sub_fmpq(floor_to_a, a, q_floor, nf);
                renf_elem_sub_fmpq(a_to_ceil, a, q_ceil, nf);
                renf_elem_neg(a_to_ceil, a_to_ceil, nf);

                if (floor == near)
                    REQUIRE(renf_elem_cmp(floor_to_a, a_to_ceil, nf) <= 0);
                else
                    REQUIRE(renf_elem_cmp(floor_to_a, a_to_ceil, nf) >= 0);
            }

            renf_elem_clear(floor_to_a, nf);
            renf_elem_clear(a_to_ceil, nf);

            fmpq_clear(q_floor);
            fmpq_clear(q_ceil);
        }
    }
}
