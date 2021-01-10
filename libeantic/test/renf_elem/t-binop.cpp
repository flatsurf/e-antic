/*
    Copyright (C) 2017 Vincent Delecroix
    Copyright (C) 2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <limits>

#include "../../e-antic/renf_elem.h"

#include "rand_generator.hpp"
#include "renf_generator.hpp"
#include "renf_elem_generator.hpp"
#include "fmpz_generator.hpp"
#include "fmpq_generator.hpp"

TEST_CASE("Arithmetic with renf_elem", "[renf_elem][binop]")
{
    flint_rand_t& state = GENERATE(rands());
    renf_t& nf = GENERATE_REF(take(16, renfs(state)));
    renf_elem_t& a = GENERATE_REF(take(16, renf_elems(state, nf)));
    renf_elem_t& b = GENERATE_REF(take(16, renf_elems(state, nf)));
    
    CAPTURE(nf, a, b);

    renf_elem_t c;
    renf_elem_init(c, nf);

    /* c = a + b */
    renf_elem_add(c, a, b, nf);
    renf_elem_check_embedding(c, nf, 1024);

    /* c = (a + b) - a - b */
    renf_elem_sub(c, c, a, nf);
    renf_elem_check_embedding(c, nf, 1024);
    renf_elem_sub(c, c, b, nf);
    renf_elem_check_embedding(c, nf, 1024);

    CAPTURE(c);
    REQUIRE(renf_elem_is_zero(c, nf));

    renf_elem_mul(c, a, b, nf);
    if (renf_elem_is_zero(b, nf))
    {
        CAPTURE(c);
        REQUIRE(renf_elem_is_zero(c, nf));
    }
    else
    {
        renf_elem_div(c, c, b, nf);
        CAPTURE(c);
        REQUIRE(renf_elem_equal(c, a, nf));
    }

    renf_elem_clear(c, nf);
}

TEST_CASE("Floor Division of renf_elem", "[renf_elem][binop][fdiv]")
{
    flint_rand_t& state = GENERATE(rands());
    renf_t& nf = GENERATE_REF(take(16, renfs(state)));
    renf_elem_t& a = GENERATE_REF(take(64, renf_elems(state, nf)));

    SECTION("The quotient a // b for random numbers")
    {
        renf_elem_t& b = GENERATE_REF(take(4, renf_elems(state, nf)));

        if (!renf_elem_is_zero(b, nf))
        {
            CAPTURE(nf, a, b);

            fmpz_t fdiv;
            fmpz_init(fdiv);

            renf_elem_fdiv(fdiv, a, b, nf);

            renf_elem_t quotient;
            renf_elem_init(quotient, nf);

            fmpz_t fquotient;
            fmpz_init(fquotient);

            renf_elem_div(quotient, a, b, nf);
            renf_elem_floor(fquotient, quotient, nf);

            CAPTURE(fdiv, fquotient);
            REQUIRE(fmpz_equal(fdiv, fquotient));

            fmpz_clear(fquotient);
            fmpz_clear(fdiv);
            renf_elem_clear(quotient, nf);
        }
    }

    SECTION("The quotient (a * n) // a for random integers n")
    {
        fmpz_t& n = GENERATE_REF(take(4, fmpzs(state)));

        if (!fmpz_is_zero(n)) {
            renf_elem_t b;
            renf_elem_init(b, nf);
            renf_elem_mul_fmpz(b, a, n, nf);

            fmpz_t fdiv;
            fmpz_init(fdiv);

            CAPTURE(a, b);
            renf_elem_fdiv(fdiv, b, a, nf);

            CAPTURE(fdiv, n);
            if (renf_elem_is_zero(a, nf))
                REQUIRE(fmpz_is_zero(fdiv));
            else
                REQUIRE(fmpz_equal(fdiv, n));

            fmpz_clear(fdiv);
            renf_elem_clear(b, nf);
        }
    }
}

TEST_CASE("Arithmetic with renf_elem and int", "[renf_elem][binop]")
{
    flint_rand_t& state = GENERATE(rands());
    renf_t& nf = GENERATE_REF(take(16, renfs(state)));
    renf_elem_t& a = GENERATE_REF(take(16, renf_elems(state, nf)));
    int b = GENERATE(take(16, random<int>(std::numeric_limits<int>::min(), std::numeric_limits<int>::max())));
    
    CAPTURE(nf, a, b);

    renf_elem_t c;
    renf_elem_init(c, nf);

    /* c = a + b */
    renf_elem_add_si(c, a, b, nf);
    renf_elem_check_embedding(c, nf, 1024);

    /* c = (a + b) - a - b */
    renf_elem_sub(c, c, a, nf);
    renf_elem_check_embedding(c, nf, 1024);
    renf_elem_sub_si(c, c, b, nf);
    renf_elem_check_embedding(c, nf, 1024);

    CAPTURE(c);
    REQUIRE(renf_elem_is_zero(c, nf));

    renf_elem_mul_si(c, a, b, nf);
    if (b == 0)
    {
        CAPTURE(c);
        REQUIRE(renf_elem_is_zero(c, nf));
    }
    else
    {
        renf_elem_div_si(c, c, b, nf);
        CAPTURE(c);
        REQUIRE(renf_elem_equal(c, a, nf));
    }

    renf_elem_clear(c, nf);
}

