/*
    Copyright (C) 2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_TEST_RENF_GENERATOR_HPP
#define E_ANTIC_TEST_RENF_GENERATOR_HPP

#include <sstream>

#include <e-antic/renf.h>

#include "../../renfxx/test/external/catch2/single_include/catch2/catch.hpp"

namespace {

/*
 * A generator of random renf_t number fields.
 */
struct RenfGenerator : public Catch::Generators::IGenerator<renf_t&>
{
    flint_rand_t& state;
    slong iterations, minlen, maxlen, minprec, maxprec, minbits, maxbits;

    mutable renf_t nf;
    mutable slong iteration = 0;
    mutable bool has_value_for_iteration = false;

    RenfGenerator(flint_rand_t& state, slong iterations, slong minlen, slong maxlen, slong minprec, slong maxprec, slong minbits, slong maxbits) : state(state), iterations(iterations), minlen(minlen), maxlen(maxlen), minprec(minprec), maxprec(maxprec), minbits(minbits), maxbits(maxbits) {}

    bool next() override
    {
        has_value_for_iteration = false;
        return ++iteration < iterations;
    }

    renf_t& get() const override
    {
        if (!has_value_for_iteration)
        {
            assert(iteration < iterations);

            if (iteration)
                renf_clear(nf);

            slong len = minlen + static_cast<slong>(n_randint(state, maxlen - minlen));
            slong prec = minprec + static_cast<slong>(n_randint(state, maxprec - minprec));
            mp_bitcnt_t bits = minbits + n_randint(state, maxbits - minbits);

            renf_randtest(nf, state, len, prec, bits);

            has_value_for_iteration = true;
        }
        return nf;
    }

    ~RenfGenerator()
    {
        if (iteration)
            renf_clear(nf);
    }
};


/*
 * Wrap RenfGenerator for use as GENERATE(renfs(...))
 */
Catch::Generators::GeneratorWrapper<renf_t&> renfs(flint_rand_t& state, slong iterations = 128, slong minlen = 2, slong maxlen = 32, slong minprec = 8, slong maxprec = 2048, slong minbits = 10, slong maxbits = 40)
{
    return Catch::Generators::GeneratorWrapper<renf_t&>(std::unique_ptr<Catch::Generators::IGenerator<renf_t&>>(new RenfGenerator(state, iterations, minlen, maxlen, minprec, maxprec, minbits, maxbits)));
}

}

namespace Catch {

/*
 * Make printing of renf_t work in Catch, i.e., show meaningful debug output for CAPTURE(nf).
 */
template <>
struct StringMaker<renf_t>
{
    static renf* nf;

    static std::string convert(renf_t const& nf)
    {
        StringMaker<renf_t>::nf = const_cast<renf*>(nf);

        char * emb = arb_get_str(nf->emb, arf_bits(arb_midref(nf->emb)), 0);
        char * pol = fmpq_poly_get_str_pretty(nf->nf->pol, "x");

        std::stringstream str;
        str << "renf_t(emb=" << emb << ", pol=" << pol << ", prec=" << nf->prec << ")";

        flint_free(pol);
        flint_free(emb);

        return str.str();
    }
};

renf* StringMaker<renf_t>::nf = nullptr;

}

#endif
