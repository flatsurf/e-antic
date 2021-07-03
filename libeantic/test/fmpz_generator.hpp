/*
    Copyright (C) 2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_TEST_FMPZ_GENERATOR_HPP
#define E_ANTIC_TEST_FMPZ_GENERATOR_HPP

#include <flint/fmpz.h>

#include "external/catch2/single_include/catch2/catch.hpp"

namespace {

/*
 * A generator of random fmpz_t integers.
 */
struct FmpzGenerator : public Catch::Generators::IGenerator<fmpz_t&>
{
    flint_rand_t& state;
    ulong minbits, maxbits;

    mutable fmpz_t a;
    mutable bool has_value_for_iteration = false;

    FmpzGenerator(flint_rand_t& state, ulong minbits, ulong maxbits) : state(state), minbits(minbits), maxbits(maxbits)
    {
       assert(maxbits > minbits);
    }

    void clear()
    {
        if (has_value_for_iteration)
            fmpz_clear(a);
        has_value_for_iteration = false;
    }

    bool next() override
    {
        clear();
        return true;
    }

    fmpz_t& get() const override
    {
        if (!has_value_for_iteration)
        {
            ulong bits = minbits + n_randint(state, maxbits - minbits);

            fmpz_init(a);
            fmpz_randtest(a, state, bits);

            has_value_for_iteration = true;
        }
        return a;
    }

    ~FmpzGenerator() override
    {
        clear();
    }
};

/*
 * Wrap FmpzGenerator for use as GENERATE_REF(take(64, fpmzs(...))).
 */
[[maybe_unused]] Catch::Generators::GeneratorWrapper<fmpz_t&> fmpzs(flint_rand_t& state, ulong minbits = 10, ulong maxbits = 40)
{
    return Catch::Generators::GeneratorWrapper<fmpz_t&>(std::unique_ptr<Catch::Generators::IGenerator<fmpz_t&>>(new FmpzGenerator(state, minbits, maxbits)));
}

}

namespace Catch {

/*
 * Make printing of `fmpz_t` work in Catch, i.e., show meaingful debug
 * output for `CAPTURE(a)` where `a` is an `fmpz_t`.
 */
template <>
struct StringMaker<fmpz_t>
{
    static std::string convert(const fmpz_t& a)
    {
        char * str = fmpz_get_str(nullptr, 10, a);

        std::string ret = str;

        flint_free(str);

        return ret;
    }
};

}


#endif