TEST_CASE("Arithmetic with renf_elem and unsigned int", "[renf_elem][binop]")
{
    flint_rand_t& state = GENERATE(rands());
    renf_t& nf = GENERATE_REF(take(16, renfs(state)));
    renf_elem_t& a = GENERATE_REF(take(16, renf_elems(state, nf)));
    unsigned int b = GENERATE(take(16, random<unsigned int>(std::numeric_limits<unsigned int>::min(), std::numeric_limits<unsigned int>::max())));
    
    CAPTURE(nf, a, b);

    renf_elem_t c;
    renf_elem_init(c, nf);

    /* c = a + b */
    renf_elem_add_ui(c, a, b, nf);
    renf_elem_check_embedding(c, nf, 1024);

    /* c = (a + b) - a - b */
    renf_elem_sub(c, c, a, nf);
    renf_elem_check_embedding(c, nf, 1024);
    renf_elem_sub_ui(c, c, b, nf);
    renf_elem_check_embedding(c, nf, 1024);

    CAPTURE(c);
    REQUIRE(renf_elem_is_zero(c, nf));

    renf_elem_mul_ui(c, a, b, nf);
    if (b == 0)
    {
        CAPTURE(c);
        REQUIRE(renf_elem_is_zero(c, nf));
    }
    else
    {
        renf_elem_div_ui(c, c, b, nf);
        CAPTURE(c);
        REQUIRE(renf_elem_equal(c, a, nf));
    }

    renf_elem_clear(c, nf);
}

TEST_CASE("Arithmetic with renf_elem and fmpz", "[renf_elem][binop]")
{
    flint_rand_t& state = GENERATE(rands());
    renf_t& nf = GENERATE_REF(take(16, renfs(state)));
    renf_elem_t& a = GENERATE_REF(take(16, renf_elems(state, nf)));
    fmpz_t& b = GENERATE_REF(take(16, fmpzs(state)));
    
    CAPTURE(nf, a, b);

    renf_elem_t c;
    renf_elem_init(c, nf);

    /* c = a + b */
    renf_elem_add_fmpz(c, a, b, nf);
    renf_elem_check_embedding(c, nf, 1024);

    /* c = (a + b) - a - b */
    renf_elem_sub(c, c, a, nf);
    renf_elem_check_embedding(c, nf, 1024);
    renf_elem_sub_fmpz(c, c, b, nf);
    renf_elem_check_embedding(c, nf, 1024);

    CAPTURE(c);
    REQUIRE(renf_elem_is_zero(c, nf));

    renf_elem_mul_fmpz(c, a, b, nf);
    if (fmpz_is_zero(b))
    {
        CAPTURE(c);
        REQUIRE(renf_elem_is_zero(c, nf));
    }
    else
    {
        renf_elem_div_fmpz(c, c, b, nf);
        CAPTURE(c);
        REQUIRE(renf_elem_equal(c, a, nf));
    }

    renf_elem_clear(c, nf);
}

TEST_CASE("Arithmetic with renf_elem and fmpq", "[renf_elem][binop]")
{
    flint_rand_t& state = GENERATE(rands());
    renf_t& nf = GENERATE_REF(take(16, renfs(state)));
    renf_elem_t& a = GENERATE_REF(take(16, renf_elems(state, nf)));
    fmpq_t& b = GENERATE_REF(take(16, fmpqs(state)));
    
    CAPTURE(nf, a, b);

    renf_elem_t c;
    renf_elem_init(c, nf);

    /* c = a + b */
    renf_elem_add_fmpq(c, a, b, nf);
    renf_elem_check_embedding(c, nf, 1024);

    /* c = (a + b) - a - b */
    renf_elem_sub(c, c, a, nf);
    renf_elem_check_embedding(c, nf, 1024);
    renf_elem_sub_fmpq(c, c, b, nf);
    renf_elem_check_embedding(c, nf, 1024);

    CAPTURE(c);
    REQUIRE(renf_elem_is_zero(c, nf));

    renf_elem_mul_fmpq(c, a, b, nf);
    if (fmpq_is_zero(b))
    {
        CAPTURE(c);
        REQUIRE(renf_elem_is_zero(c, nf));
    }
    else
    {
        renf_elem_div_fmpq(c, c, b, nf);
        CAPTURE(c);
        REQUIRE(renf_elem_equal(c, a, nf));
    }

    renf_elem_clear(c, nf);
}
