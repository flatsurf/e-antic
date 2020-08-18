/*
    Copyright (C) 2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_TEST_RENF_ELEM_CLASS_GENERATOR_HPP
#define E_ANTIC_TEST_RENF_ELEM_CLASS_GENERATOR_HPP

#include <memory>

#include <e-antic/renfxx.h>

#include "../../renf_elem/test/renf_elem_generator.hpp"

#include "../../renfxx/test/external/catch2/single_include/catch2/catch.hpp"

namespace {

struct RenfElemClassGenerator : public Catch::Generators::IGenerator<eantic::renf_elem_class>
{
    RenfElemGenerator generator;

    mutable eantic::renf_elem_class current;

    RenfElemClassGenerator(flint_rand_t& state, std::shared_ptr<const eantic::renf_class> nf, ulong iterations, ulong minbits, ulong maxbits) : generator(state, nf->renf_t(), iterations, minbits, maxbits) {}

    bool next() override
    {
        return generator.next();
    }

    eantic::renf_elem_class& get() const override
    {
        auto nf = eantic::renf_class::make(generator.nf);
        current = eantic::renf_elem_class(nf);
        renf_elem_set(current.renf_elem_t(), generator.get(), nf->renf_t());
        return current;
    }
};

/*
 * Wrap RenfElemClassGenerator for use as GENERATE(renf_elem_classs(...))
 */
Catch::Generators::GeneratorWrapper<eantic::renf_elem_class> renf_elem_classs(flint_rand_t& state, std::shared_ptr<const eantic::renf_class> nf, ulong iterations = 128, ulong minbits = 10, ulong maxbits = 40)
{
    return Catch::Generators::GeneratorWrapper<eantic::renf_elem_class>(std::unique_ptr<Catch::Generators::IGenerator<eantic::renf_elem_class>>(new RenfElemClassGenerator(state, nf, iterations, minbits, maxbits)));
}

}

#endif
