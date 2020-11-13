/*
    Copyright (C) 2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_TEST_RENF_CLASS_GENERATOR_HPP
#define E_ANTIC_TEST_RENF_CLASS_GENERATOR_HPP

#include <memory>

#include <e-antic/renfxx.h>

#include "../../renf/test/renf_generator.hpp"

#include "../../renfxx/test/external/catch2/single_include/catch2/catch.hpp"

namespace {

struct RenfClassGenerator : public Catch::Generators::IGenerator<std::shared_ptr<const eantic::renf_class>>
{
    RenfGenerator generator;

    mutable std::shared_ptr<const eantic::renf_class> current;

    RenfClassGenerator(flint_rand_t& state, ulong minlen, ulong maxlen, ulong minprec, ulong maxprec, ulong minbits, ulong maxbits) : generator(state, minlen, maxlen, minprec, maxprec, minbits, maxbits) {}

    bool next() override
    {
        return generator.next();
    }

    std::shared_ptr<const eantic::renf_class>& get() const override
    {
        current = eantic::renf_class::make(generator.get());        
        return current;
    }
};

/*
 * Wrap RenfClassGenerator for use as GENERATE(renf_classs(...))
 */
Catch::Generators::GeneratorWrapper<std::shared_ptr<const eantic::renf_class>> renf_classs(flint_rand_t& state, ulong minlen = 2, ulong maxlen = 32, ulong minprec = 8, ulong maxprec = 2048, ulong minbits = 10, ulong maxbits = 40)
{
    return Catch::Generators::GeneratorWrapper<std::shared_ptr<const eantic::renf_class>>(std::unique_ptr<Catch::Generators::IGenerator<std::shared_ptr<const eantic::renf_class>>>(new RenfClassGenerator(state, minlen, maxlen, minprec, maxprec, minbits, maxbits)));
}

}

#endif
