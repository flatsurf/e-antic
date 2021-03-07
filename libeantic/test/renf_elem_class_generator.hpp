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

#include <boost/optional.hpp>

#include "../e-antic/renfxx.h"

#include "renf_elem_generator.hpp"

#include "external/catch2/single_include/catch2/catch.hpp"

namespace {

struct RenfElemClassGenerator : public Catch::Generators::IGenerator<eantic::renf_elem_class>
{
    flint_rand_t& state;
    boost::intrusive_ptr<const eantic::renf_class> nf;
    ulong minbits, maxbits;

    mutable boost::optional<eantic::renf_elem_class> current;

    RenfElemClassGenerator(flint_rand_t& state, const eantic::renf_class& nf, ulong minbits, ulong maxbits) : state(state), nf(&nf), minbits(minbits), maxbits(maxbits) {
      assert(maxbits > minbits);
      current = nf.zero();
    }

    bool next() override
    {
        if (current)
            current = {};
        return true;
    }

    eantic::renf_elem_class& get() const override
    {
        if (!current)
        {
            ulong bits = minbits + n_randint(state, maxbits - minbits);

            current = eantic::renf_elem_class(*nf);

            renf_elem_randtest(current->renf_elem_t(), state, bits, nf->renf_t());
        }
        return *current;
    }
};

/*
 * Wrap RenfElemClassGenerator for use as GENERATE(renf_elem_classs(...))
 */
inline Catch::Generators::GeneratorWrapper<eantic::renf_elem_class> renf_elem_classs(flint_rand_t& state, const eantic::renf_class& nf, ulong minbits = 10, ulong maxbits = 40)
{
    return Catch::Generators::GeneratorWrapper<eantic::renf_elem_class>(std::unique_ptr<Catch::Generators::IGenerator<eantic::renf_elem_class>>(new RenfElemClassGenerator(state, nf, minbits, maxbits)));
}

}

#endif
