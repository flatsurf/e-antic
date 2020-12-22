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

#include "../e-antic/renf.h"

#include "external/catch2/single_include/catch2/catch.hpp"

namespace {

/*
 * A generator of random renf_t number fields.
 */
struct RenfGenerator : public Catch::Generators::IGenerator<renf_t&>
{
    flint_rand_t& state;
    ulong minlen, maxlen, minprec, maxprec, minbits, maxbits;

    mutable renf_t nf;
    mutable bool has_value_for_iteration = false;

    RenfGenerator(flint_rand_t& state, ulong minlen, ulong maxlen, ulong minprec, ulong maxprec, ulong minbits, ulong maxbits) : state(state), minlen(minlen), maxlen(maxlen), minprec(minprec), maxprec(maxprec), minbits(minbits), maxbits(maxbits)
    {
        assert(maxlen > minlen);
        assert(maxprec > minprec);
        assert(maxbits > minbits);
    }

    void clear()
    {
        if (has_value_for_iteration)
            renf_clear(nf);
        has_value_for_iteration = false;
    }

    bool next() override
    {
        clear();
        return true;
    }

    renf_t& get() const override
    {
        if (!has_value_for_iteration)
        {
            ulong len = minlen + n_randint(state, maxlen - minlen);
            ulong prec = minprec + n_randint(state, maxprec - minprec);
            mp_bitcnt_t bits = minbits + n_randint(state, maxbits - minbits);

            renf_randtest(nf, state, static_cast<slong>(len), static_cast<slong>(prec), bits);

            has_value_for_iteration = true;
        }
        return nf;
    }

    ~RenfGenerator() override
    {
        clear();
    }
};


/*
 * Wrap RenfGenerator for use as GENERATE(renfs(...))
 */
[[maybe_unused]] Catch::Generators::GeneratorWrapper<renf_t&> renfs(flint_rand_t& state, ulong minlen = 2, ulong maxlen = 32, ulong minprec = 8, ulong maxprec = 2048, ulong minbits = 10, ulong maxbits = 40)
{
    return Catch::Generators::GeneratorWrapper<renf_t&>(std::unique_ptr<Catch::Generators::IGenerator<renf_t&>>(new RenfGenerator(state, minlen, maxlen, minprec, maxprec, minbits, maxbits)));
}

}

namespace Catch {

/*
 * Make printing of renf_t work in Catch, i.e., show meaningful debug output for CAPTURE(nf).
 */
template <>
struct StringMaker<renf_t>
{
    static renf* latest;

    static std::string convert(renf_t const& nf)
    {
        StringMaker<renf_t>::latest = const_cast<renf*>(nf);

        char * emb = arb_get_str(nf->emb, arf_bits(arb_midref(nf->emb)), 0);
        char * pol = fmpq_poly_get_str_pretty(nf->nf->pol, "x");

        std::stringstream str;
        str << "renf_t(emb=" << emb << ", pol=" << pol << ", prec=" << nf->prec << ")";

        flint_free(pol);
        flint_free(emb);

        return str.str();
    }
};

renf* StringMaker<renf_t>::latest = nullptr;

}

#endif
