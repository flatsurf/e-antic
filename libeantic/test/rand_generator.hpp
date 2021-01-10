/*
    Copyright (C) 2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_TEST_RAND_GENERATOR_HPP
#define E_ANTIC_TEST_RAND_GENERATOR_HPP

#include <flint/flint.h>

#include "external/catch2/single_include/catch2/catch.hpp"

namespace {

/*
 * Generates a single flint_rand_t in a way that is compatible with Catch2 testing.

 */
struct RandGenerator : public Catch::Generators::IGenerator<flint_rand_t&>
{
    flint_rand_t state;

    RandGenerator()
    {
        FLINT_GC_INIT();
        flint_randinit(state);
    }

    bool next() override
    {
        return false;
    }

    flint_rand_t& get() const override
    {
        return const_cast<flint_rand_t&>(state);
    }

    ~RandGenerator() override
    {
        flint_randclear(state);
        flint_cleanup_master();
    }
};

/*
 * Wrap RandGenerator for use in GENERATE(rands())
 */
[[maybe_unused]] Catch::Generators::GeneratorWrapper<flint_rand_t&> rands()
{
  return Catch::Generators::GeneratorWrapper<flint_rand_t&>(std::unique_ptr<Catch::Generators::IGenerator<flint_rand_t&>>(new RandGenerator()));
}

}

#endif
