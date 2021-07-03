/*
    Copyright (C) 2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_TEST_FMPQ_GENERATOR_HPP
#define E_ANTIC_TEST_FMPQ_GENERATOR_HPP

#include <flint/fmpq.h>

#include "external/catch2/single_include/catch2/catch.hpp"

namespace {

/*
 * A generator of random fmpq_t rationals.
 */
struct FmpqGenerator : public Catch::Generators::IGenerator<fmpq_t&>
{
    flint_rand_t& state;
    ulong minbits, maxbits;

    mutable fmpq_t a;
    mutable bool has_value_for_iteration = false;

    FmpqGenerator(flint_rand_t& state, ulong minbits, ulong maxbits) : state(state), minbits(minbits), maxbits(maxbits)
    {
      assert(maxbits > minbits);
    }

    void clear()
    {
        if (has_value_for_iteration)
            fmpq_clear(a);
        has_value_for_iteration = false;
    }

    bool next() override
    {
        clear();
        return true;
    }

    fmpq_t& get() const override
    {
        if (!has_value_for_iteration)
        {
            ulong bits = minbits + n_randint(state, maxbits - minbits);

            fmpq_init(a);
            fmpq_randtest(a, state, bits);

            has_value_for_iteration = true;
        }
        return a;
    }

    ~FmpqGenerator() override
    {
        clear();
    }
};

/*
 * Wrap FmpqGenerator for use as GENERATE_REF(take(64, fmpqs(...))).
 */
[[maybe_unused]] Catch::Generators::GeneratorWrapper<fmpq_t&> fmpqs(flint_rand_t& state, ulong minbits = 10, ulong maxbits = 40)
{
    return Catch::Generators::GeneratorWrapper<fmpq_t&>(std::unique_ptr<Catch::Generators::IGenerator<fmpq_t&>>(new FmpqGenerator(state, minbits, maxbits)));
}

}

namespace Catch {

/*
 * Make printing of `fmpq_t` work in Catch, i.e., show meaingful debug
 * output for `CAPTURE(a)` where `a` is an `fmpq_t`.
 */
template <>
struct StringMaker<fmpq_t>
{
    static std::string convert(const fmpq_t& a)
    {
        char * str = fmpq_get_str(nullptr, 10, a);

        std::string ret = str;

        flint_free(str);

        return ret;
    }
};

}


#endif

